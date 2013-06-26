#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <inttypes.h>
#include <string.h>
#include "heap.h"

/*

The following operators find corresponding 
index of the heap element.

Note: 
-indexes start with 0 as usual
-parent index is -1 when invoked for 0

*/

#define HEAP_PARENT(i)  (((i + 1) >> 1) - 1)
#define HEAP_LEFT(i)    (((i + 1) << 1) - 1)
#define HEAP_RIGHT(i)   ((i + 1) << 1)

static void heap_items_swap(struct _heap * heap, int i, int j) {
    char * items = heap->items;
    size_t s = heap->item_size;
    char * tmp = malloc(s);

    memcpy(tmp, items + i*s, s);
    memcpy(items + i*s, items + j*s, s);
    memcpy(items + j*s, tmp, s);
    
    free(tmp);
}

struct _heap * heap_create(heap_cmp cmp, size_t item_size, size_t init_size) {
    struct _heap * heap;

    heap = malloc(sizeof(struct _heap));
    heap->n_items = 0;
    heap->item_size = item_size;

    heap->cmp = cmp;

    if (init_size == 0) 
        heap->n_max = HEAP_INIT_SIZE;
    else 
        heap->n_max = init_size;

    heap->items = malloc(heap->item_size * heap->n_max); 
    
    return heap;
}

void heap_remove(struct _heap * heap) {
    assert( heap != NULL ); 
    assert( heap->items != NULL );

    free(heap->items);
    free(heap); 
}

/*

Pushing min element from heap.  When invoked the min element is
the first element in array.

To safely remove the element (to keep the heap property) we have to
place the last element to first position and invoke heapify method.

*/
void * heap_pop(struct _heap * heap, void * item) {
    char * items = heap->items;
    size_t s = heap->item_size;

    if (heap->n_items == 0)
        return NULL; 

    // get top node 
    memcpy(item, items, s);
    --heap->n_items;

    // if still not empty - move tail to head
    if (heap->n_items > 0)
        memcpy(items, items + heap->n_items * s, s);

    // heapify if more than 1
    if (heap->n_items > 1)
        heap_heapify(heap, 0);

    return item;
}

/* Methods keep the heap property (see Cormen et al) */

void heap_heapify(struct _heap * heap, int i) {
    char * items = heap->items;
    size_t s = heap->item_size;

    size_t l, r, mx;
    int done = 0;

    assert( i < heap->n_items );

    while (!done) {
        l = HEAP_LEFT(i);
        r = HEAP_RIGHT(i);

        mx = ( l < heap->n_items && heap->cmp(items + l*s, items + i*s)) 
            ?  l : i;  
        mx = ( r < heap->n_items && heap->cmp(items + r*s, items + mx*s)) 
            ? r : mx;
        
        if (mx != i) {
            heap_items_swap(heap, i, mx);
            i = mx;
        } else {
            done=1;
        }
    }
}

void heap_insert(struct _heap * heap, void * item) {
    size_t s = heap->item_size;

    if (heap->n_max == heap->n_items) {
        heap->n_max *= HEAP_RESIZE_FACTOR;
        heap->items = realloc(heap->items, heap->n_max * s);
    }

    memcpy(heap->items + heap->n_items * s, item, s);
    ++heap->n_items;

    /* 

    A little trick here:
    we invoke heap_update with NULL to specify
    that we don't need an assignment and value check.

    */

    heap_update(heap, NULL, heap->n_items-1);
}

/* 

Updating the i-th heap element 
Note: it is possible only to
    -  decrease weight for MIN HEAP
    -  increase weight for MAX HEAP

*/
void heap_update(struct _heap * heap, void * item, int i) { 
    char * items = heap->items;
    size_t s = heap->item_size;
    int j;
    
    assert ( i < heap->n_items );

    if (item != NULL) {  // if invoked directly
        // this assertion check if we reducing the priority and 
        // not increasing it
        assert( heap->cmp(item, items + i*s));
        memcpy(items + i*s, item, s);
    }

    while (i > 0 && heap->cmp(items + i*s, items + HEAP_PARENT(i)*s)) {
        j = HEAP_PARENT(i);
        heap_items_swap(heap, i, j);
        i = j;
    }
}

int heap_find(struct _heap * heap, void * item, heap_key_eq key_eq) {
    for ( size_t i = 0;  i < heap->n_items; ++i )
        if (key_eq(heap->items + i*heap->item_size, item))
            return i;
    return -1; 
}

