#include <stdio.h>
#include "bitset.h"

void test_create() {
    struct _bitset * bs = bitset_create(8); 
    bitset_set(bs, 0);
    assert ( bitset_get(bs, 0) == 1 );

    bitset_remove(bs);
}

void test_set_all() {
    struct _bitset * bs = bitset_create(1024); 
    bitset_set_all(bs);

    for (int i = 0; i < 1024; ++i) {
        assert ( bitset_get(bs, i) == 1);
    }

    bitset_remove(bs);
}

void test_clear_all() {
    struct _bitset * bs = bitset_create(1024); 

    bitset_set_all(bs);
    bitset_clear_all(bs);

    for (int i = 0; i < 1024; ++i) {
        assert ( bitset_get(bs, i) == 0);
    }

    bitset_remove(bs);
}

void test_clear() {
    struct _bitset * bs = bitset_create(8); 
    bitset_set(bs, 0);
    assert ( bitset_get(bs, 0) == 1 );
    bitset_clear(bs, 0);
    assert ( bitset_get(bs, 0) == 0 );

    bitset_remove(bs);
}

int main() {
    test_create();
    test_set_all();
    test_clear_all();
    test_clear();

    return 0;
} 
