#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "queue.h"
#include "bitset.h"
#include "fst.h"
#include "iter.h"

/*

Iterate through the whole fst beginning from start state.
The iteration discipline is FIFO.

The cache of visited states is stored in the 'marked' hash table

*/

struct _fst_iter * fst_iter_create(const struct _fst * fst) {
    struct _fst_iter * iter;
    iter = malloc(sizeof(struct _fst_iter));
    iter->fst = fst;
    iter->state = fst->start;
    iter->marked = bitset_create(fst->n_states);
    iter->queue = queue_create(sizeof(state_t));

    queue_enque(iter->queue, &iter->state);
    bitset_set(iter->marked, iter->state);

    return iter;
}

void fst_iter_remove(struct _fst_iter * iter) {
    bitset_remove(iter->marked); 
    queue_remove(iter->queue);
    iter->fst = NULL;
    free(iter);
}

state_t fst_iter_next(struct _fst_iter * iter) {
    struct _arc * arc;
    struct _state * state;
    state_t s;
    arc_t a;
    
    if((queue_deque(iter->queue, &s)) != NULL) {
        iter->state = s; 
        state = &iter->fst->states[s]; 
        for(a=0; a < state->n_arcs; ++a) {
            arc = &state->arcs[a];
            if (bitset_get(iter->marked, arc->state) == 0) {
                queue_enque(iter->queue, &arc->state);
                bitset_set(iter->marked, arc->state);
            }
        }
    } else {
        iter->state = -1;
    }

    return iter->state;
}

int fst_iter_visited(struct _fst_iter * iter, state_t s) {
    return bitset_get(iter->marked, s);
}


