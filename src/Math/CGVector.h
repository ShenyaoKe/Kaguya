//
//  CGVector.h
//
//  Created by Shenyao Ke on 9/28/14.
//  Copyright (c) 2014 AKIKA. All rights reserved.
//
#pragma once
#ifndef __CGVector__
#define __CGVector__

//#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
//#endif // !KAGUYA_DOUBLE_AS_FLOAT

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "Math/MathUtil.h"
using namespace std;

class Vector2D
{
public:
    Float x, y;
    
	Vector2D() :x(0), y(0){}
	virtual ~Vector2D(){}
	Vector2D(const Vector2D &vec) : x(vec.x), y(vec.y){}
	Vector2D(Float posX, Float posY) :x(posX), y(posY){}
    //Vector2D zero(){ x = 0; y = 0; }
	virtual void printInfo() const{ cout << "x:" << x << "\ty:" << y << endl; }
	
	Vector2D operator-() const{ return Vector2D(-x, -y); }
	Vector2D operator+(const Vector2D& vec) const{ return Vector2D(x + vec.x, y + vec.y); }
	Vector2D operator-(const Vector2D& vec) const{ return Vector2D(x - vec.x, y - vec.y); }
	Float operator*(const Vector2D& vec) const{ return x * vec.x + y * vec.y; }
	Vector2D operator*(Float n) const{ return Vector2D(x * n, y * n); }
	Vector2D operator/(Float n) const{ return Vector2D(x / n, y / n); }
	Vector2D& operator += (const Vector2D& vec);
	Vector2D& operator -= (const Vector2D& vec);
	Float operator[](int i) const{ assert(i >= 0 && i <= 1); (&x)[i]; }
    Float dotMul(const Vector2D& vec) const;
	//virtual Vector2D crossMul(Vector2D& v2) const;
	friend Vector2D operator*(Float n, const Vector2D& vec){ return vec * n; }

	virtual void getData(Float* &data) const;
	virtual Float getDist(const Vector2D& v) const;
	virtual Float getDistSq(const Vector2D& v) const;
	virtual Float getLength() const;
	virtual Float getLenSq() const;
	virtual Float getRadian() const;
	virtual Float getTheta() const;
    virtual void normalize();

	Vector2D getPerpendicular() const{ return Vector2D(y, -x); }
	Vector2D getNorm() const;
	friend Vector2D Normalize(const Vector2D &vec);
	friend Float Cross(const Vector2D& vec0, const Vector2D &vec1);
};

class Vector3D// :public Vector2D
{
public:
	Float x, y, z;

	Vector3D() :x(0), y(0), z(0){}
	~Vector3D(){}
	Vector3D(const Vector3D &vec) : x(vec.x), y(vec.y), z(vec.z){}
	Vector3D(Float posX, Float posY, Float posZ) :x(posX), y(posY), z(posZ){}
	Vector3D(const Vector2D& v0, Float z0) :x(v0.x), y(v0.y), z(z0){}
	//Vector3D zero();
	Vector3D operator-() const;
	Vector3D operator+(const Vector3D& vec) const;
	Vector3D operator-(const Vector3D& vec) const;
	Float operator*(const Vector3D& vec) const;
	Vector3D operator*(Float n) const;
	Vector3D operator/(Float n) const; 
	Vector3D& operator = (const Vector2D& vec);
	Vector3D& operator += (const Vector3D& vec);
	Vector3D& operator -= (const Vector3D& vec);
	Float operator[](int i) const;
	Float& operator[](int i);
	Float dotMul(const Vector3D& vec) const;
	Vector3D crossMul(const Vector3D& vec) const;

	friend Vector3D operator*(Float n, const Vector3D& vec){ return vec * n; }
	friend ostream& operator << (ostream& os, const Vector3D& vec);
	void printInfo() const{ cout << "x:" << x << "\ty:" << y << "\tz:" << z << endl; }
	void getData(Float* data) const;
	Float getLength() const;
	Float getLenSq() const;
	void normalize();
	friend Vector3D Normalize(const Vector3D &vec);
	friend Vector3D Cross(const Vector3D& vec0, const Vector3D &vec1);
};
class Vector4D//:public Vector3D
{
public:
	Float x, y, z, w;// = 0;

	Vector4D(){}
	~Vector4D(){}
	Vector4D(const Vector4D &vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w){}
	Vector4D(Float posX, Float posY, Float posZ, Float posW) :x(posX), y(posY), z(posZ), w(posW){}
	Vector4D(const Vector3D& vec, Float posW) :x(vec.x), y(vec.y), z(vec.z), w(posW){}

	Vector4D operator-() const;
	Vector4D operator+(const Vector4D& vec) const;
	Vector4D operator-(const Vector4D& vec) const;
	Float operator*(const Vector4D& vec) const;
	Vector4D operator*(Float n) const;
	Vector4D operator/(Float n) const;

