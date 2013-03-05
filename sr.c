#include <float.h>
#include "sr.h"

weight_t real_sum(const weight_t a, const weight_t b) { return a+b; }
weight_t real_product(const weight_t a, const weight_t b) { return a*b; }
weight_t tropical_sum(const weight_t a, const weight_t b) { return a < b ? a : b; }
weight_t tropical_product(const weight_t a, const weight_t b) { return a + b; }

const struct _sr sr_real = {
    .sum    = real_sum,
    .prod   = real_product, 
    .zero   = 0,
    .one    = 1
};

const struct _sr sr_tropical = {
    .sum    = tropical_sum,
    .prod   = tropical_product, 
    .zero   = FLT_MAX,
    .one    = 0
};

/*
It is better to use this definition

struct _sr sr_types[2] = { sr_tropical, sr_real }

*/

struct _sr sr_get(uint8_t sr_type) {
    switch (sr_type) { 
        case SR_REAL:
            return sr_real;
        case SR_TROPICAL:
            return sr_tropical;
        default:
            return sr_tropical;
    }
}

