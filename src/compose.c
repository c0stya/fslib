#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "queue.h"
#include "fst.h"
#include "hash.h"

static int cmp_pair(void * a, void * b) {
    return memcmp(a, b, sizeof(struct _spair)) == 0 ? 1 : 0;
}

static size_t hash_pair(void * p) {
    size_t hash = 17;
    hash = hash*31 + ((struct _spair *)p)->a;
    hash = hash*31 + ((struct _spair *)p)->b;
    return hash;
}

void match_arcs(    
                    const struct _fst * fst_a,
                    const struct _fst * fst_b,
                    const struct _spair * pair,
                    struct _sr * sr,
                    struct _queue * mq)
{
    struct _state * state_a = &fst_a->states[pair->a];
    struct _state * state_b = &fst_b->states[pair->b];
    
    int osort = fst_a->flags & OSORT;
    int isort = fst_b->flags & ISORT;

    size_t asize = sizeof(struct _arc);

    arc_t m = state_a->n_arcs + 1;
    arc_t n = state_b->n_arcs + 1;
    
    struct _arc * arcs_a = malloc(asize * m);
    struct _arc * arcs_b = malloc(asize * n);

    memcpy(arcs_a + 1, state_a->arcs, asize * (m-1));
    memcpy(arcs_b + 1, state_b->arcs, asize * (n-1));

    struct _arc loop_arc_a = { 
            .state = pair->a, 
            .ilabel = EPS,
            .olabel = EPS,
            .weight = sr->one 
    };

    struct _arc loop_arc_b = { 
            .state = pair->b, 
            .ilabel = EPS,
            .olabel = EPS,
            .weight = sr->one 
    };

    arcs_a[0] = loop_arc_a;
    arcs_b[0] = loop_arc_b;

    if (isort && osort)
        match_full_sorted(arcs_a, arcs_b, m, n, mq);
    else
        if (isort || osort) {
            if (isort) {
                match_half_sorted(arcs_a, arcs_b, m, n, mq);
            } else {
                match_half_sorted_rev(arcs_a, arcs_b, m, n, mq);
            }
        } else {
            match_unsorted(arcs_a, arcs_b, m, n, mq);
        }

    free(arcs_a);
    free(arcs_b);
}

struct _fst * fst_compose(
    const struct _fst * fst_a, 
    const struct _fst * fst_b, 
    struct _fst * fst_c )
{
    struct _sr sr = sr_get(fst_a->sr_type);
    struct _queue * q = queue_create(sizeof(struct _spair));
    struct _queue * mq = queue_create(sizeof(struct _arc_pair)); 

    struct _hash * marked = hash_create(
                    hash_pair,
                    cmp_pair,
                    sizeof(struct _spair),
                    sizeof(state_t), 
                    1000 ); // try to predict

    struct _spair      pair;
    struct _arc_pair   mi;    // composition item

    state_t sc, dst_sc;

    struct _state * state_a, * state_b;
    struct _state * dst_state_a, * dst_state_b;
    struct _arc * arc_a, * arc_b;

    // Add initial stack to agenda
    pair.a = fst_a->start;
    pair.b = fst_b->start;
    
    queue_enque(q, &pair);

    while( (queue_deque(q, &pair)) != NULL ) {

        state_a = &fst_a->states[pair.a];
        state_b = &fst_b->states[pair.b];


        if (hash_get(marked, &pair, &sc) == NULL) {

            // Create a new state
            sc = fst_add_state(fst_c);
            
            if (state_a->final && state_b->final)
                fst_set_final(fst_c, sc, sr.one);

            if (pair.a == fst_a->start && pair.b == fst_b->start)
                fst_c->start = sc;

            hash_add(marked, &pair, &sc);
        }

        // Match procedure
        match_arcs(     fst_a, fst_b, 
                        &pair,
                        &sr,
                        mq  );

        while ( queue_deque(mq, &mi) != NULL ) {

            arc_a = &mi.a;
            arc_b = &mi.b;
            
            pair.a = arc_a->state;
            pair.b = arc_b->state;

                
            // State lookup
            if (hash_get(marked, &pair, &dst_sc) == NULL) {
                
                // Create a new state
                dst_state_a = &fst_a->states[pair.a];
                dst_state_b = &fst_b->states[pair.b];

                dst_sc = fst_add_state(fst_c);
                
                if (dst_state_a->final && dst_state_b->final) {
                    fst_set_final(fst_c, dst_sc, sr.one);
                }

                queue_enque(q, &pair);

                // Add to a makred table
                hash_add(marked, &pair, &dst_sc);
            }

            fst_add_arc(
                fst_c,
                sc, 
                dst_sc, 
                arc_a->ilabel, 
                arc_b->olabel,
                sr.prod(arc_a->weight, arc_b->weight));
        }
    }

    queue_remove(q);
    queue_remove(mq);
    hash_remove(marked);

    return fst_c;
}
