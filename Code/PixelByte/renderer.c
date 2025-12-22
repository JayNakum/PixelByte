#include "renderer.h"

#include "font8x8/font8x8.h"

void pxb_draw_point(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const uint32_t size, const pxb_pixel_t color)
{
    const int halfSize = (int)size / 2;

    const uint32_t scaledMinX = (uint32_t)(pxb_util_clamp((int)(x) - halfSize, (int)0, (int)pixelBuffer->width));
    const uint32_t scaledMaxX = (uint32_t)(pxb_util_clamp((int)(x) + halfSize, (int)0, (int)pixelBuffer->width));
    const uint32_t scaledMinY = (uint32_t)(pxb_util_clamp((int)(y) - halfSize, (int)0, (int)pixelBuffer->height));
    const uint32_t scaledMaxY = (uint32_t)(pxb_util_clamp((int)(y) + halfSize, (int)0, (int)pixelBuffer->height));

    for (uint32_t u = scaledMinX; u < scaledMaxX; ++u)
    {
        for (uint32_t v = scaledMinY; v < scaledMaxY; ++v)
        {
            pxb_pixelbuffer_writePixel(pixelBuffer, u, v, color);
        }
    }
}

void pxb_draw_line(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x0, const uint32_t y0, const uint32_t x1, const uint32_t y1, const pxb_pixel_t color)
{
    PXB_ASSERT(x0 != x1 || y0 != y1);

    // Bresenham's Line Drawing Algorithm

    int u = (int)(x0);
    int v = (int)(y0);
    const int endX = (int)(x1);
    const int endY = (int)(y1);

    // Distances in X and Y.
    const int dx = abs(endX - u);
    const int dy = abs(endY - v);

    // (stepX, stepY) are the step directions.
    const int stepX = (x0 < x1) ? 1 : -1;
    const int stepY = (y0 < y1) ? 1 : -1;

    int error = dx - dy; // error accumulator - indicates how far we are from the ideal line.

    while (u != endX || v != endY) // we always write one pixel at (u, v).
    {
        if (u >= 0 && v >= 0 && u < (int)pixelBuffer->width && v < (int)pixelBuffer->height)
        {
            pxb_pixelbuffer_writePixel(pixelBuffer, (uint32_t)(u), (uint32_t)(v), color);
        }

        int errorScaled = 2 * error; // scaled error to avoid division.

        if (errorScaled > -dy) // if we are above the line, step in X axis.
        {
            error -= dy;
            u += stepX;
        }

        if (errorScaled < dx) // if we are below the line, step in Y axis.
        {
            error += dx;
            v += stepY;
        }
    }
}

void pxb_draw_triangle(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x0, const uint32_t y0, const uint32_t x1, const uint32_t y1, const uint32_t x2, const uint32_t y2, const pxb_pixel_t color)
{
    pxb_draw_line(pixelBuffer, x0, y0, x1, y1, color);
    pxb_draw_line(pixelBuffer, x1, y1, x2, y2, color);
    pxb_draw_line(pixelBuffer, x2, y2, x0, y0, color);
}

void pxb_fill_triangle(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x0, const uint32_t y0, const uint32_t x1, const uint32_t y1, const uint32_t x2, const uint32_t y2, const pxb_pixel_t color)
{
    int xa = (int)x0;
    int ya = (int)y0;
    int xb = (int)x1;
    int yb = (int)y1;
    int xc = (int)x2;
    int yc = (int)y2;

    if (ya > yb)
    {
        int tmpX = xa; xa = xb; xb = tmpX;
        int tmpY = ya; ya = yb; yb = tmpY;
    }
    if (ya > yc)
    {
        int tmpX = xa; xa = xc; xc = tmpX;
        int tmpY = ya; ya = yc; yc = tmpY;
    }
    if (yb > yc)
    {
        int tmpX = xb; xb = xc; xc = tmpX;
        int tmpY = yb; yb = yc; yc = tmpY;
    }

    int totalHeight = yc - ya;
    if (totalHeight <= 0)
    {
        return;
    }

    for (int i = 0; i <= totalHeight; ++i)
    {
        int v = ya + i;
        if (v < 0 || v >= (int)pixelBuffer->height)
        {
            continue;
        }

        int segmentHeight;
        int secondHalf = (i > (yb - ya) || yb == ya) ? 1 : 0;
        if (secondHalf)
        {
            segmentHeight = yc - yb;
        }
        else
        {
            segmentHeight = yb - ya;
        }

        float alpha = (float)i / (float)totalHeight;
        float beta = 0.0f;
        if (segmentHeight > 0)
        {
            beta = (float)(i - (secondHalf ? (yb - ya) : 0)) / (float)segmentHeight;
        }

        int uStart = (int)((float)(xa) +(float)(xc - xa) * alpha);
        int uEnd;
        if (secondHalf)
        {
            uEnd = (int)((float)(xb) +(float)(xc - xb) * beta);
        }
        else
        {
            uEnd = (int)((float)(xa) + (float)(xb - xa) * beta);
        }

        if (uStart > uEnd)
        {
            int tmp = uStart;
            uStart = uEnd;
            uEnd = tmp;
        }

        if (uStart < 0)
        {
            uStart = 0;
        }
        if (uEnd >= (int)pixelBuffer->width)
        {
            uEnd = (int)pixelBuffer->width - 1;
        }

        for (int u = uStart; u <= uEnd; ++u)
        {
            pxb_pixelbuffer_writePixel(pixelBuffer, (uint32_t)u, (uint32_t)v, color);
        }
    }
}

