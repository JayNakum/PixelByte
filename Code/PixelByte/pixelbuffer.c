#include "pixelbuffer.h"

void pxb_pixelbuffer_clear(pxb_pixelbuffer_t* pixelBuffer, const pxb_pixel_t clearValue)
{
    PXB_ASSERT(pixelBuffer);
    PXB_ASSERT(pixelBuffer->pixelBuffer);

    for (uint32_t i = 0; i < pixelBuffer->width * pixelBuffer->height; ++i)
    {
        pixelBuffer->pixelBuffer[i] = clearValue;
    }
}

pxb_pixel_t pxb_pixelbuffer_getPixel(const pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y)
{
    PXB_ASSERT(x < pixelBuffer->width && y < pixelBuffer->height);
    return pixelBuffer->pixelBuffer[y * pixelBuffer->width + x];
}

void pxb_pixelbuffer_writePixel(pxb_pixelbuffer_t* pixelBuffer, const uint32_t x, const uint32_t y, const pxb_pixel_t pixelValue)
{
    PXB_ASSERT(x < pixelBuffer->width && y < pixelBuffer->height);
    pixelBuffer->pixelBuffer[y * pixelBuffer->width + x] = pixelValue;
}

void pxb_pixelbuffer_copy(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(source->pixelBuffer != NULL);

    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(destination->pixelBuffer != NULL);

    PXB_ASSERT(source->width == destination->width);
    PXB_ASSERT(source->height == destination->height);

    const size_t pixelCount = source->width * source->height;
    const size_t byteCount = pixelCount * sizeof(pxb_pixel_t);

    memcpy(destination->pixelBuffer, source->pixelBuffer, byteCount);
}

void pxb_pixelbuffer_crop(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(source->pixelBuffer != NULL);

    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(destination->pixelBuffer != NULL);

    if (x >= source->width || y >= source->height)
    {
        return;
    }

    const uint32_t maxWidth = source->width - x;
    const uint32_t maxHeight = source->height - y;

    const uint32_t cropWidth = (width > maxWidth) ? maxWidth : width;
    const uint32_t cropHeight = (height > maxHeight) ? maxHeight : height;

    PXB_ASSERT(destination->width >= cropWidth && destination->height >= cropHeight);

    for (uint32_t v = 0; v < cropHeight; ++v)
    {
        for (uint32_t u = 0; u < cropWidth; ++u)
        {
            const pxb_pixel_t pixel = pxb_pixelbuffer_getPixel(source, x + u, y + v);

            pxb_pixelbuffer_writePixel(destination, u, v, pixel);
        }
    }
}

void pxb_pixelbuffer_copyScaled(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination, const uint32_t x, const uint32_t y, const uint32_t scale)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(source->pixelBuffer != NULL);
    PXB_ASSERT(destination->pixelBuffer != NULL);

    PXB_ASSERT(x < destination->width && y < destination->height);

    if (scale == 0)
    {
        return;
    }

    const uint32_t scaledWidth = source->width * scale;
    const uint32_t scaledHeight = source->height * scale;

    const uint32_t maxWidth = (x + scaledWidth > destination->width) ? (destination->width - x) : scaledWidth;
    const uint32_t maxHeight = (y + scaledHeight > destination->height) ? (destination->height - y) : scaledHeight;

    for (uint32_t dstV = 0; dstV < maxHeight; ++dstV)
    {
        const uint32_t srcV = dstV / scale;
        const uint32_t v = y + dstV;
        if (v >= destination->height)
        {
            continue;
        }

        for (uint32_t dstU = 0; dstU < maxWidth; ++dstU)
        {
            const uint32_t srcU = dstU / scale;
            const uint32_t u = x + dstU;
            if (u >= destination->width)
            {
                break;
            }
            const pxb_pixel_t pixel = pxb_pixelbuffer_getPixel(source, srcU, srcV);
            pxb_pixelbuffer_writePixel(destination, u, v, pixel);
        }
    }
}

