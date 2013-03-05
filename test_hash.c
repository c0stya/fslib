#include <stdio.h>
#include <assert.h>
#include "hash.h"


static int int_cmp(void * a, void * b) {
    return (*(int *)a == *(int *)b) ? 1:0;
}

static size_t int_hash(void * a) {
    return *(int *)a;
}

static void test_hash() {
    int size = 1003;
    struct _hash * hash = hash_create(
                        int_hash,
                        int_cmp,
                        sizeof(int),
                        sizeof(int),
                        size);
    int obj;
    int key=0;

    assert (hash_get(hash, &key, &obj) == NULL);

    for (int i=0; i < 1500; i++) {
        hash_add(hash, &i, &i);
    }

    for (int i=0; i < 1500; i++) {
        hash_get(hash, &i, &obj);
        assert ( obj == i );
    }
    
    key=1500;
    assert ( hash_get(hash, &key, &obj) == NULL );

    // check for-each macro
    struct _bitem * bitem;
    int i=0;
    _for_each_hashitem(hash, bitem) {
        i++;
    }
    assert ( i == 1500 );

    hash_remove(hash);

    // Another hash table
    struct _hash * hash2 = hash_create(
                        int_hash,
                        int_cmp,
                        sizeof(int),
                        sizeof(int),
                        10);

    int a = 0, b = 1;

    key=1;

    hash_add(hash2, &key, &a);
    hash_add(hash2, &key, &b);
    assert ( hash_get(hash2, &key, &obj) != NULL );
    assert ( obj == key );
    hash_remove(hash2);
}

static void test_hash_resize() {
    struct _hash * hash = hash_create(
                        int_hash,
                        int_cmp,
                        sizeof(int),
                        sizeof(int),
                        1);
    int key=0;
    int item=0;
    
    hash_add(hash, &key, &item);
    assert(hash->size == 2);
    key = 1;
    hash_add(hash, &key, &item);
    assert(hash->size == 4);
    key = 2;
    hash_add(hash, &key, &item);
    assert(hash->size == 4);
    key = 3;
    hash_add(hash, &key, &item);
    assert(hash->size == 8);
}

int main() {
    test_hash();
    test_hash_resize();
    return 0;
}
