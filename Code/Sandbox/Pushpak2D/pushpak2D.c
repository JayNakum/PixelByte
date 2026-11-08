#include "pushpak2D.h"

#include "vertex.h"
#include "player.h"

#define TOTAL_MEMORY_SIZE MB(128)
static pxb_memory_t gMemory;
static uint32_t gAllocationOffset;
static uint32_t gRemainingSizeInBytes;
static inline void* linear_allocator(const size_t sizeInBytes)
{
    void* allocation = pxb_memory_allocate(gMemory, gAllocationOffset, sizeInBytes);
    gAllocationOffset += sizeInBytes;
    gRemainingSizeInBytes -= sizeInBytes;
    return allocation;
}

#define PIXELBUFFER_COUNT 2
static pxb_pixelbuffer_t gPixelBuffers[PIXELBUFFER_COUNT];
static uint8_t gCurrentBackBufferIndex;
static struct Matrix3 gProjectionMatrix;

#define PLAYER_PRESETS 1
static struct PlayerData* gPlayerData[PLAYER_PRESETS];
static struct Player gPlayer;

static void loadGameData(void)
{
    // load all the sprites
    pxb_texture_t playerPresetSprites[PLAYER_PRESETS];
    playerPresetSprites[0].width = 100;
    playerPresetSprites[0].height = 100;
    const size_t allocationSize = playerPresetSprites[0].width * playerPresetSprites[0].height * sizeof(pxb_pixel_t);
    playerPresetSprites[0].pixels = linear_allocator(allocationSize);
    // dummy sprite texture
    pxb_pixelbuffer_clear(&playerPresetSprites[0], pxb_pixelRGB(200, 200, 200));
    pxb_draw_8bitChar(&playerPresetSprites[0], 3, 0, 'A', 16, PXB_PIXEL_COLOR_BLACK);

    // load the player presets
    const size_t playerDataSize = sizeof(struct PlayerData);
    gPlayerData[0] = linear_allocator(playerDataSize);
    gPlayerData[0]->speed = 100.0f;
    gPlayerData[0]->turnRadius = 2.6f;
    gPlayerData[0]->sprite = playerPresetSprites[0];
}

void initialize(const uint32_t width, const uint32_t height)
{
    gAllocationOffset = 0;
    gMemory = pxb_memory_create(TOTAL_MEMORY_SIZE);

    loadGameData();

    gPlayer = initializePlayer(gPlayerData[0]);

    const bool result = resize(width, height); // Allocates the pixel buffer at gAllocationOffset
    PXB_ASSERT(result);

    gCurrentBackBufferIndex = 0;
}

void update(const float deltaTime, const struct InputActions action)
{
    updatePlayer(&gPlayer, deltaTime, action);
    (void)deltaTime;
    if (action.screenshot)
    {
        pxb_pixelbuffer_t* currentBackBuffer = &gPixelBuffers[gCurrentBackBufferIndex];
        pxb_pixelbuffer_saveAsPPM(currentBackBuffer, ".\\screenshot.ppm");
    }
}

pxb_pixelbuffer_t* render(void)
{
    pxb_pixelbuffer_t* currentBackBuffer = &gPixelBuffers[gCurrentBackBufferIndex];

    pxb_pixelbuffer_clear(currentBackBuffer, PXB_PIXEL_COLOR_WHITE);

    const struct Matrix3 view = matrix3_view(0.0f, 0.0f, 0.0f, 1.0f);
    const struct Matrix3 viewProj = matrix3_multiply(gProjectionMatrix, view);

    renderPlayer(currentBackBuffer, &gPlayer, viewProj);

    gCurrentBackBufferIndex ^= 1;
    return currentBackBuffer;
}

bool resize(const uint32_t width, const uint32_t height)
{
    if (width == 0 || height == 0)
    {
        return false;
    }

    const size_t allocationSize = width * height * sizeof(pxb_pixel_t);
    if (allocationSize > TOTAL_MEMORY_SIZE)
    {
        return false;
    }

    gPixelBuffers[0].width = width;
    gPixelBuffers[0].height = height;
    gPixelBuffers[0].pixels = linear_allocator(allocationSize);
    PXB_ASSERT(gPixelBuffers[0].pixels);

    gPixelBuffers[1].width = width;
    gPixelBuffers[1].height = height;
    gPixelBuffers[1].pixels = linear_allocator(allocationSize);
    PXB_ASSERT(gPixelBuffers[1].pixels);

    gProjectionMatrix = matrix3_ortho((float)(width), (float)(height), 2.0f);

    return true;
}

void shutdown(void)
{
    pxb_memory_destroy(gMemory);
}
