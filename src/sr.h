#ifndef __SR_H__
#define __SR_H__

#include "inttypes.h"

#define weight_t        float

weight_t            real_sum(const weight_t a, const weight_t b);
weight_t            real_product(const weight_t a, const weight_t b);
weight_t            tropical_sum(const weight_t a, const weight_t b);
weight_t            tropical_product(const weight_t a, const weight_t b);

typedef weight_t (*sr_sum)(const weight_t a, const weight_t b);
typedef weight_t (*sr_prod)(const weight_t a, const weight_t b);

struct _sr {
    sr_sum      sum;
    sr_prod     prod;
    weight_t    zero;
    weight_t    one;
};

struct _sr sr_get(uint8_t sr_type);

extern struct _sr sr_types [];

#define SR_TROPICAL     0
#define SR_REAL         1


#endif
