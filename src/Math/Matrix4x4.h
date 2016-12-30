//
//  Matrix4x4.h
//
//  Created by Shenyao Ke on 1/21/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once

#include "Core/Kaguya.h"
#include "Math/Geometry.h"
#include "Math/Matrix3x3.h"
#include "Tracer/Ray.h"

namespace Kaguya
{

struct Matrix4x4
{
    Matrix4x4() : mtx{ 1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1 }
    {
    }
    Matrix4x4(Float val) : mtx{ val }
    {}
    Matrix4x4(const Float mat[4][4])
    {
        memcpy(mtx, mat, sizeof(mtx));
    }
    Matrix4x4(const Float mat[16])
    {
        memcpy(mtx, mat, sizeof(mtx));
    }
    Matrix4x4(const Matrix4x4 &mat)
    {
        memcpy(mtx, mat.mtx, sizeof(mtx));
    }
    Matrix4x4(Float t00, Float t01, Float t02, Float t03,
              Float t10, Float t11, Float t12, Float t13,
              Float t20, Float t21, Float t22, Float t23,
              Float t30, Float t31, Float t32, Float t33)
    {
        mtx[0][0] = t00; mtx[0][1] = t01; mtx[0][2] = t02; mtx[0][3] = t03;
        mtx[1][0] = t10; mtx[1][1] = t11; mtx[1][2] = t12; mtx[1][3] = t13;
        mtx[2][0] = t20; mtx[2][1] = t21; mtx[2][2] = t22; mtx[2][3] = t23;
        mtx[3][0] = t30; mtx[3][1] = t31; mtx[3][2] = t32; mtx[3][3] = t33;
    }
    ~Matrix4x4() {}

    // Utils
    Float* data() { return *mtx; }
    const Float* data() const { return *mtx; }

    // Operators
    Float* operator [] (int i);
    const Float* operator [] (int i) const;
    Point3f operator () (const Point3f &v, Float w) const;
    Point3f operator () (const Point3f &v) const;
    Vector3f operator () (const Vector3f &v) const;
    Ray operator () (const Ray &ray) const;
    const Matrix4x4 &operator = (const Matrix4x4 &mat);
    friend Matrix4x4 operator + (const Matrix4x4 &m1, const Matrix4x4 &m2);
    friend Matrix4x4 operator - (const Matrix4x4 &m1, const Matrix4x4 &m2);
    friend Matrix4x4 operator * (const Matrix4x4 &m1, const Matrix4x4 &m2);
    //friend Vector4D operator * (const Matrix4x4 &m, const Vector4D &p);
    Vector3f operator * (const Vector3f &v);
    Point3f operator * (const Point3f &p);
    friend bool operator == (const Matrix4x4 &m1, const Matrix4x4 &m2);
    friend bool operator != (const Matrix4x4 &m1, const Matrix4x4 &m2);
    //Vector3D operator * (Vector3D &p) const;
    //Matrix4x4 operator / (const Matrix4x4 &) const;

    void setToZero();
    void setToIdentity();

    void setValueAt(int x, int y, Float val)
    {
        // (X, Y) is (row, column) indices in Row Major
        mtx[y][x] = val;
    }

    void printInfo(const std::string &text = "") const;
    Float determinant() const;
    Float minor(int x, int y) const;
    Float cofactor(int x, int y) const;
    Matrix4x4 transpose() const;
    Matrix4x4 adjoint() const;
    Matrix4x4 inverse() const;

    //Set transformation matrix
    static Matrix4x4 zero();
    static Matrix4x4 identity();
    static Matrix4x4 translate(Float tx, Float ty, Float tz);
    static Matrix4x4 translate(const Vector3f &vec);
    static Matrix4x4 RotateX(Float theta);
    static Matrix4x4 RotateY(Float theta);
    static Matrix4x4 RotateZ(Float theta);
    static Matrix4x4 rotate(Float alpha, Float beta, Float gamma);// in degree
    static Matrix4x4 rotate(const Vector3f &axis, Float theta,
                            bool isNormalized = false);
    static Matrix4x4 scale(Float sx, Float sy, Float sz);
    static Matrix4x4 scale(Float scale);
    static Matrix4x4 shear(const Vector3f &vec);
    static Matrix4x4 reflect(const Vector3f &vec);
    static Matrix4x4 lookAt(const Point3f &pos = Point3f(),
                            const Point3f &target = Point3f(0, 0, 1), const Vector3f &up = Vector3f(0, 1, 0));
    static Matrix4x4 perspective(Float verticalAngle = 90, Float aspectRatio = 1.6,
                                 Float nearPlane = 0.001, Float farPlane = 100);

