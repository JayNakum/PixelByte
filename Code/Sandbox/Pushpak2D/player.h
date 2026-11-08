#ifndef PSP_PLAYER_H
#define PSP_PLAYER_H

#include "pixelbyte.h"
#include "inputActions.h"

#include "math/matrix3.h"

struct PlayerData
{
    float speed;
    float turnRadius;

    pxb_texture_t sprite;
};

struct Player
{
    float position[2];
    float rotation;

    const struct PlayerData* data;
};

struct Player initializePlayer(const struct PlayerData* data);

void updatePlayer(struct Player* player, const float dt, const struct InputActions action);

void renderPlayer(pxb_pixelbuffer_t* pixelBuffer, const struct Player* player, const struct Matrix3 viewProj);

#endif // PSP_PLAYER_H
