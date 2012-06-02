
#pragma once
#include "vector.h"
#include <cstring>

class Matrix4x4
{
public:

    union {
        float data[16];
        struct {
            float a, b, c, d,
                  e, f, g, h,
                  i, j, k, l,
                  m, n, o, p;
        }; 
    };

    Matrix4x4(float _00 = 1.f, float _01 = 0.f, float _02 = 0.f, float _03 = 0.f,
              float _10 = 0.f, float _11 = 1.f, float _12 = 0.f, float _13 = 0.f,
              float _20 = 0.f, float _21 = 0.f, float _22 = 1.f, float _23 = 0.f,
              float _30 = 0.f, float _31 = 0.f, float _32 = 0.f, float _33 = 1.f) : a(_00), b(_01), c(_02), d(_03),
                                                                                    e(_10), f(_11), g(_12), h(_13),
                                                                                    i(_20), j(_21), k(_22), l(_23),
                                                                                    m(_30), n(_31), o(_32), p(_33) {}

    Matrix4x4(const float *copy)                        { memcpy(data, copy,      16 * sizeof(float)); }
    Matrix4x4(const Matrix4x4 &copy)                    { memcpy(data, copy.data, 16 * sizeof(float)); }

    Matrix4x4 operator + (const Matrix4x4 &mat) const;
    Matrix4x4 operator - (const Matrix4x4 &mat) const;
    Matrix4x4 operator * (const Matrix4x4 &mat) const;
    
    Vector3 operator * (const Vector3 &v) const;
    Vector4 operator * (const Vector4 &v) const; 
    
    Matrix4x4 operator + (float s) const;
    Matrix4x4 operator - (float s) const;
    Matrix4x4 operator * (float s) const;
    Matrix4x4 operator / (float s) const;

    inline Matrix4x4 operator += (const Matrix4x4 &mat) { return *this = *this + mat; }
    inline Matrix4x4 operator -= (const Matrix4x4 &mat) { return *this = *this - mat; }
    inline Matrix4x4 operator *= (const Matrix4x4 &mat) { return *this = *this * mat; }

    inline Matrix4x4 operator += (float s) { return *this = *this + s; }
    inline Matrix4x4 operator -= (float s) { return *this = *this - s; }
    inline Matrix4x4 operator *= (float s) { return *this = *this * s; }
    inline Matrix4x4 operator /= (float s) { return *this = *this / s; }

    static Matrix4x4 fromOpenGLMatrix(float *glmatrix);
    void setOpenGLMatrix(float *glmatrix);
    void toOpenGLMatrix(float *buffer) const;
    void loadGL() const;
    void multGL() const;

    void invert();
    Matrix4x4 getInverse();

    static Matrix4x4 rotationMatrix(const Vector3 &axis, const float angle);

    friend Matrix4x4 operator*(float val, const Matrix4x4 &r);
};
