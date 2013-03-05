#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "fst.h"

void test_shortest() {
    struct _fst * fst = fst_create();
    struct _fst * path = fst_create();
    state_t state_a = fst_add_state(fst);
    state_t state_b = fst_add_state(fst);
    state_t state_c = fst_add_state(fst);

    fst_add_arc(fst, state_a, state_b, 1, 1, 1.0);
    fst_add_arc(fst, state_a, state_b, 1, 1, 2.0);
    fst_add_arc(fst, state_b, state_c, 2, 2, 1.0);
    fst_add_arc(fst, state_b, state_c, 2, 2, 2.0);

    fst_set_final(fst, state_c, 0.0);

    fst_shortest(fst, path);
    fst_print(path);

    fst_remove(fst);
    fst_remove(path);
}

void test_shortest_c() {
    struct _fst * fst = fst_create();
    struct _fst * path = fst_create();
    state_t a = fst_add_state(fst);
    state_t b = fst_add_state(fst);
    state_t c = fst_add_state(fst);
    state_t d = fst_add_state(fst);

    fst_add_arc(fst, a, b, 1, 1, 1.0);
    fst_add_arc(fst, a, c, 2, 2, 2.0);
    fst_add_arc(fst, b, d, 3, 3, 1.0);
    fst_add_arc(fst, c, d, 4, 4, 2.0);
    fst_add_arc(fst, a, d, 5, 5, 4.0);

    fst_set_final(fst, d, 0.0);

    fst_shortest(fst, path);
    fst_print(path);

    fst_remove(fst);
    fst_remove(path);
}


int main() {
    test_shortest();
    test_shortest_c();
    return 0;
}


