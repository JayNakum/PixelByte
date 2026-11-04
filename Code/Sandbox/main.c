#include "pixelbyte.h"

#include "platform.h"

static bool gDidExit = false;
static const char* gScreenshotFilePath = NULL;
static pxb_pixelbuffer_t gPixelBuffer;

static void pxb_resize(pxb_pixelbuffer_t* pixelBuffer, const uint32_t newWidth, const uint32_t newHeight);
static void handleEvents(void);

int main(int argc, char** argv)
{
    gScreenshotFilePath = "./pixelbyte_output.ppm";
    for (int i = 0; i < argc; ++i)
    {
        PXB_LOG(SANDBOX, "%s", argv[i]);
    }

    const uint32_t WINDOW_WIDTH = 2048;
    const uint32_t WINDOW_HEIGHT = 1024;

    if (!platform_init("PixelByte", WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        return 1;
    }

    gPixelBuffer.pixelBuffer = calloc(WINDOW_WIDTH * WINDOW_HEIGHT, sizeof(pxb_pixel_t));
    gPixelBuffer.width = WINDOW_WIDTH;
    gPixelBuffer.height = WINDOW_HEIGHT;

    double lastTime = platform_timeSeconds();
    while (!gDidExit)
    {
        const double now = platform_timeSeconds();
        float elapsedTime = (float)(now - lastTime);
        lastTime = now;

        handleEvents();

        pxb_pixelbuffer_clear(&gPixelBuffer, PXB_PIXEL_COLOR_WHITE);

        (void)(elapsedTime);
        pxb_draw_text(&gPixelBuffer, 256, 420, "0xPIXELBYTE", 16, PXB_PIXEL_COLOR_BLACK);

        bool presentResult = platform_present(&gPixelBuffer);
        PXB_ASSERT(presentResult);
    }

    free(gPixelBuffer.pixelBuffer);
    platform_shutdown();
    return 0;
}

static void pxb_resize(pxb_pixelbuffer_t* pixelBuffer, const uint32_t newWidth, const uint32_t newHeight)
{
    pxb_pixelbuffer_t newPixelBuffer;
    newPixelBuffer.pixelBuffer = calloc(newWidth * newHeight, sizeof(pxb_pixel_t));
    newPixelBuffer.width = newWidth;
    newPixelBuffer.height = newHeight;

    pxb_pixelbuffer_copyBilinear(pixelBuffer, &newPixelBuffer);

    free(pixelBuffer->pixelBuffer);

    pixelBuffer->pixelBuffer = newPixelBuffer.pixelBuffer;
    pixelBuffer->width = newWidth;
    pixelBuffer->height = newHeight;
}

static void handleEvents(void)
{
    platform_event_t event;
    while (platform_pollEvent(&event))
    {
        switch (event.type)
        {
        case PLATFORM_EVENT_QUIT:
            gDidExit = true;
            break;
        case PLATFORM_EVENT_RESIZED:
            pxb_resize(&gPixelBuffer, event.width, event.height);
            break;
        case PLATFORM_EVENT_KEY_PRESSED:
            switch (event.key)
            {
            case PLATFORM_KEY_ESCAPE: gDidExit = true; break;
            case PLATFORM_KEY_F12: pxb_pixelbuffer_saveAsPPM(&gPixelBuffer, gScreenshotFilePath); break;
            default: break;
            }
            break;
        }
    }
}
