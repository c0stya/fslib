#ifndef __ITER_H__
#define __ITER_H__

struct _fst_iter {
    const struct _fst *     fst;
    struct _bitset *        marked;
    struct _queue *         queue;
    state_t                 state;
};

struct _fst_iter *  fst_iter_create(const struct _fst * fst);
//struct _fst_iter *  fst_miter_create(struct _fst * fst, struct _list * start);
state_t             fst_iter_next(struct _fst_iter * iter);
void                fst_iter_remove(struct _fst_iter * iter);
int                 fst_iter_visited(
                        struct _fst_iter * iter, state_t state);

#endif
