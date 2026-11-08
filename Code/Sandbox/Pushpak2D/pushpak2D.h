#ifndef PSP_PUSHPAK2D
#define PSP_PUSHPAK2D

#include "pixelbyte.h"
#include "inputActions.h"

void initialize(const uint32_t width, const uint32_t height);

bool resize(const uint32_t width, const uint32_t height);

void update(const float deltaTime, const struct InputActions action);

pxb_pixelbuffer_t* render(void);

void shutdown(void);

#endif // !PSP_PUSHPAK2D
