#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "fst.h"
#include "test.h"

void test_reverse() {
    struct _fst * fst = fst_create();
    state_t state_a = fst_add_state(fst);
    state_t state_b = fst_add_state(fst);

    fst_add_arc(fst, state_a, state_b, 1, 1, 1);

    fst_reverse(fst);

    struct _state       state = fst->states[state_b];
    struct _arc         arc;

    assert( fst->n_states == 2);
    assert( state.n_arcs == 1);

    arc = state.arcs[0];
    assert ( arc.state == state_a);
    assert ( arc.ilabel == 1 ); 
    assert ( arc.olabel == 1 ); 
    assert ( arc.weight == 1 ); 

    fst_remove(fst);
}

void test_rm_states() {
    char * fst_str_0 = "0 1 1 1\n1 2 1 1\n2 3 1 1\n3"; 
    char * fst_str_1 = "0 1 1 1\n1 2 1 1"; 

    struct _fst * fst_0 = fst_create();
    struct _fst * fst_1 = fst_create();

    fst_compile_str(fst_0, fst_str_0);
    fst_compile_str(fst_1, fst_str_1);

    struct _bitset * bs = bitset_create(4);

    bitset_set(bs, 3);

    fst_rm_states(fst_0, bs);

    assert_fst_equal( fst_0, fst_1 );

    bitset_remove(bs);

    fst_remove(fst_0);
    fst_remove(fst_1);
}

void test_trim() {
    char * fst_str_0 = "0 1 1 1\n0 2 1 1\n3 0 1 1\n1"; 
    char * fst_str_1 = "0 1 1 1\n1";

    struct _fst * fst_0 = fst_create();
    struct _fst * fst_1 = fst_create();

    fst_compile_str(fst_0, fst_str_0);
    fst_compile_str(fst_1, fst_str_1);

    fst_trim(fst_0);
    fst_print(fst_0);
    
    assert_fst_equal( fst_0, fst_1 );

    fst_remove(fst_0);
    fst_remove(fst_1);
}

void test_trim_close() {
    char * fst_str_0 = "0 1 1 1\n0 2 1 1\n3 0 1 1\n1\n2"; 
    char * fst_str_1 = "0 1 1 1\n1";

    struct _fst * fst_0 = fst_create();
    struct _fst * fst_1 = fst_create();

    fst_compile_str(fst_0, fst_str_0);
    fst_compile_str(fst_1, fst_str_1);

    fst_trim(fst_0);
    fst_print(fst_0);
    
    //assert_fst_equal( fst_0, fst_1 );

    fst_remove(fst_0);
    fst_remove(fst_1);
}

int main() {
    test_reverse();
    test_rm_states();
    test_trim();
    test_trim_close();
    return 0;
}


