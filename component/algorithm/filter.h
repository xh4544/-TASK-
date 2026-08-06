#ifndef FILTER_H
#define FILTER_H

#include "struct_typedef.h"

/**
 * @brief 一阶低通滤波器：out = out * (1-alpha) + input * alpha
 */
typedef struct
{
    fp32 out;       /* 滤波输出 */
    fp32 alpha;     /* 滤波系数，0~1，越小越平滑 */
} first_order_filter_t;

void  first_order_filter_init(first_order_filter_t *f, fp32 alpha);
fp32  first_order_filter_cali(first_order_filter_t *f, fp32 input);

#endif /* FILTER_H */
