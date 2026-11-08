#ifndef PSP_VERTEX_H
#define PSP_VERTEX_H

#include "math/matrix3.h"

struct Vertex
{
    float x, y;
    float u, v;
};
static const struct Vertex QUAD[6] = {
    {  0.5f,  0.5f, 1.0f, 0.0f },
    {  0.5f, -0.5f, 1.0f, 1.0f },
    { -0.5f, -0.5f, 0.0f, 1.0f },
    { -0.5f, -0.5f, 0.0f, 1.0f },
    { -0.5f,  0.5f, 0.0f, 0.0f },
    {  0.5f,  0.5f, 1.0f, 0.0f }
};

static inline struct Vertex transform_vertex(const struct Matrix3 m, const struct Vertex v)
{
    return (struct Vertex) {
        v.x* m.mat[0][0] + v.y * m.mat[0][1] + m.mat[0][2],
        v.x* m.mat[1][0] + v.y * m.mat[1][1] + m.mat[1][2],
        v.u,
        v.v
    };
}

#endif // PSP_VERTEX_H
