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

static void test_compose_1() {
    char * fst_str_0 = "0 1 1 1\n1 2 1 1\n2";
    char * fst_str_1 = "0 0 1 1\n0 0 1 0\n0 0 0 1\n0";
    char * fst_str_2 = "0 0	0 1\n0 1 1 1\n0 1 1 0\n1 1 0 1\n1 2 1 1\n1 2 1 0\n2 2 0 1\n2";

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

static void test_compose_sorted_full_0() {

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

    fst_arc_sort(fst_0, 1);
    fst_arc_sort(fst_1, 0);

    fst_compose(fst_0, fst_1, fst_3);
    fst_print(fst_3);

    assert_fst_equal( fst_2, fst_3 );

    fst_remove(fst_0);
    fst_remove(fst_1);
    fst_remove(fst_2);
    fst_remove(fst_3);
}



static void test_compose_sorted_full_1() {
    char * fst_str_0 = "0 1 1 1\n1 2 1 1\n2";
    char * fst_str_1 = "0 0 1 1\n0 0 1 0\n0 0 0 1\n0";
    char * fst_str_2 = "0 0	0 1\n0 1 1 1\n0 1 1 0\n1 1 0 1\n1 2 1 1\n1 2 1 0\n2 2 0 1\n2";

    struct _fst * fst_0 = fst_create();
    struct _fst * fst_1 = fst_create();
    struct _fst * fst_2 = fst_create();
    struct _fst * fst_3 = fst_create();

    fst_compile_str(fst_0, fst_str_0);
    fst_compile_str(fst_1, fst_str_1);
    fst_compile_str(fst_2, fst_str_2);


    fst_arc_sort(fst_0, 1);
    fst_arc_sort(fst_1, 0);

    fst_compose(fst_0, fst_1, fst_3);
    
    printf("Full sorted:\n");
    fst_print(fst_3);

    assert_fst_equal( fst_2, fst_3 );

    fst_remove(fst_0);
    fst_remove(fst_1);
    fst_remove(fst_2);
    fst_remove(fst_3);
}

static void test_compose_sorted_half_0() {

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

    fst_arc_sort(fst_1, 0);

    fst_compose(fst_0, fst_1, fst_3);
    fst_print(fst_3);

    assert_fst_equal( fst_2, fst_3 );

    fst_remove(fst_0);
    fst_remove(fst_1);
    fst_remove(fst_2);
    fst_remove(fst_3);
}

static void test_compose_sorted_half_1() {
    char * fst_str_0 = "0 1 1 1\n1 2 1 1\n2";
    char * fst_str_1 = "0 0 1 1\n0 0 1 0\n0 0 0 1\n0";
    char * fst_str_2 = "0 0	0 1\n0 1 1 1\n0 1 1 0\n1 1 0 1\n1 2 1 1\n1 2 1 0\n2 2 0 1\n2";

    struct _fst * fst_0 = fst_create();
    struct _fst * fst_1 = fst_create();
    struct _fst * fst_2 = fst_create();
    struct _fst * fst_3 = fst_create();

    fst_compile_str(fst_0, fst_str_0);
    fst_compile_str(fst_1, fst_str_1);
    fst_compile_str(fst_2, fst_str_2);


    fst_arc_sort(fst_1, 0);

    fst_compose(fst_0, fst_1, fst_3);
    
    printf("Half sorted direct:\n");
    fst_print(fst_3);

    assert_fst_equal( fst_2, fst_3 );

    fst_remove(fst_0);
    fst_remove(fst_1);
    fst_remove(fst_2);
    fst_remove(fst_3);
}

static void test_compose_sorted_half_2() {
    char * fst_str_0 = "0 1 1 1\n1 2 1 1\n2";
    char * fst_str_1 = "0 0 1 1\n0 0 1 0\n0 0 0 1\n0";
    char * fst_str_2 = "0 0	0 1\n0 1 1 1\n0 1 1 0\n1 1 0 1\n1 2 1 1\n1 2 1 0\n2 2 0 1\n2";

    struct _fst * fst_0 = fst_create();
    struct _fst * fst_1 = fst_create();
    struct _fst * fst_2 = fst_create();
    struct _fst * fst_3 = fst_create();

    fst_compile_str(fst_0, fst_str_0);
    fst_compile_str(fst_1, fst_str_1);
    fst_compile_str(fst_2, fst_str_2);


    fst_arc_sort(fst_0, 1);

    fst_compose(fst_0, fst_1, fst_3);
    
    printf("Half sorted reversed:\n");
    fst_print(fst_3);

    // sort for the normalized comparison
    fst_arc_sort(fst_2, 0);
    fst_arc_sort(fst_3, 0);
    assert_fst_equal( fst_2, fst_3 );

    fst_remove(fst_0);
    fst_remove(fst_1);
    fst_remove(fst_2);
    fst_remove(fst_3);
}

int main() {
    test_compose();
    test_compose_1();
    test_compose_sorted_full_0();
    test_compose_sorted_full_1();
    test_compose_sorted_half_0();
    test_compose_sorted_half_1();
    test_compose_sorted_half_2();
    return 0;
}


