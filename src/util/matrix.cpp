
#include "matrix.h"

#include <qgl.h>

#define PCW_OP(op) for (int i = 0; i < 16; i++) { \
                        m.data[i] = data[i] op mat.data[i]; \
                   }

#define SCL_OP(op) for (int i = 0; i < 16; i++) { \
                        m.data[i] = data[i] op s; \
                   }

Matrix4x4 Matrix4x4::operator + (const Matrix4x4 &mat) const
{
    Matrix4x4 m;
    PCW_OP(+)
    return m;
}

Matrix4x4 Matrix4x4::operator - (const Matrix4x4 &mat) const
{
    Matrix4x4 m;
    PCW_OP(-)
    return m;
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4 &mat) const
{
    float m0  = a * mat.a + b * mat.e + c * mat.i + d * mat.m,
          m1  = a * mat.b + b * mat.f + c * mat.j + d * mat.n,
          m2  = a * mat.c + b * mat.g + c * mat.k + d * mat.o,
          m3  = a * mat.d + b * mat.h + c * mat.l + d * mat.p,
          
          m4  = e * mat.a + f * mat.e + g * mat.i + h * mat.m,
          m5  = e * mat.b + f * mat.f + g * mat.j + h * mat.n,
          m6  = e * mat.c + f * mat.g + g * mat.k + h * mat.o,
          m7  = e * mat.d + f * mat.h + g * mat.l + h * mat.p,
          
          m8  = i * mat.a + j * mat.e + k * mat.i + l * mat.m,
          m9  = i * mat.b + j * mat.f + k * mat.j + l * mat.n,
          m10 = i * mat.c + j * mat.g + k * mat.k + l * mat.o,
          m11 = i * mat.d + j * mat.h + k * mat.l + l * mat.p,
          
          m12 = m * mat.a + n * mat.e + o * mat.i + p * mat.m,
          m13 = m * mat.b + n * mat.f + o * mat.j + p * mat.n,
          m14 = m * mat.c + n * mat.g + o * mat.k + p * mat.o,
          m15 = m * mat.d + n * mat.h + o * mat.l + p * mat.p;

    return Matrix4x4(m0 , m1 , m2 , m3 ,
                     m4 , m5 , m6 , m7 ,
                     m8 , m9 , m10, m11,
                     m12, m13, m14, m15);
}

Vector3 Matrix4x4::operator * (const Vector3 &v) const
{
    return Vector3(a * v.x + b * v.y + c * v.z,
                   e * v.x + f * v.y + g * v.z,
                   i * v.x + j * v.y + k * v.z);
}

Vector4 Matrix4x4::operator * (const Vector4 &v) const
{
    return Vector4(a * v.x + b * v.y + c * v.z + d * v.w,
                   e * v.x + f * v.y + g * v.z + h * v.w,
                   i * v.x + j * v.y + k * v.z + l * v.w,
                   m * v.x + n * v.y + o * v.z + p * v.w);
}

Matrix4x4 Matrix4x4::operator + (float s) const
{
    Matrix4x4 m;
    SCL_OP(+)
    return m;
}

Matrix4x4 Matrix4x4::operator - (float s) const
{
    Matrix4x4 m;
    SCL_OP(-)
    return m;
}

Matrix4x4 Matrix4x4::operator * (float s) const
{
    Matrix4x4 m;
    SCL_OP(*)
    return m;
}

Matrix4x4 Matrix4x4::operator / (float s) const
{
    if (s) {
        Matrix4x4 m;
        SCL_OP(/)
        return m;
    }
    return *this;
}

Matrix4x4 operator*(float val, const Matrix4x4 &r)
{
    return r * val;
}

void Matrix4x4::toOpenGLMatrix(float *buffer) const
{
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            buffer[x*4+y] = data[y*4+x];
        }
    }
}

void Matrix4x4::setOpenGLMatrix(float *glmatrix)
{
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            data[x*4+y] = glmatrix[y*4+x];
        }
    }
}

Matrix4x4 Matrix4x4::fromOpenGLMatrix(float *glmatrix)
{
    Matrix4x4 m;
    m.setOpenGLMatrix(glmatrix);
    return m;
}

void Matrix4x4::loadGL() const
{
    float data[16];
    toOpenGLMatrix(data);
    glLoadMatrixf(data);
}

void Matrix4x4::multGL() const
{
    float data[16];
    toOpenGLMatrix(data);
    glMultMatrixf(data);
}

