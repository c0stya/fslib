#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "fst.h"

void test_print() {
    struct _fst * fst = fst_create();
    state_t state_a = fst_add_state(fst);
    state_t state_b = fst_add_state(fst);

    for (int i=0; i < 10; ++i) {
        fst_add_arc(fst, state_a, state_b, i, i, i);
    }

    fst_set_final(fst, state_a, 1);
    fst_set_final(fst, state_b, 1);

    fst_print(fst);
    fst_remove(fst);
}

int main() {
    test_print();
}
