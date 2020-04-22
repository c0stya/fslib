#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <inttypes.h>
#include <string.h>
#include "heap.h"
#include "hash.h"

/*

The following operators find corresponding 
index of the heap element.

Note: 
-indexes start with 0 as usual
-parent index is -1 when invoked for 0

TODO:
    refactor the ugly index support with tag // index

*/

#define HEAP_PARENT(i)  (((i + 1) >> 1) - 1)
#define HEAP_LEFT(i)    (((i + 1) << 1) - 1)
#define HEAP_RIGHT(i)   ((i + 1) << 1)

static void heap_items_swap(struct _heap * heap, size_t i, size_t j) {
    char * items = heap->items;
    size_t s = heap->item_size;
    char * tmp = malloc(s);

    memcpy(tmp, items + i*s, s);
    memcpy(items + i*s, items + j*s, s);
    memcpy(items + j*s, tmp, s);

    // index
    if (heap->ht != NULL) {
        hash_add(heap->ht, items + i*s, &i);
        hash_add(heap->ht, items + j*s, &j);
    }
    
    free(tmp);
}

struct _heap * heap_create(         heap_cmp cmp, 
                                    size_t item_size, 
                                    size_t init_size,
                                    size_t limit) {
    struct _heap * heap;

    heap = malloc(sizeof(struct _heap));
    heap->n_items = 0;
    heap->item_size = item_size;
    heap->limit = limit;

    heap->cmp = cmp;

    if (limit != 0) {
        heap->n_max = limit + 1; 
    } else {
        if (init_size == 0) 
            heap->n_max = HEAP_INIT_SIZE;
        else 
            heap->n_max = init_size;    
    }

    heap->items = malloc(heap->item_size * heap->n_max); 

    // create lookup hash table to track indexes
    heap->ht = NULL;
    
    return heap;
}

struct _hash * heap_index(struct _heap * heap, hash_func hsh, cmp_func hcmp) {
    if (heap->ht != NULL)
        hash_remove(heap->ht);
    
    heap->ht = hash_create( hsh, 
                            hcmp, 
                            heap->item_size,    // key size
                            sizeof(size_t),     // item size
                            heap->n_max         // init size
                        ); 
    
    // lets index all the items 
    for (size_t i = 0;  i < heap->n_items; ++i)
        hash_add(heap->ht, heap->items + i*heap->item_size, &i);
    
    return heap->ht;
}  

void heap_remove(struct _heap * heap) {
    assert( heap != NULL ); 
    assert( heap->items != NULL );

    if (heap->ht != NULL) {
        hash_remove(heap->ht);
    }

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
    if (heap->n_items > 0) {
        memcpy(items, items + heap->n_items * s, s);
        // index
        size_t i=0;
        if (heap->ht != NULL)  
            hash_add(heap->ht, items, &i);
    }

    // heapify if more than 1
    if (heap->n_items > 1)
        heap_heapify(heap, 0);

    return item;
}

/* Methods keep the heap property (see Cormen et al) */

void heap_heapify(struct _heap * heap, size_t i) {
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

static void heap_delete_max(struct _heap * heap) {
    // using linear search to find max element
    char * items = heap->items; 
    size_t s = heap->item_size;

    size_t m = 0; 
    for (size_t i = 1; i < heap->n_items + 1; ++i) {
        if ( heap->cmp(items +m*s, items + i*s))  m = i;
    }

    // replace with the last element 
    if (m != heap->n_items) {  // is already the last, do nothing
        memcpy(items + m*s, items + heap->n_items*s, s); 

        // index 
        if (heap->ht != NULL) 
            hash_add(heap->ht, items + m*s, &m);

        heap_update(heap, NULL, m);
    }
}

void heap_insert(struct _heap * heap, void * item) {
    size_t s = heap->item_size;

    if (heap->limit == 0 && heap->n_max == heap->n_items) {
        heap->n_max *= HEAP_RESIZE_FACTOR;
        heap->items = realloc(heap->items, heap->n_max * s);
    }

    // move to tail
    memcpy(heap->items + heap->n_items * s, item, s);

    // index
    if (heap->ht != NULL) 
        hash_add(heap->ht, item, &heap->n_items);

    /* 

    A little trick here:
    we invoke heap_update with NULL to specify
    that we don't need an assignment and value check.

    */

    if (heap->limit == 0 || heap->n_items < heap->limit) {
        heap_update(heap, NULL, heap->n_items++);
    } else {
        heap_delete_max(heap);
    }
}

/* 

Updating the i-th heap element 
Note: it is possible only to
    -  decrease weight for MIN HEAP
    -  increase weight for MAX HEAP

*/

void heap_update(struct _heap * heap, void * item, size_t i) { 
    char * items = heap->items;
    size_t s = heap->item_size;
    size_t j;
    
    // remove assertion for equality
    assert ( i < heap->n_items );

    if (item != NULL) {  // if invoked directly
        // check if we reducing the priority and 
        // not increasing it
        assert( heap->cmp(item, items + i*s) );
        memcpy(items + i*s, item, s);

        // index
        if (heap->ht != NULL) 
            hash_add(heap->ht, item, &i);
    }

    while (i > 0 && heap->cmp(items + i*s, items + HEAP_PARENT(i)*s)) {
        j = HEAP_PARENT(i);
        heap_items_swap(heap, i, j);

        i = j;
    }
}

// hash
void * heap_find(struct _heap * heap, void * item, size_t * pi) {
    assert( heap->ht != NULL );

    return  hash_get(heap->ht, item, pi);
}