	Vector4D& operator = (const Vector3D& vec);
	Vector4D& operator += (const Vector4D& vec);
	Vector4D& operator -= (const Vector4D& vec);
	Vector4D& operator *= (Float n);
	Vector4D& operator /= (Float n);
	Float dotMul(const Vector4D& vec) const;
	//Vector4D crossMul(const Vector4D& vec) const;
	void getData(Float* data) const;
	Float getLength() const;
	Float getLenSq() const;
	Vector4D getNorm() const;
	void normalize();

protected:
private:
};
/*
class Versor
{
public:
	Float q[4] = {};

	Versor(){}
	~Versor(){}

	Versor operator+ (const Versor& rhs);
	Versor operator- (const Versor& rhs);
	Versor operator* (const Versor& rhs);
	Versor operator/ (Float n);
	Versor operator* (Float n);

	void quat_from_degree(const Vector3D& vec, Float deg);
	void quat_from_radian(const Vector3D& vec, Float rad);
protected:
	
private:
};*/
/************************************************************************/
/* Vector2D Definition                                                  */
/************************************************************************/
inline Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	*this = *this + vec;
	return *this;
}
inline Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	*this = *this + vec;
	return *this;
}
inline Float Vector2D::dotMul(const Vector2D& vec) const
{
    return (x * vec.x + y * vec.y);
}
inline Float Vector2D::getDist(const Vector2D& vec) const
{
    return (Float)sqrt((this->x - vec.x) * (this->x - vec.x) + (this->y - vec.y) * (this->y - vec.y));
}
inline Float Vector2D::getDistSq(const Vector2D& vec) const
{
    return (Float)((x - vec.x) * (x - vec.x) + (y - vec.y) * (y - vec.y));
}
/*
Vector2D Vector2D::crossMul(Vector2D v2)
{
    Vector2D vNorm;
    vNorm.x = y * v2.z - z * v2.y;
    vNorm.y = z * v2.x - x * v2.z;
    return vNorm;
}*/
inline void Vector2D::getData(Float* &data) const
{
	data[0] = x;
	data[1] = y;
}
inline Float Vector2D::getLength() const
{
	return (Float)sqrt(x * x + y * y);
}
inline Float Vector2D::getLenSq() const
{
	return x * x + y * y;
}
inline Float Vector2D::getTheta() const
{
    if (getLength() == 0) {
        return 0;
    } else {
        if (y < 0) {
            return 180 * (2 * M_PI - acos(x / getLength())) / M_PI;
        } else {
            return 180 * acos(x / getLength()) / M_PI;
        }
    }
}
inline Float Vector2D::getRadian() const
{
	if (getLength() == 0) {
		return 0;
	}
	else {
		if (y < 0) {
			return 2 * M_PI - acos(x / getLength());
		}
		else {
			return acos(x / getLength());
		}
	}
}
inline void Vector2D::normalize()
{
	Float len = getLength();
	x /= len;
	y /= len;
}
inline Vector2D Vector2D::getNorm() const
{
	Float len = getLength();
	return Vector2D(x / len, y / len);
}
inline Vector2D Normalize(const Vector2D &vec)
{
	Float rvsLen = 1.0 / vec.getLength();
	return vec * rvsLen;
}

inline Float Cross(const Vector2D& vec0, const Vector2D &vec1)
{
	return vec0.x * vec1.y - vec1.x * vec0.y;
}
/************************************************************************/
/* Vector3D Definition                                                  */
/************************************************************************/
inline Vector3D Vector3D::operator-() const
{
	Vector3D ret = Vector3D(-x, -y, -z);
	return ret;
}
inline Vector3D Vector3D::operator+(const Vector3D& vec) const
{
	Vector3D ret = Vector3D(x + vec.x, y + vec.y, z + vec.z);
	return ret;
}
inline Vector3D Vector3D::operator-(const Vector3D& vec) const
{
	Vector3D ret = Vector3D(x - vec.x, y - vec.y, z - vec.z);
	return ret;
}
inline Float Vector3D::operator*(const Vector3D& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}
inline Vector3D Vector3D::operator*(Float n) const
{
	Vector3D ret = Vector3D(this->x * n, this->y * n, this->z * n);
	return ret;
}
inline Vector3D Vector3D::operator/(Float n) const
{
	n = 1.0 / n;
	Vector3D ret = Vector3D(x * n, y * n, z * n);
	return ret;
}
inline Vector3D& Vector3D::operator=(const Vector2D& vec)
{
	*this = Vector3D(vec, 0);
	return *this;
}
inline Vector3D& Vector3D::operator += (const Vector3D& vec)
{
	*this = *this + vec;
	return *this;
}
inline Vector3D& Vector3D::operator -= (const Vector3D& vec)
{
	*this = *this - vec;
	return *this;
}
inline Float Vector3D::operator[](int i) const
{
	assert(i >= 0 && i <= 2);
	return (&x)[i];
}
inline Float& Vector3D::operator[](int i)
{
	assert(i >= 0 && i <= 2);
	return (&x)[i];
}
inline Float Vector3D::dotMul(const Vector3D& vec) const
{
	return (x * vec.x + y * vec.y + z * vec.z);
}
inline Vector3D Vector3D::crossMul(const Vector3D& vec) const
{
	Vector3D vNorm;
	vNorm.x = y * vec.z - z * vec.y;
	vNorm.y = z * vec.x - x * vec.z;
	vNorm.z = x * vec.y - y * vec.x;
	return vNorm;
}
//inline void Vector3D::getData(Float* &data) const
// {
// 	data[0] = x;
// 	data[1] = y;
// 	data[2] = z;
// }
inline Float Vector3D::getLength() const
{
	return sqrt(x*x + y*y + z*z);
}
inline Float Vector3D::getLenSq() const
{
	return x * x + y * y + z * z;
}

