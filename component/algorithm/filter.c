#include "filter.h"

void first_order_filter_init(first_order_filter_t *f, fp32 alpha)
{
    f->out   = 0.0f;
    f->alpha = alpha;
}

fp32 first_order_filter_cali(first_order_filter_t *f, fp32 input)
{
    f->out = f->out * (1.0f - f->alpha) + input * f->alpha;
    return f->out;
}
