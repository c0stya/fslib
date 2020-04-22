#ifndef __BITSET_H__
#define __BITSET_H__

#include <assert.h>
#include <inttypes.h>

typedef uint32_t word_t;

struct _bitset { 
    word_t *words; 
    int n_words; 
};

struct _bitset *    bitset_create(int nbits);
void                bitset_remove(struct _bitset * bitset);
void                bitset_set(struct _bitset * bitset, int b);
void                bitset_clear(struct _bitset * bitset, int b);
int                 bitset_get(struct _bitset * bitset, int b);
void                bitset_clear_all(struct _bitset * bitset);
void                bitset_set_all();
void                bitset_union(struct _bitset * a, const struct _bitset * b);
void                bitset_intersect(struct _bitset * a, const struct _bitset * b);
struct _bitset *    bitset_toggle_all(struct _bitset * bitset);

#endif
