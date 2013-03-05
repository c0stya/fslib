#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "queue.h"
#include "fst.h"
#include "hash.h"

static int cmp_triple(void * a, void * b) {
    return memcmp(a, b, sizeof(struct _striple)) == 0 ? 1 : 0;
}

static size_t hash_triple(void * p) {
    size_t hash = 17;
    hash = hash*31 + ((struct _striple *)p)->a;
    hash = hash*31 + ((struct _striple *)p)->b;
    hash = hash*31 + ((struct _striple *)p)->c;
    return hash;
}

void match_arcs(    
                    const struct _fst * fst_a,
                    const struct _fst * fst_b,
                    const struct _striple * tri,
                    struct _sr * sr,
                    struct _queue * mq)
{
    struct _state * state_a = &fst_a->states[tri->a];
    struct _state * state_b = &fst_b->states[tri->b];
    
    int osort = fst_a->flags & OSORT;
    int isort = fst_b->flags & ISORT;

    size_t asize = sizeof(struct _arc);

    arc_t m = state_a->n_arcs + 1;
    arc_t n = state_b->n_arcs + 1;
    
    struct _arc * arcs_a = malloc(asize * m);
    struct _arc * arcs_b = malloc(asize * n);

    memcpy(arcs_a + 1, state_a->arcs, asize * (m-1));
    memcpy(arcs_b + 1, state_b->arcs, asize * (n-1));

    struct _arc loop_arc = { 
            .state = tri->a, 
            .ilabel = EPS,
            .olabel = EPS_L,
            .weight = sr->one 
    };

    arcs_a[0] = loop_arc;

    loop_arc.state = tri->b;
    loop_arc.ilabel = EPS_L;
    loop_arc.olabel = EPS;

    arcs_b[0] = loop_arc;

    if (isort && osort)
        match_full_sorted(arcs_a, arcs_b, tri->c, m, n, mq);
    else
        if (isort || osort) {
            if (osort)
                match_half_sorted(arcs_a, arcs_b, tri->c, m, n, mq);
            else {
                // reverse the pairs
                struct _match_item mi, rmi;
                match_half_sorted(arcs_b, arcs_a, tri->c, n, m, mq);
                for (size_t i=0; i < mq->n_items; ++i) {
                    queue_deque(mq, &mi);
                    rmi.a = mi.b;
                    rmi.b = mi.a;
                    rmi.f = mi.f;
                    queue_enque(mq, &rmi);
                }
            }
        } else {
            match_unsorted(arcs_a, arcs_b, tri->c, m, n, mq);
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
    struct _queue * q = queue_create(sizeof(struct _striple));
    struct _queue * mq = queue_create(sizeof(struct _match_item)); 

    struct _hash * marked = hash_create(
                    hash_triple,
                    cmp_triple,
                    sizeof(struct _striple),
                    sizeof(state_t), 
                    fst_a->n_states * fst_b->n_states * 5 ); // try to predict

    struct _striple      tri, tri2;
    struct _match_item   mi;    // composition item

    state_t sc, dst_sc;

    struct _state * state_a, * state_b;
    struct _state * dst_state_a, * dst_state_b;
    struct _arc * arc_a, * arc_b;

    // Add initial stack to agenda
    tri.a = fst_a->start;
    tri.b = fst_b->start;
    tri.c = 0;
    
    queue_enque(q, &tri);

    while( (queue_deque(q, &tri)) != NULL ) {

        state_a = &fst_a->states[tri.a];
        state_b = &fst_b->states[tri.b];

        tri2 = tri;
        tri2.c = 0;

        if (hash_get(marked, &tri2, &sc) == NULL) {

            // Create a new state
            sc = fst_add_state(fst_c);
            
            if (state_a->final && state_b->final)
                fst_set_final(fst_c, sc,
                    sr.prod(state_a->weight, state_b->weight));

            if (tri.a == fst_a->start && tri.b == fst_b->start && tri.c == 0)
                fst_c->start = sc;

            hash_add(marked, &tri, &sc);
        }

        // Match procedure
        match_arcs(     fst_a, fst_b, 
                        &tri,
                        &sr,
                        mq  );

        //printf("== %zd %zd %zd\n", tri.a, tri.b, tri.c);

        while ( queue_deque(mq, &mi) != NULL ) {

            arc_a = &mi.a;
            arc_b = &mi.b;
            
            //printf("%zd\n", mi.f);

            tri.a = arc_a->state;
            tri.b = arc_b->state;
            tri.c = mi.f;

            tri2 = tri;
            tri2.c = 0;
                
            // State lookup
            if (hash_get(marked, &tri2, &dst_sc) == NULL) {
                
                // Create a new state
                dst_state_a = &fst_a->states[tri.a];
                dst_state_b = &fst_b->states[tri.b];

                dst_sc = fst_add_state(fst_c);
                
                if (dst_state_a->final && dst_state_b->final) {
                    fst_set_final(fst_c, dst_sc,
                        sr.prod(dst_state_a->weight, dst_state_b->weight));
                }

                queue_enque(q, &tri);

                // Add to a makred table
                hash_add(marked, &tri, &dst_sc);
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