void Matrix4x4::invert()
{
    float det = a*f*k*p-a*f*l*o-a*g*j*p+a*g*l*n+a*h*j*o-a*h*k*n-b*e*k*p+b*e*l*o+b*g*i*p-b*g*l*m-b*h*i*o+b*h*k*m+c*e*j*p-c*e*l*n-c*f*i*p+c*f*l*m+c*h*i*n-c*h*j*m-d*e*j*o+d*e*k*n+d*f*i*o-d*f*k*m-d*g*i*n+d*g*j*m;
    float _00 = (f*k*p+g*l*n+h*j*o-f*l*o-g*j*p-h*k*n)/det,
        _01 = (b*l*o+c*j*p+d*k*n-b*k*p-c*l*n-d*j*o)/det,
        _02 = (b*g*p+c*h*n+d*f*o-b*h*o-c*f*p-d*g*n)/det,
        _03 = (b*h*k+c*f*l+d*g*j-b*g*l-c*h*j-d*f*k)/det,
        _10 = (e*l*o+h*k*m+g*i*p-e*k*p-g*l*m-h*i*o)/det,
        _11 = (a*k*p+c*l*m+d*i*o-a*l*o-c*i*p-d*k*m)/det,
        _12 = (a*h*o+c*e*p+d*g*m-a*g*p-c*h*m-d*e*o)/det,
        _13 = (a*g*l+c*h*i+d*e*k-a*h*k-c*e*l-d*g*i)/det,
        _20 = (e*j*p+f*l*m+h*i*n-e*l*n-f*i*p-h*j*m)/det,
        _21 = (a*l*n+b*i*p+d*j*m-a*j*p-b*l*m-d*i*n)/det,
        _22 = (a*f*p+b*h*m+d*e*n-a*h*n-b*e*p-d*f*m)/det,
        _23 = (a*h*j+b*e*l+d*f*i-a*f*l-b*h*i-d*e*j)/det,
        _30 = (e*k*n+f*i*o+g*j*m-e*j*o-f*k*m-g*i*n)/det,
        _31 = (a*j*o+b*k*m+c*i*n-a*k*n-b*i*o-c*j*m)/det,
        _32 = (a*g*n+b*e*o+c*f*m-a*f*o-b*g*m-c*e*n)/det,
        _33 = (a*f*k+b*g*i+c*e*j-a*g*j-b*e*k-c*f*i)/det;
    a = _00;
    b = _01;
    c = _02;
    d = _03;
    e = _10;
    f = _11;
    g = _12;
    h = _13;
    i = _20;
    j = _21;
    k = _22;
    l = _23;
    m = _30;
    n = _31;
    o = _32;
    p = _33;
}

Matrix4x4 Matrix4x4::getInverse()
{
    Matrix4x4 m(*this);
    m.invert();
    return m;
}

Matrix4x4 Matrix4x4::rotationMatrix(const Vector3 &v, const float angle)
{
    float theta = atan2(v.z, v.x),
          phi = -atan2(v.y, sqrt(v.x*v.x + v.z*v.z));

    float cos_theta = cos(theta),
          sin_theta = sin(theta),
          cos_phi = cos(phi),
          sin_phi = sin(phi),
          cos_angle = cos(angle),
          sin_angle = sin(angle);

    Matrix4x4 rotx(1,         0,          0, 0,
                   0, cos_angle, -sin_angle, 0, 
                   0, sin_angle,  cos_angle, 0,
                   0,         0,          0, 1);
    Matrix4x4 roty(cos_theta, 0, sin_theta, 0,
                           0, 1,         0, 0,
                  -sin_theta, 0, cos_theta, 0,
                           0, 0,         0, 1);
    Matrix4x4 rotz(cos_phi, -sin_phi, 0, 0,
                   sin_phi,  cos_phi, 0, 0, 
                         0,        0, 1, 0,
                         0,        0, 0, 1);
    Matrix4x4 invroty(cos_theta, 0,-sin_theta, 0,
                              0, 1,         0, 0,
                      sin_theta, 0, cos_theta, 0,
                              0, 0,         0, 1);
    Matrix4x4 invrotz(cos_phi,  sin_phi, 0, 0,
                     -sin_phi,  cos_phi, 0, 0, 
                            0,        0, 1, 0,
                            0,        0, 0, 1);

    return invroty * invrotz * rotx * rotz * roty;
}

