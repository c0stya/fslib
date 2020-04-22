#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "fst.h"

static void test_fst_read_write() {
    const char filename [] = "tests/test_io.t";

    struct _fst * fst = fst_create();
    state_t state_a = fst_add_state(fst);
    state_t state_b = fst_add_state(fst);

    for (arc_t i=0; i < 10; ++i) {
        fst_add_arc(fst, state_a, state_b, i, i, i);
    }

    fst_set_final(fst, state_b, 1.0);

    fst_fwrite(fst, filename);
    fst_remove(fst);

    fst = fst_create();
    fst_fread(fst, filename);

    size_t count = 0;

    struct _state *     state = &fst->states[0];
    struct _arc         arc;

    assert(fst->n_states == 2);

    assert(fst->states[0].final == 0);
    assert(fst->states[1].final == 1);
    assert(fst->states[1].weight == 1.0);

    for(arc_t a = 0; a < state->n_arcs; ++a) {
        arc = state->arcs[a];
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

int main() {
    test_fst_read_write();
    test_fst_stress();
    return 0;
}
