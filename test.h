#ifndef __TEST_H__
#define __TEST_H__
#include "fst.h"

void assert_arc_equal(const struct _arc * arc_a, const struct _arc * arc_b);
void assert_state_equal(const struct _state * state_a, const struct _state * state_b);
void assert_fst_equal(const struct _fst * a, const struct _fst * b);

#endif
