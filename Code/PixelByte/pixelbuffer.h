#ifndef PIXELBYTE_PIXELBUFFER_H
#define PIXELBYTE_PIXELBUFFER_H

#include "pch.h"

#include "pixel.h"

typedef struct
{
    uint32_t width, height;
    pxb_pixel_t *pixelBuffer;

} pxb_pixelbuffer_t;

void pxb_pixelbuffer_clear(pxb_pixelbuffer_t* pixelBuffer, const pxb_pixel_t clearValue);

pxb_pixel_t pxb_pixelbuffer_getPixel(const pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y);
void pxb_pixelbuffer_writePixel(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const pxb_pixel_t pixelValue);

void pxb_pixelbuffer_copy(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination);
void pxb_pixelbuffer_copyScaled(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination, const uint32_t x, const uint32_t y, const uint32_t scale);
void pxb_pixelbuffer_copyNearest(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination);
void pxb_pixelbuffer_copyBilinear(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination);
void pxb_pixelbuffer_crop(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height);

void pxb_pixelbuffer_saveAsPPM(const pxb_pixelbuffer_t* source, const char* filePath);

#endif // PIXELBYTE_PIXELBUFFER_H