inline void Vector3D::normalize()
{
	Float rvsLen = 1.0 / getLength();
	x *= rvsLen;
	y *= rvsLen;
	z *= rvsLen;
}

inline Vector3D Normalize(const Vector3D &vec)
{
	Float rvsLen = 1.0 / vec.getLength();
	return vec * rvsLen;
}

inline Vector3D Cross(const Vector3D& vec0, const Vector3D &vec1)
{
	return Vector3D(
		vec0.y * vec1.z - vec0.z * vec1.y,
		vec0.z * vec1.x - vec0.x * vec1.z,
		vec0.x * vec1.y - vec0.y * vec1.x
		);
}
/************************************************************************/
/* Vector4D Definition                                                  */
/************************************************************************/
inline Vector4D Vector4D::operator-() const
{
	return Vector4D(-x, -y, -z, -w);
}
inline Vector4D Vector4D::operator+(const Vector4D& vec) const
{
	return Vector4D(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
}
inline Vector4D Vector4D::operator-(const Vector4D& vec) const
{
	return Vector4D(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
}
inline Float Vector4D::operator*(const Vector4D& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
}
inline Vector4D Vector4D::operator*(Float n) const
{
	return Vector4D(x * n, y * n, z * n, w * n);
}
inline Vector4D Vector4D::operator/(Float n) const
{
	return Vector4D(x / n, y / n, z / n, w / n);
}

inline Vector4D& Vector4D::operator=(const Vector3D& vec)
{
	*this = Vector4D(vec, 0.0);
	return *this;
}

inline Vector4D& Vector4D::operator += (const Vector4D& vec)
{
	*this = *this + vec;
	return *this;
}
inline Vector4D& Vector4D::operator -= (const Vector4D& vec)
{
	*this = *this - vec;
	return *this;
}
inline Vector4D& Vector4D::operator*=(Float n)
{
	*this = *this * n;
	return *this;
}
inline Vector4D& Vector4D::operator/=(Float n)
{
	*this = *this / n;
	return *this;
}
inline ostream& operator << (ostream& os, const Vector3D& vec)
{
	os << vec.x << ", " << vec.y << ", " << vec.z;
	return os;
}
/*
inline Float Vector4D::dotMul(const Vector4D& vec) const
{
	return (x * vec.x + y * vec.y + z * vec.z + w * vec.w);
}*/
// inline Vector4D Vector4D::crossMul(Vector4D& v2) const
// {
// 	Vector4D vNorm;
// 	vNorm.x = y * v2.z - z * v2.y;
// 	vNorm.y = z * v2.x - x * v2.z;
// 	vNorm.z = x * v2.y - y * v2.x;
// 	return vNorm;
// }

// inline void Vector4D::getData(Float* &data) const
// {
// 	data[0] = x;
// 	data[1] = y;
// 	data[2] = z;
// 	data[3] = w;
// }
inline Float Vector4D::getLength() const
{
	return (Float)sqrt(x*x + y*y + z*z + w*w);
}
inline Vector4D Vector4D::getNorm() const
{
	Float rvsLen = 1.0 / getLength();
	return (*this) * rvsLen;
}
inline void Vector4D::normalize()
{
	Float invLen = 1.0 / getLength();
	x *= invLen;
	y *= invLen;
	z *= invLen;
	w *= invLen;
}

const Vector2D X_AXIS2D(1, 0);
const Vector2D Y_AXIS2D(0, 1);
const Vector3D X_AXIS3D(1, 0, 0);
const Vector3D Y_AXIS3D(0, 1, 0);
const Vector3D Z_AXIS3D(0, 0, 1);
typedef Vector2D Point2D;
typedef Vector3D Point3D;
static Point3D NEG_INF3D(-INFINITY, -INFINITY, -INFINITY);
static Point3D INF3D(INFINITY, INFINITY, INFINITY);

#endif /* defined(__CGVector__) */