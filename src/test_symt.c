#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "symt.h"

static void test_create() {
    struct _symt*  st = symt_create();
    
    assert( st->sym != NULL );
    for(int i=0; i < st->n_max; ++i) {
        assert( NULL == st->sym[i] );
    }
    
    symt_remove(st);
}

static void test_add() {
    struct _symt *  st = symt_create();

    symt_add(st, 1, "Mary");
    symt_add(st, 2, "had");
    symt_add(st, 3, "a");
    symt_add(st, 4, "little");
    symt_add(st, 5, "lamb");

    symt_print(st);
    
    assert( strcmp(st->sym[1], "Mary") == 0 );
    assert( strcmp(st->sym[2], "had") == 0 );
    assert( strcmp(st->sym[3], "a") == 0 );
    assert( strcmp(st->sym[4], "little") == 0 );
    assert( strcmp(st->sym[5], "lamb") == 0 );

    assert ( st->n_items == 5);
    assert ( st->n_max >= 5);


    symt_remove(st);
}

static void test_add_sparse() {
    struct _symt *  st = symt_create();

    symt_add(st, 100, "token");
    assert( NULL == st->sym[0] );
    assert( strcmp(st->sym[100], "token") == 0 );

    symt_remove(st);
}

static void test_reverse() {
    struct _symt *  st = symt_create();
    size_t i;

    char k0 [] = "000";
    char k1 [] = "111";

    symt_add(st, 0, k0);
    symt_add(st, 1, k1);

    symt_print(st);

    struct _hash * rev = symt_reverse(st);

    assert( hash_get(rev, k0, &i) != NULL );
    assert ( i == 0);
    assert( hash_get(rev, k1, &i) != NULL );
    assert ( i == 1);

    symt_remove(st);
}

static void test_getr() {
    struct _symt *  st = symt_create();

    char k0 [] = "000";
    char k1 [] = "111";
    char k2 [] = "222";

    symt_add(st, 0, k0);
    symt_add(st, 1, k1);

    symt_print(st);
    symt_reverse(st);

    assert( symt_getr(st, k0) == 0 );
    assert( symt_getr(st, k1) == 1 );
    assert( symt_getr(st, k2) == -1 );

    symt_remove(st);
}

static void test_read() {
    struct _symt *  st = symt_create();

    symt_fread(st, "tests/test_symt.txt");
    assert ( st->n_items == 2);

    assert ( strcmp("one", st->sym[1]) == 0 );
    assert ( strcmp("two", st->sym[2]) == 0 );

    symt_remove(st);
}

int main() {
    test_create();
    test_add();
    test_add_sparse();
    test_reverse();
    test_getr();
    test_read();
    return 0;
}


