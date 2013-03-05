#include <stdio.h>
#include "fst.h"
#include "queue.h"

void fst_print(const struct _fst * fst) {
    struct _state state;
    struct _arc arc;

    state_t s;
    arc_t   a;
    
    struct _queue *finals = queue_create(sizeof(state_t));
    
    for(s=0; s < fst->n_states; ++s) {
        state = fst->states[s];
        for(a=0; a < state.n_arcs; ++a) {
            arc = state.arcs[a];

            printf("%"PRIu32"\t%"PRIu32"\t%"PRIu32"\t%"PRIu32"\t%.5f\n",
                s,
                arc.state,
                arc.ilabel,
                arc.olabel,
                arc.weight);
        }

        if(state.final == 1) {
            queue_enque(finals, &s);
        }
    }

    while(queue_deque(finals, &s) != NULL) {
        state = fst->states[s];
        printf("%"PRIu32"\t%f\n", s, state.weight);
    }

    queue_remove(finals);
}
