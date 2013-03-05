#include "sr.h"
#include "fst.h"

/* this algorithm is incorrect - please fix */

struct _fst * fst_union(struct _fst * a, const struct _fst * b) {
    struct _sr sr = sr_get(a->sr_type);

    //state_t offset = a->n_states;
    //state_t start = fst_add_state(a);

    fst_stack(a, b);

    fst_add_arc(a, start, a->start + offset, EPS, EPS, sr.one);
    fst_add_arc(a, start, b->start + offset, EPS, EPS, sr.one);

    fst_set_start(a);

    return a;
}