void pxb_draw_circle(pxb_pixelbuffer_t* pixelBuffer, const uint32_t centerX, const uint32_t centerY, const uint32_t radius, const pxb_pixel_t color)
{
    PXB_ASSERT(radius > 0);

    // Bresenham's Circle Drawing Algorithm

    int x = (int)(radius);
    int y = 0;
    int decision = 1 - (int)(radius);

    while (y <= x)
    {
        const int positions[8][2] = {
            { (int)(centerX) + x, (int)(centerY) + y },
            { (int)(centerX) + y, (int)(centerY) + x },
            { (int)(centerX) - y, (int)(centerY) + x },
            { (int)(centerX) - x, (int)(centerY) + y },
            { (int)(centerX) - x, (int)(centerY) - y },
            { (int)(centerX) - y, (int)(centerY) - x },
            { (int)(centerX) + y, (int)(centerY) - x },
            { (int)(centerX) + x, (int)(centerY) - y },
        };

        for (unsigned int i = 0; i < 8; ++i)
        {
            const int u = positions[i][0];
            const int v = positions[i][1];

            if (u >= 0 && v >= 0 && u < (int)pixelBuffer->width && v < (int)pixelBuffer->height)
            {
                pxb_pixelbuffer_writePixel(pixelBuffer, (uint32_t)u, (uint32_t)v, color);
            }
        }

        y++;

        if (decision <= 0)
        {
            decision += 2 * y + 1;
        }
        else
        {
            x--;
            decision += 2 * (y - x) + 1;
        }
    }
}

void pxb_fill_circle(pxb_pixelbuffer_t* pixelBuffer, const uint32_t centerX, const uint32_t centerY, const uint32_t radius, const pxb_pixel_t color)
{
    PXB_ASSERT(radius > 0);

    int cx = (int)centerX;
    int cy = (int)centerY;
    int r = (int)radius;

    for (int y = -r; y <= r; ++y)
    {
        int v = cy + y;
        if (v < 0 || v >= (int)pixelBuffer->height)
        {
            continue;
        }

        float fx = sqrtf((float)(r * r - y * y));
        int dx = (int)fx;

        int startX = cx - dx;
        int endX = cx + dx;

        if (startX < 0)
        {
            startX = 0;
        }
        if (endX >= (int)pixelBuffer->width)
        {
            endX = (int)pixelBuffer->width - 1;
        }

        for (int u = startX; u <= endX; ++u)
        {
            pxb_pixelbuffer_writePixel(pixelBuffer, (uint32_t)u, (uint32_t)v, color);
        }
    }
}

static inline const uint8_t* pxb_getGlyph(const uint32_t code)
{
    if (code < 128)
    {
        return (const uint8_t*)(&font8x8_basic[code][0]);
    }
    else if (code < 256)
    {
        return (const uint8_t*)(&font8x8_ext_latin[code - 128][0]);
    }
    else if (code >= 0x2500 && code <= 0x257F)
    {
        return (const uint8_t*)(&font8x8_box[code - 0x2500][0]);
    }
    else if (code >= 0x0390 && code <= 0x03C9)
    {
        return (const uint8_t*)(&font8x8_greek[code - 0x0390][0]);
    }
    else
    {
        return NULL;
    }
}

void pxb_draw_glyph(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const uint8_t* glyph, const uint32_t scale, const pxb_pixel_t color)
{
    for (uint32_t row = 0; row < 8; row++)
    {
        uint8_t line = glyph[row];

        for (uint32_t col = 0; col < 8; col++)
        {
            if (line & (1u << col))
            {
                for (uint32_t sy = 0; sy < scale; sy++)
                {
                    uint32_t v = y + row * scale + sy;
                    if (v >= pixelBuffer->height)
                    {
                        continue;
                    }

                    for (uint32_t sx = 0; sx < scale; sx++)
                    {
                        uint32_t u = x + col * scale + sx;
                        if (u >= pixelBuffer->width)
                        {
                            continue;
                        }

                        pxb_pixelbuffer_writePixel(pixelBuffer, u, v, color);
                    }
                }
            }
        }
    }
}

void pxb_draw_8bitChar(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const char c, const uint32_t scale, const pxb_pixel_t color)
{
    const uint8_t* glyph = pxb_getGlyph((uint8_t)(c));
    if (!glyph)
    {
        return;
    }

    pxb_draw_glyph(pixelBuffer, x, y, glyph, scale, color);
}

void pxb_draw_text(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const char* text, const uint32_t scale, const pxb_pixel_t color)
{
    uint32_t cursorX = x;
    uint32_t cursorY = y;

    uint32_t charWidth = 8 * scale;
    uint32_t charHeight = 8 * scale;

    while (*text)
    {
        char c = *text;

        if (c == '\n')
        {
            cursorX = x;
            cursorY += charHeight;
        }
        else if (c == '\t')
        {
            cursorX += charWidth * 4;
        }
        else
        {
            pxb_draw_8bitChar(pixelBuffer, cursorX, cursorY, c, scale, color);
            cursorX += charWidth;
        }

        text++;
    }
}
