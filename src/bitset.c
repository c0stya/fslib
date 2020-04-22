/*

Adopded from Dale Hagglund answer at stackoverflow.com

*/
#include <stdlib.h>
#include "bitset.h"

enum { WORD_SIZE = sizeof(word_t) * 8 };

static inline int bindex(int b) { return b / WORD_SIZE; }
static inline int boffset(int b) { return b % WORD_SIZE; }

struct _bitset * bitset_create(int nbits) {
    struct _bitset * bitset;

    bitset = malloc(sizeof(struct _bitset));
    bitset->n_words = (nbits / WORD_SIZE + 1);
    bitset->words = malloc(sizeof(*bitset->words) * bitset->n_words);
    bitset_clear_all(bitset);

    return bitset;
}

void bitset_remove(struct _bitset * bitset) {
    free(bitset->words);
    free(bitset);
}

void bitset_set(struct _bitset * bitset, int b) { 
    bitset->words[bindex(b)] |= 1 << (boffset(b)); 
}

void bitset_clear(struct _bitset * bitset, int b) { 
    bitset->words[bindex(b)] &= ~(1 << (boffset(b)));
}

int bitset_get(struct _bitset * bitset, int b) { 
    return bitset->words[bindex(b)] & (1 << (boffset(b))) ? 1 : 0;
}

void bitset_clear_all(struct _bitset * bitset) { 
    for(int i = 0; i < bitset->n_words; ++i)
        bitset->words[i] = 0; 
}
    
void bitset_set_all(struct _bitset * bitset) {
    for(int i = 0; i < bitset->n_words; ++i)
        bitset->words[i] = ~0; 
}

void bitset_union(struct _bitset * a, const struct _bitset * b) {
    assert ( a->n_words == b->n_words ); 

    for(int i = 0; i < a->n_words; ++i)
        a->words[i] |= b->words[i]; 
}

void bitset_intersect(struct _bitset * a, const struct _bitset * b) {
    assert ( a->n_words == b->n_words ); 

    for(int i = 0; i < a->n_words; ++i)
        a->words[i] &= b->words[i]; 
}

struct _bitset * bitset_toggle_all(struct _bitset * bitset) {
    for(int i = 0; i < bitset->n_words; ++i)
        bitset->words[i] ^= ~0; 
    return bitset;
}


