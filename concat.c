#include "queue.h"
#include "fst.h"

struct _fst * fst_concat(struct _fst * a, const struct _fst * b) {
    s = fst_add_state(c);

    struct _sr sr = get_sr(a);

    state_t offset = a->n_states;
    fst_stack(a, b);

    state_t s;

    while(queue_deque(finals, &s) != NULL) {
        fst_add_arc(a, s, b->start + offset, EPS, EPS, sr.one);
    }

    return a;
}
