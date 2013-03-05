#include "fst.h"
#include "test.h"

static void test_union() {
    struct _fst * a = fst_create();
    struct _fst * b = fst_create();
    struct _fst * c = fst_create();

    char * str_a = "0 1 1 1\n1";
    char * str_b = "0 1 2 2\n1";
    char * str_c = "0 1 1 1\n2 3 2 2\n0 2 0 0\n1\n3";

    fst_compile_str(a, str_a);
    fst_compile_str(b, str_b);
    fst_compile_str(c, str_c);
    
    fst_union(a, b);

    assert_fst_equal( a, c );

    fst_remove(a);
    fst_remove(b);
    fst_remove(c);
}

int main() {
    test_union();
    return 0;
}