    static Matrix4x4 perspectiveFromFilm(Float filmVert = 1, Float filmHori = 1,
                                         Float focal = 1, Float nearPlane = 0.001, Float farPlane = 100);

    static Matrix4x4 orthography(Float lf = -1, Float rt = 1,
                                 Float bt = -1, Float tp = 1, Float nr = -1, Float fr = 1);

    // Column Major
    Float mtx[4][4];
    //Float det = 0;
};

inline Float* Matrix4x4::operator[](int i)
{
    return mtx[i];
}

inline const Float* Matrix4x4::operator[](int i) const
{
    return mtx[i];
}

inline Point3f Matrix4x4::operator()(const Point3f &v, Float w) const
{
    Float x = v.x, y = v.y, z = v.z;
    Float xp = mtx[0][0] * x + mtx[1][0] * y + mtx[2][0] * z;
    Float yp = mtx[0][1] * x + mtx[1][1] * y + mtx[2][1] * z;
    Float zp = mtx[0][2] * x + mtx[1][2] * y + mtx[2][2] * z;
    if (w == 1.0)
    {
        xp += mtx[3][0];
        yp += mtx[3][1];
        zp += mtx[3][2];
        w = mtx[0][3] * x + mtx[1][3] * y + mtx[2][3] * z + mtx[3][3];
        if (w != 1.0)
        {
            Float invW = (Float)1 / w;
            xp *= invW;
            yp *= invW;
            zp *= invW;
        }
    }
    return Point3f(xp, yp, zp);
}

inline Point3f Matrix4x4::operator()(const Point3f &v) const
{
    Float x = v.x, y = v.y, z = v.z;
    Float xp = mtx[0][0] * x + mtx[1][0] * y + mtx[2][0] * z + mtx[3][0];
    Float yp = mtx[0][1] * x + mtx[1][1] * y + mtx[2][1] * z + mtx[3][1];
    Float zp = mtx[0][2] * x + mtx[1][2] * y + mtx[2][2] * z + mtx[3][2];
    Float wp = mtx[0][3] * x + mtx[1][3] * y + mtx[2][3] * z + mtx[3][3];
    Assert(wp != 0);
    if (wp == 1)
    {
        return Point3f(xp, yp, zp);
    }
    else
    {
        wp = (Float)1 / wp;
        return Point3f(xp * wp, yp * wp, zp * wp);
    }
}

inline Vector3f Matrix4x4::operator()(const Vector3f &v) const
{
    return Vector3f(
        v.x * mtx[0][0] + v.y * mtx[1][0] + v.z * mtx[2][0],
        v.x * mtx[0][1] + v.y * mtx[1][1] + v.z * mtx[2][1],
        v.x * mtx[0][2] + v.y * mtx[1][2] + v.z * mtx[2][2]
    );
}

inline Ray Matrix4x4::operator()(const Ray & ray) const
{
    Ray ret((*this)(ray.o), (*this)(ray.d), ray.tmin, ray.tmax);
    ret.time = ray.time;
    //ret.dp = ray.dp;
    return ret;
}

inline Matrix4x4 operator+(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
    return Matrix4x4(
        m1.mtx[0][0] + m2.mtx[0][0],
        m1.mtx[0][1] + m2.mtx[0][1],
        m1.mtx[0][2] + m2.mtx[0][2],
        m1.mtx[0][3] + m2.mtx[0][3],

        m1.mtx[1][0] + m2.mtx[1][0],
        m1.mtx[1][1] + m2.mtx[1][1],
        m1.mtx[1][2] + m2.mtx[1][2],
        m1.mtx[1][3] + m2.mtx[1][3],

        m1.mtx[2][0] + m2.mtx[2][0],
        m1.mtx[2][1] + m2.mtx[2][1],
        m1.mtx[2][2] + m2.mtx[2][2],
        m1.mtx[2][3] + m2.mtx[2][3],

        m1.mtx[3][0] + m2.mtx[3][0],
        m1.mtx[3][1] + m2.mtx[3][1],
        m1.mtx[3][2] + m2.mtx[3][2],
        m1.mtx[3][3] + m2.mtx[3][3]
    );
}

inline Matrix4x4 operator-(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
    return Matrix4x4(
        m1.mtx[0][0] - m2.mtx[0][0],
        m1.mtx[0][1] - m2.mtx[0][1],
        m1.mtx[0][2] - m2.mtx[0][2],
        m1.mtx[0][3] - m2.mtx[0][3],

        m1.mtx[1][0] - m2.mtx[1][0],
        m1.mtx[1][1] - m2.mtx[1][1],
        m1.mtx[1][2] - m2.mtx[1][2],
        m1.mtx[1][3] - m2.mtx[1][3],

        m1.mtx[2][0] - m2.mtx[2][0],
        m1.mtx[2][1] - m2.mtx[2][1],
        m1.mtx[2][2] - m2.mtx[2][2],
        m1.mtx[2][3] - m2.mtx[2][3],

        m1.mtx[3][0] - m2.mtx[3][0],
        m1.mtx[3][1] - m2.mtx[3][1],
        m1.mtx[3][2] - m2.mtx[3][2],
        m1.mtx[3][3] - m2.mtx[3][3]
    );
}

inline Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
    Matrix4x4 ret(0.);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                ret.mtx[j][i] += m1.mtx[k][i] * m2.mtx[j][k];
            }
        }
    }
    return ret;
}
/*

inline Vector4D operator*(const Matrix4x4 &m, const Vector4D &p)
{
    return Vector4D(
        p.x * m.mtx[0][0] + p.y * m.mtx[1][0] + p.z * m.mtx[2][0] + p.w * m.mtx[3][0],
        p.x * m.mtx[0][1] + p.y * m.mtx[1][1] + p.z * m.mtx[2][1] + p.w * m.mtx[3][1],
        p.x * m.mtx[0][2] + p.y * m.mtx[1][2] + p.z * m.mtx[2][2] + p.w * m.mtx[3][2],
        p.x * m.mtx[0][3] + p.y * m.mtx[1][3] + p.z * m.mtx[2][3] + p.w * m.mtx[3][3]
        );
}*/

