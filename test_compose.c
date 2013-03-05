#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "fst.h"
#include "test.h"

static void test_compose() {
    char * fst_str_0 = "0 1 1 1\n1"; 
    char * fst_str_1 = "0 1 1 1\n1"; 
    char * fst_str_2 = "0 1 1 1\n1"; 

    struct _fst * fst_0 = fst_create();
    struct _fst * fst_1 = fst_create();
    struct _fst * fst_2 = fst_create();
    struct _fst * fst_3 = fst_create();

    fst_compile_str(fst_0, fst_str_0);
    fst_compile_str(fst_1, fst_str_1);
    fst_compile_str(fst_2, fst_str_2);

    fst_compose(fst_0, fst_1, fst_3);
    fst_print(fst_3);

    assert_fst_equal( fst_2, fst_3 );

    fst_remove(fst_0);
    fst_remove(fst_1);
    fst_remove(fst_2);
    fst_remove(fst_3);
}

int main() {
    test_compose();
    return 0;
}


