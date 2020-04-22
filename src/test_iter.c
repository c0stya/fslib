#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "fst.h"
#include "iter.h"

void test_iter_create() {
    struct _fst * fst = fst_create();
    state_t state_a = fst_add_state(fst);
    state_t state_b = fst_add_state(fst);
    state_t state_c = fst_add_state(fst);

    fst_add_arc(fst, state_a, state_b, 1, 1, 1);
    fst_add_arc(fst, state_b, state_c, 1, 1, 1);

    struct _fst_iter * iter = fst_iter_create(fst);

    assert ( state_a == fst_iter_next(iter));
    assert ( state_a == iter->state );
    assert ( state_b == fst_iter_next(iter));
    assert ( state_b == iter->state );
    assert ( state_c == fst_iter_next(iter));
    assert ( state_c == iter->state );
    assert ( -1 == fst_iter_next(iter));

    fst_remove(fst);
    fst_iter_remove(iter);
}

void test_iter_cyclic() {
    struct _fst * fst = fst_create();
    state_t state_a = fst_add_state(fst);
    state_t state_b = fst_add_state(fst);
    state_t state_c = fst_add_state(fst);

    fst_add_arc(fst, state_a, state_b, 1, 1, 1);
    fst_add_arc(fst, state_a, state_c, 1, 1, 1);
    fst_add_arc(fst, state_c, state_a, 1, 1, 1);

    struct _fst_iter * iter = fst_iter_create(fst);

    assert ( state_a == fst_iter_next(iter));
    assert ( state_a == iter->state );
    assert ( state_b == fst_iter_next(iter));
    assert ( state_b == iter->state );
    assert ( state_c == fst_iter_next(iter));
    assert ( state_c == iter->state );
    assert ( -1 == fst_iter_next(iter));

    fst_remove(fst);
    fst_iter_remove(iter);
}

void test_iter_nonaccessible() {
    struct _fst * fst = fst_create();
    state_t state_a = fst_add_state(fst);
    state_t state_b = fst_add_state(fst);

    fst_add_arc(fst, state_b, state_a, 1, 1, 1);

    struct _fst_iter * iter = fst_iter_create(fst);

    assert ( state_a == fst_iter_next(iter));
    assert ( state_a == iter->state );
    assert ( -1 == fst_iter_next(iter));

    fst_remove(fst);
    fst_iter_remove(iter);
}

void test_iter_visited() {
    struct _fst * fst = fst_create();
    state_t state_a = fst_add_state(fst);
    state_t state_b = fst_add_state(fst);

    fst_add_arc(fst, state_b, state_a, 1, 1, 1);

    struct _fst_iter * iter = fst_iter_create(fst);
    while ( fst_iter_next(iter) != -1 ) 
        {}
    
    assert ( 1 == fst_iter_visited(iter, state_a) );
    assert ( 0 == fst_iter_visited(iter, state_b) );
    
    fst_remove(fst);
    fst_iter_remove(iter);
}

int main() {
    test_iter_create();
    test_iter_cyclic();
    test_iter_visited();
    test_iter_nonaccessible();
    return 0;
}
