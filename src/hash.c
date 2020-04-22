#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

struct _hash * hash_create( hash_func hash_f,
                            cmp_func cmp_f, 
                            size_t key_size, 
                            size_t item_size, 
                            size_t size) 
{
    struct _hash * hash = (struct _hash *) malloc(sizeof(struct _hash));
    struct _bucket * buckets = (struct _bucket *)malloc(
                size * sizeof(struct _bucket));

    for (size_t i = 0; i < size; i++) {
        buckets[i].head = buckets[i].tail = NULL;
    }

    hash->key_size = key_size;
    hash->item_size = item_size;
    hash->size = size;
    hash->buckets = buckets;
    hash->hash_f = hash_f;
    hash->cmp_f = cmp_f;
    hash->n_items = 0;

    return hash;
}

void hash_resize(struct _hash * hash) {
    size_t old_size = hash->size;
    struct _bucket * old_buckets = hash->buckets;

    hash->size *= 2; 
    hash->n_items = 0;

    hash->buckets = (struct _bucket *)malloc(
                hash->size * sizeof(struct _bucket));

    for (size_t i = 0; i < hash->size; i++)
        hash->buckets[i].head = hash->buckets[i].tail = NULL;

    // rehash all the values 
    for (size_t i = 0; i < old_size; i++) { 
        for (   struct _bitem * bi = old_buckets[i].head; 
                bi != NULL;
                bi = bi->next
        ) {
            hash_add(hash, bi->key, bi->value);
        }
    }

    for (size_t i = 0; i < old_size; ++i)
        empty_chain(old_buckets[i].head); 
    free(old_buckets);
}

void hash_remove(struct _hash * hash) {
    for (size_t i = 0; i < hash->size; ++i) {
        empty_chain(hash->buckets[i].head); 
    }
    free(hash->buckets);
    free(hash);
}

void empty_chain(struct _bitem * bitem) {
    struct _bitem * t_bitem = NULL;
    while(bitem != NULL) {
        t_bitem = bitem;
        bitem = bitem->next;
        if (t_bitem->value) {
            free(t_bitem->key);
            free(t_bitem->value);
        }
        free(t_bitem);
    }
}

void hash_add(  struct _hash * hash, 
                void * key, 
                void * value) 
{
    size_t bnum = hash->hash_f(key) % hash->size;
    struct _bucket * bucket = &hash->buckets[bnum];

    // check if we allready have that key and update it
    for (
                struct _bitem * bi = bucket->head; 
                bi != NULL; 
                bi = bi->next
    ) {
        if ( hash->cmp_f(key, bi->key) ) {
            memcpy(bi->key, key, hash->key_size);
            memcpy(bi->value, value, hash->item_size);
            return;
        }
    }

    // otherwise

    struct _bitem * bitem = malloc(sizeof(struct _bitem));

    bitem->key = key;
    bitem->value = value;
    bitem->next = NULL;         // sentinel

    bitem->key = malloc(hash->key_size);
    memcpy(bitem->key, key, hash->key_size);

    bitem->value = malloc(hash->item_size);
    memcpy(bitem->value, value, hash->item_size);

    if ( bucket->tail == NULL ) {
        bucket->tail = bucket->head = bitem;
    } else {
        bucket->tail->next = bitem;
        bucket->tail = bitem;
    }

    // warning: could be a recusion while resizing
    ++hash->n_items;
    if ( ( ((float)hash->n_items) / hash->size) > HASH_ALPHA) {
        hash_resize(hash);
    }
}

void * hash_get(    struct _hash * hash, 
                    void * key, 
                    void * item) 
{
    struct _bucket bucket = hash->buckets[hash->hash_f(key) % hash->size];

    for (
                struct _bitem * bi = bucket.head; 
                bi != NULL; 
                bi = bi->next
    ) {
        if ( hash->cmp_f(key, bi->key) ) {
            memcpy(item, bi->value, hash->item_size);
            return bi->value;
        }
    }
    return NULL;
}


// remove an element from a hash table
void hash_delete(struct _hash * hash, void * key, void * item) {
    struct _bucket * bucket = &hash->buckets[hash->hash_f(key) % hash->size];
    struct _bitem * bi = bucket->head; 
    struct _bitem * bi_prev = NULL; 
    struct _bitem * bi_next; 

    while(bi != NULL) {
        bi_next = bi->next;
        if ( hash->cmp_f(key, bi->key) ) {
            if (bi_prev == NULL) {
                // seems we are in the head 
                bucket->head = bi_next;
            } else {
                bi_prev->next = bi_next;
            }
            if (bi_next == NULL) {
                // seems we are in the tail
                bucket->tail = bi_prev;
            }

            // free 
            if (bi->value) 
                free(bi->value);
            free(bi->key);
            free(bi);

            --hash->n_items;

            break;
        }

        bi_prev = bi;
        bi = bi_next;
    }
}

