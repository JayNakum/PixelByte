#ifndef PIXELBYTE_RENDERER_H
#define PIXELBYTE_RENDERER_H

#include "pch.h"

#include "pixelbuffer.h"

void pxb_draw_point(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const uint32_t size, const pxb_pixel_t color);

void pxb_draw_line(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x0, const uint32_t y0, const uint32_t x1, const uint32_t y1, const pxb_pixel_t color);

void pxb_draw_triangle(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x0, const uint32_t y0, const uint32_t x1, const uint32_t y1, const uint32_t x2, const uint32_t y2, const pxb_pixel_t color);
void pxb_fill_triangle(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x0, const uint32_t y0, const uint32_t x1, const uint32_t y1, const uint32_t x2, const uint32_t y2, const pxb_pixel_t color);

void pxb_draw_circle(pxb_pixelbuffer_t* pixelBuffer, const uint32_t centerX, const uint32_t centerY, const uint32_t radius, const pxb_pixel_t color);
void pxb_fill_circle(pxb_pixelbuffer_t* pixelBuffer, const uint32_t centerX, const uint32_t centerY, const uint32_t radius, const pxb_pixel_t color);

void pxb_draw_glyph(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const uint8_t* glyph, const uint32_t scale, const pxb_pixel_t color);
void pxb_draw_8bitChar(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const char c, const uint32_t scale, const pxb_pixel_t color);
void pxb_draw_text(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const char* text, const uint32_t scale, const pxb_pixel_t color);

#endif // PIXELBYTE_RENDERER_H