void pxb_pixelbuffer_copyNearest(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(source->pixelBuffer != NULL);
    PXB_ASSERT(destination->pixelBuffer != NULL);

    if (source->width == destination->width && source->height == destination->height)
    {
        pxb_pixelbuffer_copy(source, destination);
        return;
    }

    const uint32_t scaleX = source->width / destination->width;
    const uint32_t scaleY = source->height / destination->height;

    for (uint32_t dstV = 0; dstV < destination->height; ++dstV)
    {
        uint32_t srcV = dstV * scaleY;

        if (srcV >= source->height)
        {
            srcV = source->height - 1;
        }

        for (uint32_t dstU = 0; dstU < destination->width; ++dstU)
        {
            uint32_t srcU = dstU * scaleX;

            if (srcU >= source->width)
            {
                srcU = source->width - 1;
            }

            const pxb_pixel_t pixel = pxb_pixelbuffer_getPixel(source, srcU, srcV);
            pxb_pixelbuffer_writePixel(destination, dstU, dstV, pixel);
        }
    }
}

void pxb_pixelbuffer_copyBilinear(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(source->pixelBuffer != NULL);
    PXB_ASSERT(destination->pixelBuffer != NULL);

    if (source->width == destination->width && source->height == destination->height)
    {
        pxb_pixelbuffer_copy(source, destination);
        return;
    }

    const float scaleX = (float)(source->width - 1) / (float)destination->width;
    const float scaleY = (float)(source->height - 1) / (float)destination->height;

    for (uint32_t dstV = 0; dstV < destination->height; ++dstV)
    {
        float srcV = (float)(dstV) * scaleY;
        const uint32_t srcV0 = (uint32_t)srcV;
        uint32_t srcV1 = srcV0 + 1;

        if (srcV1 >= source->height)
        {
            srcV1 = source->height - 1;
        }

        const float ty = srcV - (float)(srcV0);

        for (uint32_t dstU = 0; dstU < destination->width; ++dstU)
        {
            float srcU = (float)(dstU) * scaleX;
            const uint32_t srcU0 = (uint32_t)srcU;
            uint32_t srcU1 = srcU0 + 1;

            if (srcU1 >= source->width)
            {
                srcU1 = source->width - 1;
            }

            const float tx = srcU - (float)(srcU0);

            const pxb_pixel_t p00 = pxb_pixelbuffer_getPixel(source, srcU0, srcV0);
            const pxb_pixel_t p10 = pxb_pixelbuffer_getPixel(source, srcU1, srcV0);
            const pxb_pixel_t p01 = pxb_pixelbuffer_getPixel(source, srcU0, srcV1);
            const pxb_pixel_t p11 = pxb_pixelbuffer_getPixel(source, srcU1, srcV1);

            const pxb_pixel_t top = pxb_pixel_lerp(p00, p10, tx);
            const pxb_pixel_t bottom = pxb_pixel_lerp(p01, p11, tx);
            const pxb_pixel_t sampledValue = pxb_pixel_lerp(top, bottom, ty);

            pxb_pixelbuffer_writePixel(destination, dstU, dstV, sampledValue);
        }
    }
}

void pxb_pixelbuffer_saveAsPPM(const pxb_pixelbuffer_t* source, const char* filePath)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(filePath != NULL);

    FILE* file = fopen(filePath, "wb");
    PXB_ASSERT(file != NULL);

    const int maxVal = 255;
    fprintf(file, "P6\n%d %d\n%d\n", (int)(source->width), (int)(source->height), maxVal);

    for (uint32_t i = 0; i < source->width * source->height; ++i)
    {
        const pxb_pixel_t* pixel = &source->pixelBuffer[i];

        uint8_t rgb[3];
        rgb[0] = pxb_pixel_clampToByte(pixel->r);
        rgb[1] = pxb_pixel_clampToByte(pixel->g);
        rgb[2] = pxb_pixel_clampToByte(pixel->b);

        fwrite(rgb, sizeof(uint8_t), 3, file);
    }

    fclose(file);

    PXB_LOG(PIXELBYTE, "Output file saved: %s", filePath);
}