inline Vector3f Matrix4x4::operator*(const Vector3f &v)
{
    return Vector3f(
        v.x * mtx[0][0] + v.y * mtx[1][0] + v.z * mtx[2][0],
        v.x * mtx[0][1] + v.y * mtx[1][1] + v.z * mtx[2][1],
        v.x * mtx[0][2] + v.y * mtx[1][2] + v.z * mtx[2][2]
    );
}

inline Point3f Matrix4x4::operator*(const Point3f &p)
{
    Float x = p.x, y = p.y, z = p.z;
    Float xp = x * mtx[0][0] + y * mtx[1][0] + z * mtx[2][0] + mtx[3][0];
    Float yp = x * mtx[0][1] + y * mtx[1][1] + z * mtx[2][1] + mtx[3][1];
    Float zp = x * mtx[0][2] + y * mtx[1][2] + z * mtx[2][2] + mtx[3][2];
    Float wp = x * mtx[0][3] + y * mtx[1][3] + z * mtx[2][3] + mtx[3][3];
    Assert(wp != 0);
    if (wp == 1)
    {
        return Point3f(xp, yp, zp);
    }
    else
    {
        wp = 1. / wp;
        return Point3f(xp * wp, yp * wp, zp * wp);
    }
}



inline bool operator==(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
    return m1.mtx[0][0] == m2.mtx[0][0]
        && m1.mtx[0][1] == m2.mtx[0][1]
        && m1.mtx[0][2] == m2.mtx[0][2]
        && m1.mtx[0][3] == m2.mtx[0][3]
        && m1.mtx[1][0] == m2.mtx[1][0]
        && m1.mtx[1][1] == m2.mtx[1][1]
        && m1.mtx[1][2] == m2.mtx[1][2]
        && m1.mtx[1][3] == m2.mtx[1][3]
        && m1.mtx[2][0] == m2.mtx[2][0]
        && m1.mtx[2][1] == m2.mtx[2][1]
        && m1.mtx[2][2] == m2.mtx[2][2]
        && m1.mtx[2][3] == m2.mtx[2][3]
        && m1.mtx[3][0] == m2.mtx[3][0]
        && m1.mtx[3][1] == m2.mtx[3][1]
        && m1.mtx[3][2] == m2.mtx[3][2]
        && m1.mtx[3][3] == m2.mtx[3][3];
}

