#ifndef __HASH_H__
#define __HASH_H__

#include <stddef.h>
#include <inttypes.h>

#define HASH_ALPHA 0.75

#define _for_each_hashitem(hash, bitem) \
    for ( size_t __hash_iter = 0; __hash_iter < hash->size; ++__hash_iter ) \
        for ( bitem = hash->buckets[__hash_iter].head; \
                bitem != NULL; \
                bitem = bitem->next ) 

struct _bitem {
    void *              key;
    void *              value;
    struct _bitem *     next;
};

struct _bucket {
    struct _bitem * head;
    struct _bitem * tail;
};

typedef size_t      (*hash_func)(void * key);
typedef int         (*cmp_func)(void * key_a, void * key_b);

struct _hash {
    struct _bucket *        buckets;
    size_t                  item_size;
    size_t                  key_size;
    size_t                  size;
    size_t                  n_items;
    hash_func               hash_f;
    cmp_func                cmp_f;
};


struct _hash *      hash_create(    hash_func hash_f,
                                    cmp_func cmp_f,
                                    size_t item_size, 
                                    size_t key_size, 
                                    size_t size);

void                hash_remove(struct _hash *);
void                hash_resize(struct _hash *);
void                hash_add(struct _hash *, void * key, void * item);
void *              hash_get(struct _hash *, void * key, void * item);
void                empty_chain(struct _bitem *);
void                hash_get_list(struct _hash * hash);
void                hash_delete(struct _hash * hash, void * key, void * item);

size_t              hash_ptr(void * ptr);

#endif
