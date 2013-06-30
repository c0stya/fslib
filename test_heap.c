#include <assert.h>
#include <stdio.h>
#include "heap.h"

static int cmp_lower(const void * a, const void * b) {
   return *(int*)a < *(int*)b;
}

static void test_heap_create() {
    struct _heap * h = heap_create(cmp_lower, sizeof(int), 10, 0);
    assert ( h->n_items == 0);
    assert ( h->n_max == 10);
    assert ( h->cmp == cmp_lower);

    heap_remove(h);
}

static void test_heap_pop() {
    int a[] = {1,0,4,3,2};
    int b;

    struct _heap * h = heap_create(cmp_lower, sizeof(int), 10, 0);

    for (int i =0 ; i < 5; ++i)
        heap_insert(h, &a[i]);

    assert ( *(int*) heap_pop(h, &b) == 0);
    assert ( *(int*) heap_pop(h, &b) == 1);
    assert ( *(int*) heap_pop(h, &b) == 2);
    assert ( *(int*) heap_pop(h, &b) == 3);
    assert ( *(int*) heap_pop(h, &b) == 4);
    assert ( heap_pop(h, &b) == NULL);

    heap_remove(h);
}

static void test_heap_equal() {
    int a[] = {1,1,1,0,0};
    int b;

    struct _heap * h = heap_create(cmp_lower, sizeof(int), 10, 0);

    for (int i =0 ; i < 5; ++i)
        heap_insert(h, &a[i]);

    assert ( *(int*) heap_pop(h, &b) == 0);
    assert ( *(int*) heap_pop(h, &b) == 0);
    assert ( *(int*) heap_pop(h, &b) == 1);
    assert ( *(int*) heap_pop(h, &b) == 1);
    assert ( *(int*) heap_pop(h, &b) == 1);
    assert ( heap_pop(h, &b) == NULL);

    heap_remove(h);
}

static void test_heap_update() {
    int a[] = {3,2,1};
    int b;

    struct _heap * h = heap_create(cmp_lower, sizeof(int), 10, 0);

    heap_insert(h, &a[0]);
    heap_insert(h, &a[1]);
    /* 23 */
    heap_update(h, &a[2], 1);
    /* 12 */ 
    assert ( *(int*) heap_pop(h, &b) == 1);
    assert ( *(int*) heap_pop(h, &b) == 2);
    
    heap_remove(h);
}

static void test_heap_update2() {
    int a[] = {1,1,1};

    struct _heap * h = heap_create(cmp_lower, sizeof(int), 10, 0);

    heap_insert(h, &a[0]);
    heap_insert(h, &a[1]);
    heap_insert(h, &a[2]);

    a[0] = 0;
    heap_update(h, &a[0], 0);

    assert ( *(int*) h->items == 0);
    
    heap_remove(h);
}


static size_t int_hash(void * a) { return *(int*)a; }
static int int_cmp(void * a, void * b) { return *(int*)a == *(int*)b; }

static void test_heap_find() {
    int a[] = {3,2,1};
    size_t i;

    struct _heap * h = heap_create(cmp_lower, sizeof(int), 10, 0);

    heap_index(h, int_hash, int_cmp); 

    heap_insert(h, &a[0]);
    heap_insert(h, &a[1]);
    heap_insert(h, &a[2]);
    /* 1.. */
    assert ( NULL != heap_find(h, &a[2], &i ));
    assert ( i == 0 );
    
    heap_remove(h);
}

static void test_heap_resize() {

    int a[] = {0,1,3};
    struct _heap * h = heap_create(cmp_lower, sizeof(int), 1, 0);

    assert ( h != NULL );

    heap_insert(h, &a[0]);

    assert ( h->n_items == 1 );
    assert ( h->n_max == 1);

    heap_insert(h, &a[0]);

    assert ( h->n_items == 2 );
    assert ( h->n_max == 2);

    heap_insert(h, &a[0]);
    assert ( h->n_items == 3 );
    assert ( h->n_max == 4);

    heap_remove(h);
}

static void test_heap_limit() {
    int a[] = {1,0,4,3,2};
    int b;

    struct _heap * h = heap_create(cmp_lower, sizeof(int), 0, 3);

    for (int i =0 ; i < 5; ++i)
        heap_insert(h, &a[i]);

    //assert( h->limit == 3 );
    //assert( h->n_items == 3 );

    assert ( *(int*) heap_pop(h, &b) == 0);
    assert ( *(int*) heap_pop(h, &b) == 1);
    assert ( *(int*) heap_pop(h, &b) == 2);
    assert ( heap_pop(h, &b) == NULL);

    heap_remove(h);
}

static void test_heap_limit_1() {
    int a[] = {4,3,2,1,0};
    int b;

    struct _heap * h = heap_create(cmp_lower, sizeof(int), 0, 3);

    for (int i =0 ; i < 5; ++i)
        heap_insert(h, &a[i]);

    //assert( h->limit == 3 );
    //assert( h->n_items == 3 );

    assert ( *(int*) heap_pop(h, &b) == 0);
    assert ( *(int*) heap_pop(h, &b) == 1);
    assert ( *(int*) heap_pop(h, &b) == 2);
    assert ( heap_pop(h, &b) == NULL);

    heap_remove(h);
}

int main() {
    test_heap_create();
    test_heap_pop();
    test_heap_equal();
    test_heap_update();
    test_heap_update2();
    test_heap_resize();
    test_heap_find();
    test_heap_limit();
    test_heap_limit_1();
    return 0;
}

