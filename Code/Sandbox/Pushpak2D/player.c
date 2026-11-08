#include "player.h"

#include "vertex.h"

struct Player initializePlayer(const struct PlayerData* data)
{
    struct Player newPlayer = (struct Player){ 0 };

    newPlayer.data = data;

    return newPlayer;
}

void updatePlayer(struct Player* player, const float dt, const struct InputActions action)
{
    if (action.left)
    {
        player->rotation += player->data->turnRadius * dt;
    }

    if (action.right)
    {
        player->rotation -= player->data->turnRadius * dt;
    }

    const float fx = -sinf(player->rotation);
    const float fy = cosf(player->rotation);

    const float speed = player->data->speed;
    player->position[0] += fx * speed * dt;
    player->position[1] += fy * speed * dt;
}

void renderPlayer(pxb_pixelbuffer_t* pixelBuffer, const struct Player* player, const struct Matrix3 viewProj)
{
    struct Matrix3 world = matrix3_identity();
    world = matrix3_multiply(world, matrix3_translate(player->position[0], player->position[1]));
    world = matrix3_multiply(world, matrix3_rotate(player->rotation));
    world = matrix3_multiply(world, matrix3_scale(50.0f, 50.0f));

    const struct Matrix3 mvp = matrix3_multiply(viewProj, world);

    struct Vertex transformedQuad[6];
    for (size_t i = 0; i < 6; ++i)
    {
        transformedQuad[i] = transform_vertex(mvp, QUAD[i]);
    }

    pxb_fill_triangle_textured(pixelBuffer,
        transformedQuad[0].x, transformedQuad[0].y,
        transformedQuad[0].u, transformedQuad[0].v,
        transformedQuad[1].x, transformedQuad[1].y,
        transformedQuad[1].u, transformedQuad[1].v,
        transformedQuad[2].x, transformedQuad[2].y,
        transformedQuad[2].u, transformedQuad[2].v,
        &player->data->sprite
    );
    pxb_fill_triangle_textured(pixelBuffer,
        transformedQuad[3].x, transformedQuad[3].y,
        transformedQuad[3].u, transformedQuad[3].v,
        transformedQuad[4].x, transformedQuad[4].y,
        transformedQuad[4].u, transformedQuad[4].v,
        transformedQuad[5].x, transformedQuad[5].y,
        transformedQuad[5].u, transformedQuad[5].v,
        &player->data->sprite
    );
    // pxb_fill_triangle(pixelBuffer, transformedQuad[0].x, transformedQuad[0].y, transformedQuad[1].x, transformedQuad[1].y, transformedQuad[2].x, transformedQuad[2].y, pxb_pixelRGB(101, 230, 219));
    // pxb_fill_triangle(pixelBuffer, transformedQuad[3].x, transformedQuad[3].y, transformedQuad[4].x, transformedQuad[4].y, transformedQuad[5].x, transformedQuad[5].y, pxb_pixelRGB(101, 230, 219));
}
