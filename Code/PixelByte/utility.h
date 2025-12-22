#ifndef PIXELBYTE_UTILITY_H
#define PIXELBYTE_UTILITY_H

#include "stdint.h"

static inline int pxb_util_clamp(const int value, const int min, const int max)
{
    return (value < min) ? min : (value > max ? max : value);
}

#endif // PIXELBYTE_UTILITY_H
