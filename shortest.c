#include <assert.h>
#include <stdlib.h>
#include "fst.h"
#include "sr.h"
#include "heap.h"

/* 
Using priority queue it is possible to 
get rid of hash table and use only bitmap-based iterator
*/

static struct _sr sr;
static weight_t * W = NULL;
static struct _arc * B = NULL;

static int states_cmp(const void * a, const void * b) {
    return W[*(state_t*)a] == sr.sum(
                    W[*(state_t*)a],
                    W[*(state_t*)b]
    ) ? 1:0;
}

static int states_key_eq(const void * a, const void * b) {
    return *(state_t*)a == *(state_t*)b;
}

static void backtrace(struct _fst * path, state_t f) {
    struct _arc arc;
    state_t s=f;
    state_t n=0; 

    fst_add_state(path);

    while( s != 0 ) {
        arc = B[s];
        s = arc.state;
        fst_add_state(path);
        n++;
    }

    fst_set_final(path, n, sr.one);

    s=f;
    while( s != 0 ) {
        arc = B[s];
        s = arc.state;
        fst_add_arc(path, n-1, n, arc.ilabel, arc.olabel, arc.weight);
        --n;
    }

}

struct _fst * fst_shortest(const struct _fst * fst, struct _fst * path) {
    struct _state *     state;
    struct _arc   *     arc;
    struct _arc         r_arc;
    state_t             p,q;
    int                 i;

    assert ( fst->sr_type == SR_TROPICAL );
    sr = sr_get(fst->sr_type);
    
    struct _heap * Q = heap_create(states_cmp, sizeof(state_t), 0);

    W = malloc(sizeof(weight_t) * fst->n_states);
    B = malloc(sizeof(struct _arc) * fst->n_states);

    // initialize arrays
    for (state_t i=0; i < fst->n_states; ++i)
        W[i] = sr.zero;  

    q = fst->start; 
    W[q] = sr.one;

    heap_insert(Q, &q); 

    while( ( heap_pop(Q, &p)) !=NULL ) {
        state = &fst->states[p];

        if (state->final == 1) {
            backtrace(path, p);
            break;
        }
    
        for (arc_t a=0; a < state->n_arcs; ++a) {
            arc = &state->arcs[a];
            q = arc->state;
            
            // Technical check: do not explore unreacheble states
            if (arc->weight == sr.zero)
                continue;
            
            // Using the zero weights as marker of unexplored states

            if (W[q] == sr.zero) {
                heap_insert(Q, &q);
            }
        
            if (W[q] != sr.sum( 
                    W[q], sr.prod(W[p], arc->weight) ) ) {
    
                W[q] = sr.prod(W[p], arc->weight);  

                r_arc = *arc;
                r_arc.state = p;
                
                B[q] = r_arc;

                if ((i = heap_find(Q, &q, states_key_eq)) != -1) {
                    heap_update(Q, &q, i);
                }
            }

        }
    }

    heap_remove(Q);

    free(W);
    free(B);

    return path;
}

