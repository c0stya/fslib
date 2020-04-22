#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "hash.h"

static size_t int_hash(void * a) {
    return *(int *)a;
}

static int int_cmp(void * a, void * b) {
    return (*(int *)a == *(int *)b) ? 1:0;
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

    hash_remove(hash);
}

static void test_load_int() {

    struct _hash * hash = hash_create(
                        int_hash,
                        int_cmp,
                        sizeof(int),
                        sizeof(int),
                        10000);
    int i;
 
    for (i=0; i < 100000; i++) {
        hash_add(hash, &i, &i);
    }
    
    int obj, j;
    for (i=0; i < 100000; i++) {
        j = ~i;
        assert ( hash_get(hash, &i, &obj) != NULL );
        assert ( obj == i );
        assert ( hash_get(hash, &j, &obj) == NULL );
    }

    hash_remove(hash);
}

static uint32_t FNV_PRIME_32 = 16777619;
static uint32_t FNV_OFFSET_32 = 2166136261U;

static size_t fnv32(void * str) {

    const char * s = str;
    uint32_t hsh = FNV_OFFSET_32, i;

    for(i = 0; i < strlen(s); i++) {
        // xor next byte into the bottom of the hash
        hsh = hsh ^ (s[i]); 
        // Multiply by prime number found to work well
        hsh = hsh * FNV_PRIME_32; 
    }
    return hsh;
}

static int str_cmp(void * a, void * b) {
    return strcmp((const char *)a, (const char *)b) == 0 ? 1: 0;
}

static void test_load_str() {
    char token[64];

    struct _hash * hash = hash_create(
                        fnv32,
                        str_cmp,
                        sizeof(token),
                        sizeof(int),
                        10000);
    int i, obj;
 
    for (i=0; i < 100000; i++) {
        sprintf(token, "%d", i);
        hash_add(hash, token, &i);
    }
    
    for (i=0; i < 100000; i++) {
        sprintf(token, "%d", i);

        assert ( hash_get(hash, token, &obj) != NULL );
        assert ( obj == i );

        sprintf(token, "%d", ~i);
        assert ( hash_get(hash, token, &obj) == NULL );
    }

    hash_remove(hash);
}

static void test_hash_update() {
    struct _hash * h = hash_create(
                        int_hash,
                        int_cmp,
                        sizeof(int),
                        sizeof(int),
                        1);
    int key=0;
    int item;
    int item0=0;
    int item1=1;
    
    hash_add(h, &key, &item0);
    assert( hash_get(h, &key, &item) != NULL );
    assert( item == 0 );
    hash_add(h, &key, &item1);
    assert( hash_get(h, &key, &item) != NULL );
    assert( item == 1);

    hash_remove(h);
}

static void test_hash_delete() {
    struct _hash * h = hash_create(
                        int_hash,
                        int_cmp,
                        sizeof(int),
                        sizeof(int),
                        1);
    int key=0;
    int item;
    int item0=0;
    
    hash_add(h, &key, &item0);
    assert( hash_get(h, &key, &item) != NULL );
    assert( item == 0 );
    hash_delete(h, &key, &item0);
    assert( hash_get(h, &key, &item) == NULL );

    assert ( h->n_items == 0 );

    hash_remove(h);
}

static void test_hash_delete_1() {
    char token[64];

    struct _hash * hash = hash_create(
                        fnv32,
                        str_cmp,
                        sizeof(token),
                        sizeof(int),
                        100);
    int i;

    for (i=0; i < 100; i++) {
        sprintf(token, "%d", i);
        hash_add(hash, token, &i);
    }

    assert ( hash->n_items == 100 );

    for (i=0; i < 100; i++) {
        sprintf(token, "%d", i);
        hash_delete(hash, token, &i);
    }

    assert ( hash->n_items == 0 );

    for (i=0; i < 100; i++) {
        sprintf(token, "%d", i);
        assert ( hash_get(hash, token, &i) == NULL);
    }
   
    hash_remove(hash);
}

int main() {
    test_hash();
    test_hash_resize();
    test_load_int();
    test_load_str();
    test_hash_update();
    test_hash_delete();
    test_hash_delete_1();
    return 0;
}

