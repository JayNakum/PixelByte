#include "pixelbyte.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static bool gDidExit = false;
static const char* gScreenshotFilePath = NULL;
static pxb_pixelbuffer_t gPixelBuffer;

static bool sdl_pxb_present(SDL_Window* window, const pxb_pixelbuffer_t* pixelBuffer);
static void pxb_resize(pxb_pixelbuffer_t* pixelBuffer, const uint32_t newWidth, const uint32_t newHeight);
static void sdl_pxb_handleEvents(void);

int main(int argc, char** argv)
{
    gScreenshotFilePath = "./pixelbyte_output.ppm";
    for (int i = 0; i < argc; ++i)
    {
        PXB_LOG(SANDBOX, "%s", argv[i]);
    }

    const uint32_t WINDOW_WIDTH = 2048;
    const uint32_t WINDOW_HEIGHT = 1024;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        PXB_LOG(SDL, "Could not initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("PixelByte", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        PXB_LOG(SDL, "Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    gPixelBuffer.pixelBuffer = calloc(WINDOW_WIDTH * WINDOW_HEIGHT, sizeof(pxb_pixel_t));
    gPixelBuffer.width = WINDOW_WIDTH;
    gPixelBuffer.height = WINDOW_HEIGHT;

    uint64_t lastTime = SDL_GetTicksNS();
    while (!gDidExit)
    {
        uint64_t now = SDL_GetTicksNS();
        float elapsedTime = (float)(now - lastTime) / (float)(1e9);
        lastTime = now;

        sdl_pxb_handleEvents();

        pxb_pixelbuffer_clear(&gPixelBuffer, PXB_PIXEL_COLOR_WHITE);

        (void)(elapsedTime);
        pxb_draw_text(&gPixelBuffer, 256, 420, "0xPIXELBYTE", 16, PXB_PIXEL_COLOR_BLACK);

        bool presentResult = sdl_pxb_present(window, &gPixelBuffer);
        PXB_ASSERT(presentResult);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

static bool sdl_pxb_present(SDL_Window* window, const pxb_pixelbuffer_t* pixelBuffer)
{
    if (!window || !pixelBuffer || !pixelBuffer->pixelBuffer)
    {
        PXB_LOG_MSG(SDL, "Invalid window or pixelBuffer.");
        return false;
    }

    // int windowWidth, windowHeight;
    // SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    SDL_Surface* fbSurface = SDL_CreateSurface((int)pixelBuffer->width, (int)pixelBuffer->height, SDL_PIXELFORMAT_RGBA8888);
    if (!fbSurface)
    {
        PXB_LOG(SDL, "SDL_CreateSurface failed: %s", SDL_GetError());
        return false;
    }

    if (SDL_MUSTLOCK(fbSurface))
    {
        if (SDL_LockSurface(fbSurface) != 0)
        {
            PXB_LOG(SDL, "SDL_LockSurface failed: %s", SDL_GetError());
            SDL_DestroySurface(fbSurface);
            return false;
        }
    }

    int pitchPixels = fbSurface->pitch / 4;
    uint32_t* dst = (uint32_t*)(fbSurface->pixels);

    for (uint32_t y = 0; y < pixelBuffer->height; ++y)
    {
        for (uint32_t x = 0; x < pixelBuffer->width; ++x)
        {
            const pxb_pixel_t* src = &pixelBuffer->pixelBuffer[y * pixelBuffer->width + x];
            const uint8_t r = pxb_pixel_clampToByte(src->r);
            const uint8_t g = pxb_pixel_clampToByte(src->g);
            const uint8_t b = pxb_pixel_clampToByte(src->b);
            const uint8_t a = pxb_pixel_clampToByte(src->a);
            dst[y * (uint32_t)(pitchPixels) + x] = (uint32_t)SDL_MapSurfaceRGBA(fbSurface, r, g, b, a);
        }
    }

    if (SDL_MUSTLOCK(fbSurface))
    {
        SDL_UnlockSurface(fbSurface);
    }

    SDL_Surface* winSurface = SDL_GetWindowSurface(window);
    if (!winSurface)
    {
        PXB_LOG(SDL, "SDL_GetWindowSurface failed: %s", SDL_GetError());
        SDL_DestroySurface(fbSurface);
        return false;
    }

    const SDL_Rect srcRect = { 0, 0, (int)pixelBuffer->width, (int)pixelBuffer->height };
    const SDL_Rect dstRect = { 0, 0, winSurface->w, winSurface->h };

    SDL_BlitSurfaceScaled(fbSurface, &srcRect, winSurface, &dstRect, SDL_SCALEMODE_NEAREST);
    SDL_UpdateWindowSurface(window);
    SDL_DestroySurface(fbSurface);

    return true;
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

static void sdl_pxb_handleEvents(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            gDidExit = true;
            break;
        case SDL_EVENT_WINDOW_RESIZED:
        {
            const uint32_t newWidth = (uint32_t)event.window.data1;
            const uint32_t newHeight = (uint32_t)event.window.data2;

            if (newWidth == 0 || newHeight == 0)
            {
                break;
            }

            pxb_resize(&gPixelBuffer, newWidth, newHeight);
        }
        break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
        {
            // bool pressed = (event.type == SDL_EVENT_KEY_DOWN);

            switch (event.key.key)
            {
            case SDLK_ESCAPE: gDidExit = true; break;
            case SDLK_F12: pxb_pixelbuffer_saveAsPPM(&gPixelBuffer, gScreenshotFilePath); break;
            default: break;
            }
        }
        break;

        default:
            break;
        }
    }
}
