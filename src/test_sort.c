#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "test.h"
#include "fst.h"

static void test_fst_sort() {
    struct _fst * fst_a = fst_create();
    struct _fst * fst_b = fst_create();

    char str_a [] = "0 0 1 1\n0 0 0 0\n1";
    char str_b [] = "0 0 0 0\n0 0 1 1\n1";

    fst_compile_str(fst_a, str_a);
    fst_compile_str(fst_b, str_b);

    fst_arc_sort(fst_a, 0);
    assert_fst_equal(fst_a, fst_b); 

    fst_remove(fst_a);
    fst_remove(fst_b);
}

int main() {
    test_fst_sort();
    return 0;
}