inline bool operator!=(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
    return m1.mtx[0][0] != m2.mtx[0][0]
        || m1.mtx[0][1] != m2.mtx[0][1]
        || m1.mtx[0][2] != m2.mtx[0][2]
        || m1.mtx[0][3] != m2.mtx[0][3]
        || m1.mtx[1][0] != m2.mtx[1][0]
        || m1.mtx[1][1] != m2.mtx[1][1]
        || m1.mtx[1][2] != m2.mtx[1][2]
        || m1.mtx[1][3] != m2.mtx[1][3]
        || m1.mtx[2][0] != m2.mtx[2][0]
        || m1.mtx[2][1] != m2.mtx[2][1]
        || m1.mtx[2][2] != m2.mtx[2][2]
        || m1.mtx[2][3] != m2.mtx[2][3]
        || m1.mtx[3][0] != m2.mtx[3][0]
        || m1.mtx[3][1] != m2.mtx[3][1]
        || m1.mtx[3][2] != m2.mtx[3][2]
        || m1.mtx[3][3] != m2.mtx[3][3];
}

inline const Matrix4x4 &Matrix4x4::operator = (const Matrix4x4 &mat)
{
    memcpy(mtx, mat.mtx, sizeof(mtx));
    return *this;
}

inline void Matrix4x4::setToZero()
{
    memset(mtx, 0, sizeof(mtx));
}

inline void Matrix4x4::setToIdentity()
{
    mtx[0][0] = mtx[1][1] = mtx[2][2] = mtx[3][3] = 1.0;

    mtx[0][1] = mtx[0][2] = mtx[0][3] =
        mtx[1][0] = mtx[1][2] = mtx[1][3] =
        mtx[2][0] = mtx[2][1] = mtx[2][3] =
        mtx[3][0] = mtx[3][1] = mtx[3][2] = 0.0;
}

inline void Matrix4x4::printInfo(const std::string &msg) const
{
    if (!msg.empty())
    {
        std::cout << msg << std::endl;
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << mtx[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}
inline Float Matrix4x4::determinant() const
{
    Float det = 0;
    for (int i = 0; i < 4; i++)
    {
        det += mtx[0][i] * mtx[1][(i + 1) % 4] * mtx[2][(i + 2) % 4] * mtx[3][(i + 3) % 4];
        det -= mtx[3][i] * mtx[2][(i + 1) % 4] * mtx[1][(i + 2) % 4] * mtx[0][(i + 3) % 4];
    }
    return det;
}
inline Float Matrix4x4::minor(int x, int y) const
{
    Matrix3x3 tmpM;
    for (int i = 1; i < 4; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            tmpM.mtx[i - 1][j - 1] = mtx[(x + i) % 4][(y + j) % 4];
        }
    }
    return tmpM.determinant();
}
inline Float Matrix4x4::cofactor(int x, int y) const
{
    if ((x + y) % 2 == 0)
    {
        return minor(x, y);
    }
    else
    {
        return -minor(x, y);
    }
}

inline Matrix4x4 Matrix4x4::zero()
{
    return Matrix4x4((Float)0);
}

inline Matrix4x4 Matrix4x4::identity()
{
    return Matrix4x4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

inline Matrix4x4 Matrix4x4::transpose() const
{
    return Matrix4x4(
        mtx[0][0], mtx[1][0], mtx[2][0], mtx[3][0],
        mtx[0][1], mtx[1][1], mtx[2][1], mtx[3][1],
        mtx[0][2], mtx[1][2], mtx[2][2], mtx[3][2],
        mtx[0][3], mtx[1][3], mtx[2][3], mtx[3][3]);
}

inline Matrix4x4 Matrix4x4::adjoint() const
{
    Matrix4x4 ret;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ret.mtx[i][j] = cofactor(j, i);
        }
    }
    return ret;
}

inline Matrix4x4 Matrix4x4::inverse() const
{
    Matrix4x4 ret, adjM;
    Float det = determinant();
    if (det == 0)
    {
#ifdef _DEBUG
        std::cout << "The matrix is non-inversable!" << std::endl;
#endif
        return zero();
    }
    adjM = adjoint();

    Float invDet = 1.0 / det;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ret.mtx[i][j] = adjM.mtx[i][j] * invDet;
        }
    }

    return ret;
}

