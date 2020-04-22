/*

Yet another version of fst

*/

#ifndef __FST_H__
#define __FST_H__

#include <inttypes.h>
#include <stdio.h>
#include "sr.h"
#include "bitset.h"
#include "queue.h"
#include "symt.h"

#define _for_each_state(fst, sid, state) \
    for(sid = 0, state = fst->states[0]; \
    sid < fst->n_state; \
    ++sid, state=fst->states[sid])


#define state_t             uint32_t
#define arc_t               uint32_t
#define label_t             uint32_t
#define weight_t            float

static const uint32_t _fst_header = 0x66733031;

#define ISORT   0x01
#define OSORT   0x02

#define EPS     0 
#define EPS_L   -1

#define START_STATE "<start>"


struct _fst {
    state_t                 start;
    state_t                 n_states; 
    state_t                 n_max;
    uint8_t                 sr_type;
    uint8_t                 flags;
    struct _state *         states;
};

struct _state {
    arc_t                   n_arcs;
    arc_t                   n_max;
    weight_t                weight;
    int                     final;
    struct _arc *           arcs;
};

struct _arc {
    state_t                 state; 
    weight_t                weight; 
    label_t                 ilabel;
    label_t                 olabel;
};

struct _spair {
    state_t     a;
    state_t     b; 
};

struct _striple {
    state_t     a;
    state_t     b; 
    state_t     c; 
};

struct _apair {
    arc_t       a;
    arc_t       b;
};

struct _arc_pair {
    struct _arc a;
    struct _arc b;
};

struct _match_item {
    struct _arc a;
    struct _arc b;
};

struct _fst *            fst_create();
void                     fst_remove(struct _fst * fst);
void                     fst_empty(struct _fst * fst);
state_t                  fst_add_state(struct _fst * fst);
arc_t                    fst_add_arc(
                                struct _fst *       fst,
                                state_t             src, 
                                state_t             dst,
                                label_t             il,
                                label_t             ol,
                                weight_t            weight
                                );

void                    fst_set_final(struct _fst * fst, state_t s, weight_t w);
void                    fst_print(const struct _fst * fst);

void                    fst_print_sym(const struct _fst * fst,
                                    struct _symt * ist,
                                    struct _symt * ost,
                                    struct _symt * sst);

void                    fst_write(const struct _fst * fst, FILE * fout);
void                    fst_read(struct _fst * fst, FILE * fin);
void                    fst_fwrite(const struct _fst * fst, const char * filename);
void                    fst_fread(struct _fst * fst, const char * filename);

struct _fst *           fst_compile(
                                    struct _fst * fst, 
                                    FILE * fin,
                                    struct _symt * ist,
                                    struct _symt * ost,
                                    struct _symt * sst,
                                    int is_acc);

struct _fst *           fst_compile_str(struct _fst * fst, const char * str);
arc_t                   fst_get_n_arcs(const struct _fst * fst);
void                    fst_arc_sort(struct _fst * fst, int sort_outer);
void                    fst_stack(struct _fst * a, const struct _fst * b);
struct _fst *           fst_union(struct _fst * a, const struct _fst * b);
int                     fst_draw(const struct _fst * fst, FILE * fout);
                        int fst_draw_sym(   const struct _fst * fst, 
                                            FILE * fout,
                                            struct _symt * ist,
                                            struct _symt * ost,
                                            struct _symt * sst);

void                    fst_copy(const struct _fst * orig, struct _fst * copy);
void                    fst_reverse(struct _fst * fst);
struct _fst *           fst_shortest(const struct _fst * fst, struct _fst * path);
struct _fst *           fst_rm_states(struct _fst * fst, struct _bitset * visited);
struct _fst *           fst_trim(struct _fst * fst);

struct _fst *           fst_compose(    const struct _fst * fst_a, 
                                        const struct _fst * fst_b, 
                                        struct _fst * fst_c );

void                    match_unsorted( 
                                        struct _arc * a,
                                        struct _arc * b,
                                        arc_t m,
                                        arc_t n,
                                        struct _queue * q);

void                    match_half_sorted( 
                                        struct _arc * a,
                                        struct _arc * b,
                                        arc_t m,
                                        arc_t n,
                                        struct _queue * q);

void                    match_half_sorted_rev( 
                                        struct _arc * a,
                                        struct _arc * b,
                                        arc_t m,
                                        arc_t n,
                                        struct _queue * q);

void                    match_full_sorted( 
                                        struct _arc * a,
                                        struct _arc * b,
                                        arc_t m,
                                        arc_t n,
                                        struct _queue * q);

void                    fst_relabel(struct _fst * fst, label_t old, label_t new, int dir);

void                    match_arcs(    
                                        const struct _fst * fst_a,
                                        const struct _fst * fst_b,
                                        const struct _spair * pair,
                                        struct _sr * sr,
                                        struct _queue * mq);

#endif

