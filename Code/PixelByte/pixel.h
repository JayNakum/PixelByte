#ifndef PIXELBYTE_PIXEL_H
#define PIXELBYTE_PIXEL_H

#include "pch.h"

typedef union
{
    struct { float r, g, b, a; };
    float v[4];

} pxb_pixel_t;

static const pxb_pixel_t PXB_PIXEL_COLOR_TRANSPARENT  = { .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 0.0f };
static const pxb_pixel_t PXB_PIXEL_COLOR_BLACK        = { .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };
static const pxb_pixel_t PXB_PIXEL_COLOR_WHITE        = { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
static const pxb_pixel_t PXB_PIXEL_COLOR_RED          = { .r = 1.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };
static const pxb_pixel_t PXB_PIXEL_COLOR_GREEN        = { .r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f };
static const pxb_pixel_t PXB_PIXEL_COLOR_BLUE         = { .r = 0.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f };
static const pxb_pixel_t PXB_PIXEL_COLOR_YELLOW       = { .r = 1.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f };
static const pxb_pixel_t PXB_PIXEL_COLOR_CYAN         = { .r = 0.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
static const pxb_pixel_t PXB_PIXEL_COLOR_MAGENTA      = { .r = 1.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f };

static inline uint8_t pxb_pixel_clampToByte(const float value)
{
    if (value <= 0.0f) return 0;
    if (value >= 1.0f) return 255;
    return (uint8_t)(value * 255.0f + 0.5f);
}

static inline pxb_pixel_t pxb_pixelRGB(const uint8_t r, const uint8_t g, const uint8_t b)
{
    pxb_pixel_t pixel = { .r = r / 255.0f, .g = g / 255.0f, .b = b / 255.0f, .a = 1.0f };
    return pixel;
}

static inline pxb_pixel_t pxb_pixelV(const uint8_t value)
{
    return pxb_pixelRGB(value, value, value);
}

static inline pxb_pixel_t pxb_pixelRGBA(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    pxb_pixel_t pixel = {.r = r/255.0f, .g = g/255.0f, .b = b/255.0f, .a = a/255.0f };
    return pixel;
}

static inline pxb_pixel_t pxb_pixelPackedRGBA(const uint32_t rgba)
{
    return pxb_pixelRGBA(((rgba >> 24) & 0xFF), ((rgba >> 16) & 0xFF), ((rgba >> 8) & 0xFF), (rgba & 0xFF));
}

static inline pxb_pixel_t pxb_pixel_lerp(const pxb_pixel_t start, const pxb_pixel_t end, const float factor)
{
    pxb_pixel_t result = {
        .r = start.r + (end.r - start.r) * factor,
        .g = start.g + (end.g - start.g) * factor,
        .b = start.b + (end.b - start.b) * factor,
        .a = start.a + (end.a - start.a) * factor
    };
    return result;
}

#endif // PIXELBYTE_PIXEL_H
