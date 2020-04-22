#include <stdlib.h>
#include <assert.h>
#include "fst.h"
#include "iter.h"
#include "bitset.h"
#include "queue.h"

void fst_close(struct _fst * fst, struct _queue * finals) {
    /*
    This methods replace multiple final states with
    a single final state. 

    */
    if ( finals->n_items < 2)
        return;    

    struct _state * state;
    struct _sr sr = sr_get(fst->sr_type); 
    state_t final;
    state_t s;

    final = fst_add_state(fst);
    fst_set_final(fst, final, sr.one);


    while(queue_deque(finals, &s) != NULL) {
        state = &fst->states[s];
        state->final = 0;
        fst_add_arc(    fst,
                        s,
                        final,
                        EPS,
                        EPS,
                        state->weight);
    }
}

void fst_reverse(struct _fst * fst) {
    struct _sr sr = sr_get(fst->sr_type); 
    struct _state * state;
    struct _arc * arc;
    struct _fst * orig = fst_create();
    state_t s;
    state_t start_s = fst->start;

    fst_copy(fst, orig);

    for(s=0; s < fst->n_states; ++s ) {
        state = &fst->states[s];

        // 'delete' arcs from each of the automaton states
        state->n_arcs = 0;
       
        // change start to final 
        // NOTE: fst should have only 1 start state
        if (state->final) {
            fst->start = s;
            state->final = 0;
        }
    }

    // set start as final
    fst_set_final(fst, start_s, sr.one);

    // add reversed arcs to the automaton
    for(s=0; s < orig->n_states; ++s) {
        state = &orig->states[s];         

        for(arc_t a=0; a < state->n_arcs; ++a) { 
            arc = &state->arcs[a];
            fst_add_arc(    fst, 
                            arc->state, 
                            s, 
                            arc->ilabel, 
                            arc->olabel, 
                            arc->weight);
        }
    }

    fst_remove(orig);
}

struct _fst * fst_rm_states(struct _fst * fst, struct _bitset * mask) {
    struct _state * state;
    struct _arc * arc;

    state_t idx[sizeof(state_t) * fst->n_states];

    state_t shift = 0;

    for(state_t s=0; s < fst->n_states; ++s) {
        if (bitset_get(mask, s) ) {
            ++shift;
        }
        else {
            idx[s] = shift;
            fst->states[s - shift] = fst->states[s];
        }
    } 

    fst->n_states -= shift;

    // TODO: do not forget to clean up arcs of removed states

    // fix arc's destination state id
    for(state_t s=0; s < fst->n_states; ++s) {
        state = &fst->states[s];

        arc_t sh = 0;
        
        for(arc_t a=0; a < state->n_arcs; ++a) {
            arc = &state->arcs[a];
            arc->state -= idx[arc->state];

            // found link to non-existing state
            if (arc->state >= fst->n_states) {
                ++sh;
            } else {
                state->arcs[a - sh] = state->arcs[a];
            }
        }
        
        state->n_arcs -= sh;
    }

    return fst;
}

static void fst_get_finals(struct _fst * fst, struct _queue * finals) {
    struct _state * state;

    for (state_t s=0; s < fst->n_states; ++s) {
        state = fst->states + s;
        if (state->final)
            queue_enque(finals, &s);
    }
}

struct _fst * fst_trim(struct _fst * fst) {
    struct _queue * finals = queue_create(sizeof(state_t));

    /*
    Before trimming check that we have at least one final states.
    If not then all the states are not co-accessible and
    there resulting automaton should be empty.
    */

    fst_get_finals(fst, finals);

    if (finals->n_items == 0) {
        fst_empty(fst);
         
    } else { 

        if (finals->n_items > 1) {
            fst_close(fst, finals);
        }

        /* 
        First lets traverse it directly and remove
        all the unreachable states    
        */

        struct _fst_iter * iter = fst_iter_create(fst);
        while ( fst_iter_next(iter) != -1 ) 
            ;
        
        /* 
        Then inverse the arcs and traverse starting
        from the final states of the original transducer
        
        NOTE: we should refresh finals queue
        */

        fst_reverse(fst);

        struct _fst_iter * iter_rev = fst_iter_create(fst);
        while ( fst_iter_next(iter_rev) != -1 ) 
            ;

        /* Reverse it back */
        fst_reverse(fst);

        /* Remove non accessible and non coacessible states */

        bitset_intersect(iter->marked, iter_rev->marked);

        bitset_toggle_all(iter->marked);

        //fst_rm_states(fst, bitset_toggle_all(iter->marked));
        fst_rm_states(fst, iter->marked);


        fst_iter_remove(iter_rev);
        fst_iter_remove(iter);
    }

    queue_remove(finals);

    return fst;
}

