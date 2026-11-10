#include "platform.h"

#include "pixelbyte.h"

#define RGFW_IMPLEMENTATION
#include <RGFW.h>

struct RGFWPlatform
{
    RGFW_window window;
    RGFW_surface surface;

    uint8_t* surfaceData;
    uint32_t surfaceWidth;
    uint32_t surfaceHeight;
};
typedef struct RGFWPlatform platform_t;
static platform_t gPlatform;

static void platform_onDebug(RGFW_debugType type, RGFW_errorCode error, const char* message);
static bool platform_createSurface(const uint32_t width, const uint32_t height);
static void platform_destroySurface(void);
static platform_key_t platform_translateKey(const RGFW_key key);

bool platform_init(const char* title, const uint32_t width, const uint32_t height)
{
    RGFW_setDebugCallback(platform_onDebug);

    if (!RGFW_createWindowPtr(title, 100, 100, (int32_t)width, (int32_t)height, (RGFW_windowFlags)0, &gPlatform.window))
    {
        PXB_LOG_MSG(RGFW, "Could not create window.");
        return false;
    }

    platform_createSurface((uint32_t)(width), (uint32_t)(height));

    return true;
}

void platform_shutdown(void)
{
    platform_destroySurface();
    RGFW_window_closePtr(&gPlatform.window);
}

bool platform_pollEvent(platform_event_t* event)
{
    RGFW_event rgfwEvent;
    while (RGFW_window_checkEvent(&gPlatform.window, &rgfwEvent))
    {
        switch (rgfwEvent.type)
        {
        case RGFW_quit:
            event->type = PLATFORM_EVENT_QUIT;
            return true;
        case RGFW_windowResized:
        {
            int32_t width = 0;
            int32_t height = 0;
            RGFW_window_getSize(&gPlatform.window, &width, &height);

            if (width <= 0 || height <= 0)
            {
                break;
            }

            platform_destroySurface();
            platform_createSurface((uint32_t)(width), (uint32_t)(height));

            event->type = PLATFORM_EVENT_RESIZED;
            event->width = (uint32_t)(width);
            event->height = (uint32_t)(height);
            return true;
        }
        case RGFW_keyPressed:
        {
            event->type = PLATFORM_EVENT_KEY_PRESSED;
            event->key = platform_translateKey(rgfwEvent.key.value);
            return true;
        }
        case RGFW_keyReleased:
        {
            event->type = PLATFORM_EVENT_KEY_RELEASED;
            event->key = platform_translateKey(rgfwEvent.key.value);
            return true;
        }
        default:
            break;
        }
    }

    return false;
}

bool platform_present(const pxb_pixelbuffer_t* pixelBuffer)
{
    PXB_ASSERT(pixelBuffer && pixelBuffer->pixels && gPlatform.surfaceData);

    const uint32_t pixelBufferWidth  = pixelBuffer->width;
    const uint32_t pixelBufferHeight  = pixelBuffer->height;
    const uint32_t surfaceWidth = gPlatform.surfaceWidth;
    const uint32_t surfaceHeight = gPlatform.surfaceHeight;

    const float scale = fminf((float)surfaceWidth / (float)pixelBufferWidth, (float)surfaceHeight / (float)pixelBufferHeight);

    const uint32_t destinationWidth = (uint32_t)((float)pixelBufferWidth * scale + 0.5f);
    const uint32_t destinationHeight = (uint32_t)((float)pixelBufferHeight * scale + 0.5f);

    const uint32_t offsetX = (surfaceWidth - destinationWidth) / 2;
    const uint32_t offsetY = (surfaceHeight - destinationHeight) / 2;

    for (uint32_t y = 0; y < destinationHeight; ++y)
    {
        uint8_t* row = gPlatform.surfaceData + ((size_t)(y + offsetY) * surfaceWidth + offsetX) * 4u;

        const float v = ((float)y + 0.5f) * ((float)pixelBufferHeight / (float)destinationHeight) - 0.5f;

        for (uint32_t x = 0; x < destinationWidth; ++x)
        {
            const float u = ((float)x + 0.5f) * ((float)pixelBufferWidth / (float)destinationWidth) - 0.5f;
            const pxb_pixel_t src = pxb_sampleNearest(pixelBuffer, u, v);

            row[x * 4u + 0] = pxb_pixel_clampToByte(src.r);
            row[x * 4u + 1] = pxb_pixel_clampToByte(src.g);
            row[x * 4u + 2] = pxb_pixel_clampToByte(src.b);
            row[x * 4u + 3] = pxb_pixel_clampToByte(src.a);
        }
    }

    RGFW_window_blitSurface(&gPlatform.window, &gPlatform.surface);

    return true;
}

double platform_timeSeconds(void)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static void platform_onDebug(RGFW_debugType type, RGFW_errorCode error, const char* message)
{
    PXB_LOG(RGFW, "%s (%d, %d)", message, (int)type, (int)error);
}

static bool platform_createSurface(const uint32_t width, const uint32_t height)
{
    gPlatform.surfaceWidth = width;
    gPlatform.surfaceHeight = height;
    gPlatform.surfaceData = calloc((size_t)(width * height * 4u), sizeof(uint8_t));
    PXB_ASSERT(gPlatform.surfaceData);

    if (!RGFW_createSurfacePtr(gPlatform.surfaceData, (int32_t)width, (int32_t)height, RGFW_formatRGBA8, &gPlatform.surface))
    {
        PXB_LOG_MSG(RGFW, "Could not create surface.");
        free(gPlatform.surfaceData);
        gPlatform.surfaceData = NULL;
        return false;
    }

    return true;
}

static void platform_destroySurface(void)
{
    RGFW_surface_freePtr(&gPlatform.surface);

    free(gPlatform.surfaceData);
    gPlatform.surfaceData = NULL;
}

static platform_key_t platform_translateKey(const RGFW_key key)
{
    switch (key)
    {
    case RGFW_escape: return PLATFORM_KEY_ESCAPE;
    case RGFW_F12: return PLATFORM_KEY_F12;
    case RGFW_a: return PLATFORM_KEY_A;
    case RGFW_d: return PLATFORM_KEY_D;
    case RGFW_right: return PLATFORM_KEY_RIGHT;
    case RGFW_left: return PLATFORM_KEY_LEFT;
    default: return PLATFORM_KEY_UNKNOWN;
    }
}
