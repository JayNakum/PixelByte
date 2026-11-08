#ifndef PXB_MATRIX3_H
#define PXB_MATRIX3_H

#include "pch.h"

struct Matrix3
{
    float mat[3][3];
};

static inline struct Matrix3 matrix3_identity(void)
{
    return (struct Matrix3){{
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f }
    }};
}

static inline struct Matrix3 matrix3_scale(const float sx, const float sy)
{
    return (struct Matrix3){{
        { sx, 0.0f, 0.0f },
        { 0.0f, sy, 0.0f },
        { 0.0f, 0.0f, 1.0f }
    }};
}

static inline struct Matrix3 matrix3_rotate(const float angle)
{
    const float s = sinf(angle);
    const float c = cosf(angle);

    return (struct Matrix3){{
        { c, -s, 0.0f },
        { s, c, 0.0f },
        { 0.0f, 0.0f, 1.0f }
    }};
}

static inline struct Matrix3 matrix3_translate(const float x, const float y)
{
    return (struct Matrix3){{
        {1.0f, 0.0f, x},
        {0.0f, 1.0f, y},
        {0.0f, 0.0f, 1.0f}
    }};
}

static inline struct Matrix3 matrix3_multiply(const struct Matrix3 a, const struct Matrix3 b)
{
    struct Matrix3 result = (struct Matrix3){0};

    result.mat[0][0] = a.mat[0][0]*b.mat[0][0] + a.mat[0][1]*b.mat[1][0] + a.mat[0][2]*b.mat[2][0];
    result.mat[0][1] = a.mat[0][0]*b.mat[0][1] + a.mat[0][1]*b.mat[1][1] + a.mat[0][2]*b.mat[2][1];
    result.mat[0][2] = a.mat[0][0]*b.mat[0][2] + a.mat[0][1]*b.mat[1][2] + a.mat[0][2]*b.mat[2][2];

    result.mat[1][0] = a.mat[1][0]*b.mat[0][0] + a.mat[1][1]*b.mat[1][0] + a.mat[1][2]*b.mat[2][0];
    result.mat[1][1] = a.mat[1][0]*b.mat[0][1] + a.mat[1][1]*b.mat[1][1] + a.mat[1][2]*b.mat[2][1];
    result.mat[1][2] = a.mat[1][0]*b.mat[0][2] + a.mat[1][1]*b.mat[1][2] + a.mat[1][2]*b.mat[2][2];

    result.mat[2][0] = a.mat[2][0]*b.mat[0][0] + a.mat[2][1]*b.mat[1][0] + a.mat[2][2]*b.mat[2][0];
    result.mat[2][1] = a.mat[2][0]*b.mat[0][1] + a.mat[2][1]*b.mat[1][1] + a.mat[2][2]*b.mat[2][1];
    result.mat[2][2] = a.mat[2][0]*b.mat[0][2] + a.mat[2][1]*b.mat[1][2] + a.mat[2][2]*b.mat[2][2];

    return result;
}

static inline struct Matrix3 matrix3_view(const float x, const float y, const float rotation, const float zoom)
{
    const float s = sinf(rotation);
    const float c = cosf(rotation);
    const float invZ = 1.0f / zoom;

    return (struct Matrix3){{
        { c * invZ, s * invZ, (-x * c - y * s) * invZ },
        { -s * invZ, c * invZ, ( x * s - y * c) * invZ },
        { 0.0f, 0.0f, 1.0f }
    }};
}

// static inline struct Matrix3 matrix3_ortho(const float left, const float right, const float bottom, const float top)
// {
//     const float invW = 1.0f / (right - left);
//     const float invH = 1.0f / (top - bottom);

//     return (struct Matrix3){{
//         { 2.0f * invW, 0.0f, -(right + left) * invW },
//         { 0.0f, 2.0f * invH, -(top + bottom) * invH },
//         { 0.0f, 0.0f, 1.0f }
//     }};
// }

static inline struct Matrix3 matrix3_ortho(const float width, const float height, const float scale)
{
    return (struct Matrix3){{
        { scale,  0.0f,  width * 0.5f },
        { 0.0f, -scale,  height * 0.5f },
        { 0.0f,  0.0f,  1.0f }
    }};
}

static inline struct Matrix3 matrix3_viewport(const float width, const float height)
{
    return (struct Matrix3){{
        {  width * 0.5f,  0.0f, width * 0.5f },
        {  0.0f, -height * 0.5f, height * 0.5f },
        {  0.0f,  0.0f,  1.0f }
    }};
}

#endif // PXB_MATRIX3_H
