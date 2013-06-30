#ifndef __HEAP_H__
#define __HEAP_H__

#define HEAP_INIT_SIZE      0xff
#define HEAP_RESIZE_FACTOR  2

#include "hash.h"

/*

Straightforward implementation of binary heap based on
Cormen at al algorithm. 

Heap is
- array based
- unlimited
- pointer container

Comparator 'heap_cmp' is a function that can be 
- strict upper for MAX HEAP
- strict lower for MIN HEAP

Example:

int cmp_lower(const void * a, const void * b) {
   return *(int*)a < *(int*)b;
}
*/

typedef int (*heap_cmp) (const void * a, const void * b);
typedef int (*heap_key_eq) (const void * a, const void * b);

struct _heap {
    size_t              n_items;
    size_t              n_max;
    size_t              item_size;
    size_t              limit;
    heap_cmp            cmp;
    struct _hash *      ht;
    char *              items;  // TODO: generalize it!
};

struct _heap * heap_create(
                            heap_cmp cmp, 
                            size_t item_size, 
                            size_t init_size,
                            size_t limit);

struct _hash * heap_index(struct _heap * heap, hash_func hsh, cmp_func hcmp);
void heap_remove(struct _heap * heap);
void * heap_pop(struct _heap * heap, void * item);
void heap_heapify(struct _heap * heap, size_t i);
void heap_insert(struct _heap * heap, void * item);
void heap_update(struct _heap * heap, void * item, size_t i);

void * heap_find(struct _heap * heap, void * item, size_t * pi);


#endif
