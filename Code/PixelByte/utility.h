#ifndef PIXELBYTE_UTILITY_H
#define PIXELBYTE_UTILITY_H

#include "stdint.h"

static inline int pxb_clamp(const int value, const int min, const int max)
{
    return (value < min) ? min : (value > max ? max : value);
}

static inline size_t pxb_alignUp(const size_t v, const size_t align)
{
    return (v + (align - 1)) & ~(align - 1);
}

static inline size_t KB(const size_t kilobytes)
{
    return kilobytes << 10;
}

static inline size_t MB(const size_t megabytes)
{
    return megabytes << 20;
}

static inline size_t GB(const size_t gigabytes)
{
    return gigabytes << 30;
}

static inline size_t TB(const size_t terabytes)
{
    return terabytes << 40;
}

#endif // PIXELBYTE_UTILITY_H
