#ifndef PIXELBYTE_SANDBOX_PLATFORM_H
#define PIXELBYTE_SANDBOX_PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

#include "pixelbuffer.h"

typedef enum
{
    PLATFORM_EVENT_QUIT,
    PLATFORM_EVENT_RESIZED,
    PLATFORM_EVENT_KEY_PRESSED,
    PLATFORM_EVENT_KEY_RELEASED,

} platform_eventType_t;

typedef enum
{
    PLATFORM_KEY_UNKNOWN,
    PLATFORM_KEY_ESCAPE,
    PLATFORM_KEY_F12,
    PLATFORM_KEY_A,
    PLATFORM_KEY_D,

} platform_key_t;

typedef struct
{
    platform_eventType_t type;
    platform_key_t key;
    uint32_t width, height;

} platform_event_t;

bool platform_init(const char* title, const uint32_t width, const uint32_t height);
void platform_shutdown(void);

bool platform_pollEvent(platform_event_t* event);
bool platform_present(const pxb_pixelbuffer_t* pixelBuffer);

double platform_timeSeconds(void);

#endif // PIXELBYTE_SANDBOX_PLATFORM_H
