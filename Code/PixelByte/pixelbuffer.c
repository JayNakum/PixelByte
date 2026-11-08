#include "pixelbuffer.h"

void pxb_pixelbuffer_clear(pxb_pixelbuffer_t* pixelBuffer, const pxb_pixel_t clearValue)
{
    PXB_ASSERT(pixelBuffer);
    PXB_ASSERT(pixelBuffer->pixels);

/*
    const uint32_t totalPixels = pixelBuffer->width * pixelBuffer->height;

    if (clearValue.r == clearValue.g && clearValue.g == clearValue.b && clearValue.b == clearValue.a)
    {
        const uint8_t byteValue = pxb_pixel_clampToByte(clearValue.r);
        memset(pixelBuffer->pixels, byteValue, totalPixels * sizeof(pxb_pixel_t));
        return;
    }
*/

    pxb_pixel_t* pixels = pixelBuffer->pixels;
    const uint32_t scanlinePixels = pixelBuffer->width;

    for (uint32_t i = 0; i < scanlinePixels; ++i)
    {
        pixels[i] = clearValue;
    }

    const size_t scanlineBytes = scanlinePixels * sizeof(pxb_pixel_t);
    for (uint32_t y = 1; y < pixelBuffer->height; ++y)
    {
        memcpy(&pixels[y * scanlinePixels], pixels, scanlineBytes);
    }
}

void pxb_pixelbuffer_copy(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(source->pixels != NULL);

    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(destination->pixels != NULL);

    PXB_ASSERT(source->width == destination->width);
    PXB_ASSERT(source->height == destination->height);

    const size_t pixelCount = source->width * source->height;
    const size_t byteCount = pixelCount * sizeof(pxb_pixel_t);

    memcpy(destination->pixels, source->pixels, byteCount);
}

void pxb_pixelbuffer_copyScaled(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination, const uint32_t x, const uint32_t y, const uint32_t scale)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(source->pixels != NULL);
    PXB_ASSERT(destination->pixels != NULL);

    PXB_ASSERT(x < destination->width && y < destination->height);

    if (scale == 0)
    {
        return;
    }

    const uint32_t scaledWidth = source->width * scale;
    const uint32_t scaledHeight = source->height * scale;

    const uint32_t maxWidth = (x + scaledWidth > destination->width) ? (destination->width - x) : scaledWidth;
    const uint32_t maxHeight = (y + scaledHeight > destination->height) ? (destination->height - y) : scaledHeight;

    const uint32_t srcWidth = source->width;
    const uint32_t dstWidth = destination->width;

    const pxb_pixel_t* srcPixels = source->pixels;
    pxb_pixel_t* dstPixels = destination->pixels;

    for (uint32_t dstV = 0; dstV < maxHeight; ++dstV)
    {
        const uint32_t srcV = dstV / scale;
        const uint32_t v = y + dstV;
        const pxb_pixel_t* srcRow = &srcPixels[srcV * srcWidth];
        pxb_pixel_t* dstRow = &dstPixels[v * dstWidth + x];

        for (uint32_t dstU = 0; dstU < maxWidth; ++dstU)
        {
            const uint32_t srcU = dstU / scale;
            dstRow[dstU] = srcRow[srcU];
        }
    }
}

void pxb_pixelbuffer_copyTiled(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination, const uint32_t offsetX, const uint32_t offsetY)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(source->pixels != NULL);
    PXB_ASSERT(destination->pixels != NULL);
    PXB_ASSERT(source->width > 0 && source->height > 0);

    const uint32_t srcWidth = source->width;
    const uint32_t srcHeight = source->height;
    const uint32_t dstWidth = destination->width;
    const uint32_t dstHeight = destination->height;

    uint32_t startX = ((offsetX % srcWidth) + srcWidth) % srcWidth;
    uint32_t startY = ((offsetY % srcHeight) + srcHeight) % srcHeight;

    for (uint32_t dstY = 0; dstY < dstHeight; ++dstY)
    {
        const uint32_t srcY = (startY + dstY) % srcHeight;
        pxb_pixel_t* dstRow = &destination->pixels[dstY * dstWidth];

        uint32_t dstX = 0;
        while (dstX < dstWidth)
        {
            const uint32_t srcX = (startX + dstX) % srcWidth;
            uint32_t copyWidth = srcWidth - srcX;

            if (dstX + copyWidth > dstWidth)
            {
                copyWidth = dstWidth - dstX;
            }

            const pxb_pixel_t* srcRow = &source->pixels[srcY * srcWidth + srcX];
            memcpy(&dstRow[dstX], srcRow, copyWidth * sizeof(pxb_pixel_t));

            dstX += copyWidth;
        }
    }
}