inline Matrix4x4 Matrix4x4::lookAt(const Point3f &pos, const Point3f &target, const Vector3f &up)
{
    //Camera to World
    Vector3f nz = pos - target;
    // distance between target and camera position is too small
    if (isFuzzyNull(nz.x) && isFuzzyNull(nz.y) && isFuzzyNull(nz.z))
    {
        return Matrix4x4();
    }
    nz.normalize();
    Vector3f nx = normalize(cross(up, nz));//right dir
    Vector3f ny = cross(nz, nx);
    nz.normalize();
#ifdef RIGHT_HAND_ORDER // OpenGL style
    return Matrix4x4(
        nx.x, nx.y, nx.z, 0.0,
        ny.x, ny.y, ny.z, 0.0,
        nz.x, nz.y, nz.z, 0.0,
        pos.x, pos.y, pos.z, 1.0
    );
#else // DirectX style
    return Matrix4x4(
        nx.x, nx.y, nx.z, 0.0,
        ny.x, ny.y, ny.z, 0.0,
        -nz.x, -nz.y, -nz.z, 0.0,
        pos.x, pos.y, pos.z, 1.0
    );
#endif
}

inline Matrix4x4 Matrix4x4::perspective(Float verticalAngle, Float aspectRatio, Float nearPlane, Float farPlane)
{

    Float radAngle = degreeToRadian(verticalAngle * 0.5);
    Float sy = std::sin(degreeToRadian(verticalAngle * 0.5));
    if (sy == 0)
    {
        return Matrix4x4(0.);
    }
    sy = std::cos(radAngle) / sy;

    Float sx = sy / aspectRatio;
    Float clip = nearPlane - farPlane;
#ifdef RIGHT_HAND_ORDER // OpenGL style
    Float sz = (farPlane + nearPlane) / clip;
    Float pz = 2.0 * farPlane * nearPlane / clip;
    Float w = -1.0;
#else // DirectX style
    Float sz = -(farPlane + nearPlane) / clip;
    Float pz = 2.0 * farPlane * nearPlane / clip;
    Float w = 1.0;
#endif

    return Matrix4x4(
        sx, 0.0, 0.0, 0.0,
        0.0, sy, 0.0, 0.0,
        0.0, 0.0, sz, w,
        0.0, 0.0, pz, 0.0
    );
}
inline Matrix4x4 Matrix4x4::perspectiveFromFilm(
    Float filmVert, Float filmHori, Float focal,
    Float nearPlane, Float farPlane)
{
    if (focal == 0 || filmVert == 0 || filmHori == 0)
    {
        return Matrix4x4(0.);
    }
    //focal *= nearPlane;
    Float sy = focal / filmVert * 2;
    Float sx = focal / filmHori * 2;
    Float clip = nearPlane - farPlane;
#ifdef RIGHT_HAND_ORDER // OpenGL style
    Float sz = (farPlane + nearPlane) / clip;
    Float pz = 2.0 * farPlane * nearPlane / clip;
    Float w = -1.0;
#else // DirectX style
    Float sz = -(farPlane + nearPlane) / clip;
    Float pz = 2.0 * farPlane * nearPlane / clip;
    Float w = 1.0;
#endif

    return Matrix4x4(
        sx, 0.0, 0.0, 0.0,
        0.0, sy, 0.0, 0.0,
        0.0, 0.0, sz, w,
        0.0, 0.0, pz, 0.0
    );
}
inline Matrix4x4 Matrix4x4::orthography(Float lf, Float rt, Float bt, Float tp, Float nr, Float fr)
{
    return Matrix4x4(
        2.0 / (lf - rt), 0.0, 0.0, 0.0,
        0.0, 2.0 / (tp - bt), 0.0, 0.0,
        0.0, 0.0, 2.0 / (fr - nr), 0.0,
        (rt + lf) / (rt - lf), (tp + bt) / (tp - bt), (nr + fr) / (fr - nr), 1.0
    );
}

inline Matrix4x4 Matrix4x4::translate(const Vector3f &vec)
{
    return translate(vec.x, vec.y, vec.z);
}

inline Matrix4x4 Matrix4x4::translate(Float tx, Float ty, Float tz)
{
    return Matrix4x4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        tx, ty, tz, 1.0
    );
}

inline Matrix4x4 Matrix4x4::RotateX(Float theta)
{
    Matrix4x4 ret;

    theta = degreeToRadian(theta);
    Float costh = cos(theta);
    Float sinth = sin(theta);

    ret.mtx[0][0] = ret.mtx[3][3] = 1.0;
    ret.mtx[1][1] = ret.mtx[2][2] = costh;
    ret.mtx[1][2] = sinth;
    ret.mtx[2][1] = -sinth;

    return ret;
}

inline Matrix4x4 Matrix4x4::RotateY(Float theta)
{
    Matrix4x4 ret;

    theta = degreeToRadian(theta);
    Float costh = cos(theta);
    Float sinth = sin(theta);

    ret.mtx[1][1] = ret.mtx[3][3] = 1.0;
    ret.mtx[0][0] = ret.mtx[2][2] = costh;
    ret.mtx[2][0] = sinth;
    ret.mtx[0][2] = -sinth;

    return ret;
}

