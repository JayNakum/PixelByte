#include "platform.h"

#include "pixelbyte.h"

#define RGFW_IMPLEMENTATION
#include <RGFW.h>

static RGFW_window gWindow;
static RGFW_surface gSurface;
static bool gHasSurface = false;
static u8* gSurfaceData = NULL;
static uint32_t gSurfaceWidth = 0;
static uint32_t gSurfaceHeight = 0;

static void platform_onDebug(RGFW_debugType type, RGFW_errorCode error, const char* message);
static bool platform_createSurface(const uint32_t width, const uint32_t height);
static void platform_destroySurface(void);
static platform_key_t platform_translateKey(const RGFW_key key);

bool platform_init(const char* title, const uint32_t width, const uint32_t height)
{
    RGFW_setDebugCallback(platform_onDebug);

    if (!RGFW_createWindowPtr(title, 100, 100, (i32)width, (i32)height, (RGFW_windowFlags)0, &gWindow))
    {
        PXB_LOG_MSG(RGFW, "Could not create window.");
        return false;
    }

    return true;
}

void platform_shutdown(void)
{
    platform_destroySurface();
    RGFW_window_closePtr(&gWindow);
}

bool platform_pollEvent(platform_event_t* event)
{
    RGFW_event rgfwEvent;
    while (RGFW_window_checkEvent(&gWindow, &rgfwEvent))
    {
        switch (rgfwEvent.type)
        {
        case RGFW_quit:
            event->type = PLATFORM_EVENT_QUIT;
            return true;
        case RGFW_windowResized:
        {
            i32 width = 0;
            i32 height = 0;
            RGFW_window_getSize(&gWindow, &width, &height);

            if (width <= 0 || height <= 0)
            {
                break;
            }

            event->type = PLATFORM_EVENT_RESIZED;
            event->width = (uint32_t)width;
            event->height = (uint32_t)height;
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
    if (!pixelBuffer || !pixelBuffer->pixels)
    {
        PXB_LOG_MSG(RGFW, "Invalid pixelBuffer.");
        return false;
    }

    if (!gHasSurface || gSurfaceWidth != pixelBuffer->width || gSurfaceHeight != pixelBuffer->height)
    {
        platform_destroySurface();
        if (!platform_createSurface(pixelBuffer->width, pixelBuffer->height))
        {
            return false;
        }
    }

    const size_t pixelCount = (size_t)pixelBuffer->width * (size_t)pixelBuffer->height;
    for (size_t i = 0; i < pixelCount; ++i)
    {
        const pxb_pixel_t* src = &pixelBuffer->pixels[i];
        gSurfaceData[i * 4 + 0] = pxb_pixel_clampToByte(src->r);
        gSurfaceData[i * 4 + 1] = pxb_pixel_clampToByte(src->g);
        gSurfaceData[i * 4 + 2] = pxb_pixel_clampToByte(src->b);
        gSurfaceData[i * 4 + 3] = pxb_pixel_clampToByte(src->a);
    }

    RGFW_window_blitSurface(&gWindow, &gSurface);

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
    PXB_LOG(RGFW, "type=%d code=%d: %s", (int)type, (int)error, message);
}

static bool platform_createSurface(const uint32_t width, const uint32_t height)
{
    gSurfaceData = calloc((size_t)width * (size_t)height * 4u, sizeof(u8));
    if (!gSurfaceData)
    {
        PXB_LOG_MSG(RGFW, "Could not allocate surface data.");
        return false;
    }

    if (!RGFW_createSurfacePtr(gSurfaceData, (i32)width, (i32)height, RGFW_formatRGBA8, &gSurface))
    {
        PXB_LOG_MSG(RGFW, "Could not create surface.");
        free(gSurfaceData);
        gSurfaceData = NULL;
        return false;
    }

    gHasSurface = true;
    gSurfaceWidth = width;
    gSurfaceHeight = height;
    return true;
}

static void platform_destroySurface(void)
{
    if (gHasSurface)
    {
        RGFW_surface_freePtr(&gSurface);
        gHasSurface = false;
    }

    free(gSurfaceData);
    gSurfaceData = NULL;
    gSurfaceWidth = 0;
    gSurfaceHeight = 0;
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