void pxb_pixelbuffer_copyNearest(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(source->pixels != NULL);
    PXB_ASSERT(destination->pixels != NULL);

    if (source->width == destination->width && source->height == destination->height)
    {
        pxb_pixelbuffer_copy(source, destination);
        return;
    }

    const float scaleX = (float)(source->width) / (float)(destination->width);
    const float scaleY = (float)(source->height) / (float)(destination->height);

    const uint32_t srcWidth = source->width;
    const uint32_t srcHeight = source->height;
    const uint32_t dstWidth = destination->width;
    const uint32_t dstHeight = destination->height;

    const pxb_pixel_t* srcPixels = source->pixels;
    pxb_pixel_t* dstPixels = destination->pixels;

    for (uint32_t dstV = 0; dstV < dstHeight; ++dstV)
    {
        uint32_t srcV = (uint32_t)((float)dstV * scaleY);
        if (srcV >= srcHeight)
        {
            srcV = srcHeight - 1;
        }

        const pxb_pixel_t* srcRow = &srcPixels[srcV * srcWidth];
        pxb_pixel_t* dstRow = &dstPixels[dstV * dstWidth];

        for (uint32_t dstU = 0; dstU < dstWidth; ++dstU)
        {
            uint32_t srcU = (uint32_t)((float)dstU * scaleX);
            if (srcU >= srcWidth)
            {
                srcU = srcWidth - 1;
            }

            dstRow[dstU] = srcRow[srcU];
        }
    }
}

void pxb_pixelbuffer_copyBilinear(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(source->pixels != NULL);
    PXB_ASSERT(destination->pixels != NULL);

    if (source->width == destination->width && source->height == destination->height)
    {
        pxb_pixelbuffer_copy(source, destination);
        return;
    }

    const float scaleX = (float)(source->width) / (float)destination->width;
    const float scaleY = (float)(source->height) / (float)destination->height;

    const uint32_t srcWidth = source->width;
    const uint32_t srcHeight = source->height;
    const uint32_t dstWidth = destination->width;
    const uint32_t dstHeight = destination->height;

    pxb_pixel_t* dstPixels = destination->pixels;

    for (uint32_t dstV = 0; dstV < dstHeight; ++dstV)
    {
        const float srcV = (float)dstV * scaleY;
        const uint32_t srcV0 = (uint32_t)srcV;
        const uint32_t srcV1 = (srcV0 + 1 >= srcHeight) ? (srcHeight - 1) : (srcV0 + 1);
        const float ty = srcV - (float)srcV0;

        pxb_pixel_t* dstRow = &dstPixels[dstV * dstWidth];

        for (uint32_t dstU = 0; dstU < dstWidth; ++dstU)
        {
            const float srcU = (float)dstU * scaleX;
            const uint32_t srcU0 = (uint32_t)srcU;
            const uint32_t srcU1 = (srcU0 + 1 >= srcWidth) ? (srcWidth - 1) : (srcU0 + 1);
            const float tx = srcU - (float)srcU0;

            const pxb_pixel_t p00 = pxb_pixelbuffer_getPixel(source, srcU0, srcV0);
            const pxb_pixel_t p10 = pxb_pixelbuffer_getPixel(source, srcU1, srcV0);
            const pxb_pixel_t p01 = pxb_pixelbuffer_getPixel(source, srcU0, srcV1);
            const pxb_pixel_t p11 = pxb_pixelbuffer_getPixel(source, srcU1, srcV1);

            const pxb_pixel_t top = pxb_pixel_lerp(p00, p10, tx);
            const pxb_pixel_t bottom = pxb_pixel_lerp(p01, p11, tx);
            dstRow[dstU] = pxb_pixel_lerp(top, bottom, ty);
        }
    }
}

void pxb_pixelbuffer_crop(const pxb_pixelbuffer_t* source, pxb_pixelbuffer_t* destination, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
{
    PXB_ASSERT(source != NULL);
    PXB_ASSERT(source->pixels != NULL);

    PXB_ASSERT(destination != NULL);
    PXB_ASSERT(destination->pixels != NULL);

    if (x >= source->width || y >= source->height)
    {
        return;
    }

    const uint32_t maxWidth = source->width - x;
    const uint32_t maxHeight = source->height - y;

    const uint32_t cropWidth = (width > maxWidth) ? maxWidth : width;
    const uint32_t cropHeight = (height > maxHeight) ? maxHeight : height;

    PXB_ASSERT(destination->width >= cropWidth && destination->height >= cropHeight);

    const uint32_t srcWidth = source->width;
    const uint32_t dstWidth = destination->width;

    const pxb_pixel_t* srcPixels = source->pixels;
    pxb_pixel_t* dstPixels = destination->pixels;

    const size_t rowBytes = cropWidth * sizeof(pxb_pixel_t);

    for (uint32_t v = 0; v < cropHeight; ++v)
    {
        const pxb_pixel_t* srcRow = &srcPixels[(y + v) * srcWidth + x];
        pxb_pixel_t* dstRow = &dstPixels[v * dstWidth];
        memcpy(dstRow, srcRow, rowBytes);
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
        const pxb_pixel_t* pixel = &source->pixels[i];

        uint8_t rgb[3];
        rgb[0] = pxb_pixel_clampToByte(pixel->r);
        rgb[1] = pxb_pixel_clampToByte(pixel->g);
        rgb[2] = pxb_pixel_clampToByte(pixel->b);

        fwrite(rgb, sizeof(uint8_t), 3, file);
    }

    fclose(file);

    PXB_LOG(PIXELBYTE, "Output file saved: %s", filePath);
}