inline Matrix4x4 Matrix4x4::RotateZ(Float theta)
{
    Matrix4x4 ret;

    theta = degreeToRadian(theta);
    Float costh = cos(theta);
    Float sinth = sin(theta);

    ret.mtx[2][2] = ret.mtx[3][3] = 1.0;
    ret.mtx[0][0] = ret.mtx[1][1] = costh;
    ret.mtx[0][1] = sinth;
    ret.mtx[1][0] = -sinth;

    return ret;
}

inline Matrix4x4 Matrix4x4::rotate(Float alpha, Float beta, Float gamma)
{
    Matrix4x4 ret;
    alpha = degreeToRadian(alpha);
    beta = degreeToRadian(beta);
    gamma = degreeToRadian(gamma);
    Float cosA = cos(alpha), sinA = sin(alpha);
    Float cosB = cos(beta), sinB = sin(beta);
    Float cosG = cos(gamma), sinG = sin(gamma);

    ret.mtx[0][0] = cosA * cosG - cosB * sinA * sinG;
    ret.mtx[0][1] = -cosB * cosG * sinA - cosA * sinG;
    ret.mtx[0][2] = sinA * sinB;
    ret.mtx[1][0] = cosG * sinA + cosA * cosB * sinG;
    ret.mtx[1][1] = cosA * cosB * cosG - sinA * sinG;
    ret.mtx[1][2] = -cosA * sinB;
    ret.mtx[2][0] = sinB * sinG;
    ret.mtx[2][1] = sinB * cosG;
    ret.mtx[2][2] = cosB;
    ret.mtx[3][3] = 1.0;

    return ret;
}

inline Matrix4x4 Matrix4x4::rotate(const Vector3f &axis, Float theta, bool isNormalized)
{
    Vector3f u = isNormalized ? axis : normalize(axis);
    Float rad = degreeToRadian(theta);
    Float c = cos(rad);
    Float s = sin(rad);
    Float t = 1 - c;

    Matrix4x4 ret;

    ret.mtx[0][0] = t * sqr(u.x) + c;
    ret.mtx[0][1] = t * u.x * u.y - s * u.x;
    ret.mtx[0][2] = t * u.x * u.z + s * u.y;
    ret.mtx[0][3] = 0.0;

    ret.mtx[1][0] = t * u.x * u.y + s * u.z;
    ret.mtx[1][1] = t * sqr(u.y) + c;
    ret.mtx[1][2] = t * u.y * u.z - s * u.x;
    ret.mtx[1][3] = 0.0;

    ret.mtx[2][0] = t * u.x * u.z - s * u.y;
    ret.mtx[2][1] = t * u.y * u.z + s * u.x;
    ret.mtx[2][2] = t * sqr(u.z) + c;
    ret.mtx[2][3] = 0.0;

    ret.mtx[3][0] = 0.0;
    ret.mtx[3][1] = 0.0;
    ret.mtx[3][2] = 0.0;
    ret.mtx[3][3] = 1.0;
    return ret;
}

inline Matrix4x4 Matrix4x4::scale(Float sx, Float sy, Float sz)
{
    return Matrix4x4(
        sx, 0.0, 0.0, 0.0,
        0.0, sy, 0.0, 0.0,
        0.0, 0.0, sz, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

inline Matrix4x4 Matrix4x4::scale(Float scale)
{
    return Matrix4x4(
        scale, 0.0, 0.0, 0.0,
        0.0, scale, 0.0, 0.0,
        0.0, 0.0, scale, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}
/*
inline void Matrix4x4::Shear(Vector3D &vec)
{
    setIdentify();
    mtx[0][1] = vec.x;
    mtx[1][0] = vec.y;
    Determinant();
}
inline void Matrix4x4::Reflect(Vector3D &vec)
{
    // vec is a vector in the direction of the line
    mtx[0][0] = vec.x * vec.x - vec.y * vec.y;	mtx[0][1] = 2 * vec.x * vec.y;
    mtx[1][0] = 2 * vec.x * vec.y;	mtx[1][1] = vec.y * vec.y - vec.x * vec.x;
    mtx[2][2] = 1;
    Determinant();
}
inline void Matrix4x4::Perspective(Vector3D &vPnt)
{
    setIdentify();
    mtx[2][0] = 1.0 / vPnt.x; mtx[2][1] = 1.0 / vPnt.y;
}
*/

}
