#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "fst.h"

void test_fst_create() {
    struct _fst * fst = fst_create();
    state_t state_a = fst_add_state(fst);
    state_t state_b = fst_add_state(fst);

    for (int i=0; i < 10; ++i) {
        fst_add_arc(fst, state_a, state_b, i, i, i);
    }

    size_t count = 0;

    struct _state       state = fst->states[state_a];
    struct _arc         arc;
    arc_t               a;

    assert(fst->n_states == 2);

    for(a = 0; a < state.n_arcs; ++a) {
        arc = state.arcs[a];
        assert ( arc.state == state_b);
        assert ( arc.ilabel == count ); 
        assert ( arc.olabel == count ); 
        assert ( arc.weight == count ); 
        ++count;
    }

    assert ( count  == 10 ) ;
    fst_remove(fst);
}


void test_fst_stress() {
    struct _fst * fst = fst_create();
    state_t sa = fst_add_state(fst);
    state_t sb = fst_add_state(fst);

    arc_t arcs_size = 1000000;

    for (arc_t i=0; i < arcs_size; ++i) {
        fst_add_arc(fst, sa, sb, i, i, i);
    }

    size_t count = 0;
    struct _arc arc;
    struct _state state = fst->states[sa];

    assert(fst->n_states == 2);

    for(arc_t i = 0; i < state.n_arcs; ++i) {
        arc = state.arcs[i];
        assert ( arc.state == sb);
        assert ( arc.ilabel == count ); 
        assert ( arc.olabel == count ); 
        assert ( arc.weight == count ); 
        ++count;
    }

    assert ( count  == arcs_size ) ;
    fst_remove(fst);
}

void test_fst_copy() {
    struct _fst * fst_a = fst_create();
    struct _fst * fst_b = fst_create();
    state_t state_a = fst_add_state(fst_a);
    state_t state_b = fst_add_state(fst_a);

    for (int i=0; i < 10; ++i) {
        fst_add_arc(fst_a, state_a, state_b, i, i, i);
    }

    fst_copy(fst_a, fst_b);

    size_t count = 0;
    struct _state       state = fst_b->states[state_a];
    struct _arc         arc;

    assert(fst_b->n_states == 2);

    for(arc_t a = 0; a < state.n_arcs; ++a) {
        arc = state.arcs[a];
        assert ( arc.state == state_b);
        assert ( arc.ilabel == count ); 
        assert ( arc.olabel == count ); 
        assert ( arc.weight == count ); 
        ++count;
    }

    assert ( count  == 10 ) ;
    fst_remove(fst_a);
    fst_remove(fst_b);
}

void test_fst_relabel() {
    struct _fst * fst = fst_create();
    state_t a = fst_add_state(fst);
    state_t b = fst_add_state(fst);

    fst_add_arc(fst, a, b, 0, 0, 0);

    struct _arc * arc;
    arc = &fst->states[a].arcs[0];

    fst_relabel(fst, 0, 1, 0);

    assert( arc->ilabel == 1);
    assert( arc->olabel == 0);

    fst_relabel(fst, 0, 1, 1);

    assert( arc->ilabel == 1);
    assert( arc->olabel == 1);

    fst_remove(fst);
}



int main() {
    test_fst_create();
    test_fst_stress();
    test_fst_copy();
    test_fst_relabel();
    /*
    test_fst_state_add();
    test_fst_arc_add();
    test_fst_state_remove();
    test_fst_arc_remove();
    test_fst_side();
    */
    return 0;
}


