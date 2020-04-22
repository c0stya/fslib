#include <assert.h>
#include "fst.h"

/* Auxillary methods for testing */

void assert_arc_equal(const struct _arc * arc_a, const struct _arc * arc_b) {
    assert ( arc_a->state == arc_b->state ); 
    assert ( arc_a->ilabel == arc_b->ilabel ); 
    assert ( arc_a->olabel == arc_b->olabel ); 
    assert ( arc_a->weight == arc_b->weight ); 
}

void assert_state_equal(const struct _state * state_a, const struct _state * state_b) {
    // Disable it cause I still need remove state-weights code
    // assert ( state_a->weight == state_b->weight );
    assert ( state_a->final == state_b->final );
}

void assert_fst_equal(const struct _fst * a, const struct _fst * b) {
    struct _state * state_a; 
    struct _state * state_b; 

    struct _arc * arc_a; 
    struct _arc * arc_b; 

    assert (a->start ==     b->start);
    assert (a->sr_type ==   b->sr_type);
    assert (a->n_states ==  b->n_states);
    
    for (state_t st = 0; st < a->n_states && st < b->n_states; ++st) {

        state_a = &a->states[st];
        state_b = &b->states[st];
        
        assert( state_a->n_arcs == state_b->n_arcs );
        assert_state_equal( state_a, state_b );

        for (arc_t arc = 0; 
                    arc < state_a->n_arcs && arc < state_b->n_arcs; 
                    ++arc) {
            arc_a = &state_a->arcs[arc];
            arc_b = &state_b->arcs[arc];

            assert_arc_equal( arc_a, arc_b );
        }
    }
}
