#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "fst.h"
#include "test.h"

void test_fst_compile() {
    struct _fst * fst = fst_create();

    char str [] = "0 1 1 1 1.0\n1 1.0";
    fst_compile_str(fst, str);
    
    assert( fst->n_states == 2);
    struct _state s0, s1;

    s0 = fst->states[0];
    s1 = fst->states[1];

    assert( s0.final == 0 );
    assert( s1.final == 1 );
    // assert( s1.weight == 1.0 );

    assert( s0.n_arcs == 1 );
    assert( s0.arcs[0].ilabel == 1);
    assert( s0.arcs[0].olabel == 1);
    assert( s0.arcs[0].weight == 1.0);

    fst_remove(fst);
}

void test_test_equal() {
    struct _fst * fst_a = fst_create();
    struct _fst * fst_b = fst_create();

    char str_a [] = "0 1 1 1\n1 1.0";
    char str_b [] = "0 1 1 1\n1 1.0";

    fst_compile_str(fst_a, str_a);
    fst_compile_str(fst_b, str_b);
    
    assert_fst_equal( fst_a, fst_b );
    
    fst_remove(fst_a);
    fst_remove(fst_b);
}

int main() {
    test_fst_compile();
    test_test_equal();
    return 0;
}
