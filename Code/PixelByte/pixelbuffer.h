#ifndef PIXELBYTE_PIXELBUFFER_H
#define PIXELBYTE_PIXELBUFFER_H

#include "pch.h"

#include "pixel.h"

typedef struct PixelBuffer
{
    uint32_t width, height;
    pxb_pixel_t* pixels;

} pxb_pixelbuffer_t;

typedef pxb_pixelbuffer_t pxb_texture_t;

static inline pxb_pixel_t pxb_pixelbuffer_getPixel(const pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y)
{
    PXB_ASSERT(x < pixelBuffer->width && y < pixelBuffer->height);
    return pixelBuffer->pixels[y * pixelBuffer->width + x];
}

static inline void pxb_pixelbuffer_writePixel(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const pxb_pixel_t pixelValue)
{
    PXB_ASSERT(x < pixelBuffer->width && y < pixelBuffer->height);
    pixelBuffer->pixels[y * pixelBuffer->width + x] = pixelValue;
}

static inline pxb_pixel_t pxb_sampleNearest(const pxb_pixelbuffer_t* pixelBuffer, const float u, const float v)
{
    const int x = (int)(u + 0.5f);
    const int y = (int)(v + 0.5f);

    if (x < 0 || y < 0 || x >= (int)(pixelBuffer->width) || y >= (int)(pixelBuffer->height))
    {
        return PXB_PIXEL_COLOR_TRANSPARENT;
    }

    return pxb_pixelbuffer_getPixel(pixelBuffer, (uint32_t)(x), (uint32_t)(y));
}

static inline pxb_pixel_t pxb_sampleBilinear(const pxb_pixelbuffer_t* pixelBuffer, const float u, const float v)
{
    const int maxX = (int)(pixelBuffer->width - 1);
    const int maxY = (int)(pixelBuffer->height - 1);

    const float fu = u;
    const float fv = v;

    const int x0 = pxb_clamp((int)(fu), 0, maxX);
    const int y0 = pxb_clamp((int)(fv), 0, maxY);

    const int x1 = pxb_clamp(x0 + 1, 0, maxX);
    const int y1 = pxb_clamp(y0 + 1, 0, maxY);

    const float tx = fu - (float)(x0);
    const float ty = fv - (float)(y0);

    const pxb_pixel_t p00 = pxb_pixelbuffer_getPixel(pixelBuffer, (uint32_t)(x0), (uint32_t)(y0));
    const pxb_pixel_t p10 = pxb_pixelbuffer_getPixel(pixelBuffer, (uint32_t)(x1), (uint32_t)(y0));
    const pxb_pixel_t p01 = pxb_pixelbuffer_getPixel(pixelBuffer, (uint32_t)(x0), (uint32_t)(y1));
    const pxb_pixel_t p11 = pxb_pixelbuffer_getPixel(pixelBuffer, (uint32_t)(x1), (uint32_t)(y1));

    const pxb_pixel_t top = pxb_pixel_lerp(p00, p10, tx);
    const pxb_pixel_t bottom = pxb_pixel_lerp(p01, p11, tx);

    return pxb_pixel_lerp(top, bottom, ty);
}

void pxb_pixelbuffer_clear(pxb_pixelbuffer_t* pixelBuffer, const pxb_pixel_t clearValue);

void pxb_pixelbuffer_copy(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination);
void pxb_pixelbuffer_copyScaled(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination, const uint32_t x, const uint32_t y, const uint32_t scale);
void pxb_pixelbuffer_copyTiled(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination, const uint32_t offsetX, const uint32_t offsetY);
void pxb_pixelbuffer_copyNearest(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination);
void pxb_pixelbuffer_copyBilinear(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination);
void pxb_pixelbuffer_crop(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height);

void pxb_pixelbuffer_saveAsPPM(const pxb_pixelbuffer_t* source, const char* filePath);

#endif // PIXELBYTE_PIXELBUFFER_H
