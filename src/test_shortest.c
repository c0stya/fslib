#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "fst.h"
#include "test.h"

void test_shortest() {
    // Straightforward test
    char * fst_str = "0 1 1 1 1.0\n0 1 2 2 2.0\n1 2 3 3 1.0\n1 2 4 4 2.0\n2";
    char * fst_str_path = "0 1 1 1 1.0\n1 2 3 3 1.0\n2";

    struct _fst * fst = fst_create();
    struct _fst * path = fst_create();
    struct _fst * _path = fst_create();

    fst_compile_str(fst, fst_str);
    fst_compile_str(_path, fst_str_path);

    fst_shortest(fst, path);

    assert_fst_equal( _path, path );

    fst_remove(fst);
    fst_remove(path);
    fst_remove(_path);
}

void test_shortest_three() {
    // three-paths test
    char * fst_str = "0 1 1 1 1.0\n0 2 2 2 2.0\n1 3 3 3 1.0\n2 3 4 4 2.0\n0 3 5 5 4.0\n3";
    char * fst_str_path = "0 1 1 1 1.0\n1 2 3 3 1.0\n2";
    
    struct _fst * fst = fst_create();
    struct _fst * path = fst_create();
    struct _fst * _path = fst_create();

    fst_compile_str(fst, fst_str);
    fst_compile_str(_path, fst_str_path);

    fst_shortest(fst, path);

    fst_print(_path);
    fst_print(path);

    assert_fst_equal( _path, path );

    fst_remove(fst);
    fst_remove(path);
    fst_remove(_path);
}

void test_shortest_cyclic() {
    // cyclic graph test
    char * fst_str = "0 1 1 1 1.0\n1 2 2 2 1.0\n1 0 3 3 1.0\n2";
    char * fst_str_path = "0 1 1 1 1.0\n1 2 2 2 1.0\n2";
    
    struct _fst * fst = fst_create();
    struct _fst * path = fst_create();
    struct _fst * _path = fst_create();

    fst_compile_str(fst, fst_str);
    fst_compile_str(_path, fst_str_path);

    fst_shortest(fst, path);

    assert_fst_equal( _path, path );

    fst_remove(fst);
    fst_remove(path);
    fst_remove(_path);
}

void test_shortest_reflexive() {
    // test for a graph with reflexive node
    char * fst_str = "0 1 1 1 1.0\n1 2 2 2 1.0\n1 1 3 3 1.0\n2";
    char * fst_str_path = "0 1 1 1 1.0\n1 2 2 2 1.0\n2";
    
    struct _fst * fst = fst_create();
    struct _fst * path = fst_create();
    struct _fst * _path = fst_create();

    fst_compile_str(fst, fst_str);
    fst_compile_str(_path, fst_str_path);

    fst_shortest(fst, path);

    assert_fst_equal( _path, path );

    fst_remove(fst);
    fst_remove(path);
    fst_remove(_path);
}

int main() {
    test_shortest();
    test_shortest_three();
    test_shortest_cyclic();
    test_shortest_reflexive();

    return 0;
}


