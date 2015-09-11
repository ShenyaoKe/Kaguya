//
//  CGVector.h
//
//  Created by Shenyao Ke on 9/28/14.
//  Copyright (c) 2014 AKIKA. All rights reserved.
//
#pragma once
#ifndef __CGVector__
#define __CGVector__

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "Math/MathUtil.h"
using namespace std;

class Vector2D
{
public:
    double x, y;
    
	Vector2D() :x(0), y(0){}
	virtual ~Vector2D(){}
	Vector2D(double posX, double posY) :x(posX), y(posY){};
    //Vector2D zero(){ x = 0; y = 0; }
	virtual void printInfo() const{ cout << "x:" << x << "\ty:" << y << endl; }
	
	Vector2D operator-() const{ return Vector2D(-x, -y); }
	Vector2D operator+(const Vector2D& vec) const{ return Vector2D(x + vec.x, y + vec.y); }
	Vector2D operator-(const Vector2D& vec) const{ return Vector2D(x - vec.x, y - vec.y); }
	double operator*(const Vector2D& vec) const{ return x * vec.x + y * vec.y; }
	Vector2D operator*(double n) const{ return Vector2D(x * n, y * n); }
	Vector2D operator/(double n) const{ return Vector2D(x / n, y / n); }
	Vector2D& operator += (const Vector2D& vec);
	Vector2D& operator -= (const Vector2D& vec);
	double operator[](int i) const{ assert(i >= 0 && i <= 1); (&x)[i]; }
    double dotMul(const Vector2D& vec) const;
	//virtual Vector2D crossMul(Vector2D& v2) const;
	friend Vector2D operator*(double n, const Vector2D& vec){ return vec * n; }

	virtual void getData(double* &data) const;
	virtual double getDist(const Vector2D& v) const;
	virtual double getDistSq(const Vector2D& v) const;
	virtual double getLength() const;
	virtual double getLenSq() const;
	virtual double getRadian() const;
	virtual double getTheta() const;
    virtual void normalize();

	Vector2D getPerpendicular() const{ return Vector2D(y, -x); }
	Vector2D getNorm() const;
};

class Vector3D// :public Vector2D
{
public:
	double x, y, z;

	Vector3D() :x(0), y(0), z(0){}
	~Vector3D(){}
	Vector3D(double posX, double posY, double posZ) :x(posX), y(posY), z(posZ){}
	Vector3D(const Vector2D& v0, double z0) :x(v0.x), y(v0.y), z(z0){}
	//Vector3D zero();
	Vector3D operator-() const;
	Vector3D operator+(const Vector3D& vec) const;
	Vector3D operator-(const Vector3D& vec) const;
	double operator*(const Vector3D& vec) const;
	Vector3D operator*(double n) const;
	Vector3D operator/(double n) const; 
	Vector3D& operator = (const Vector2D& vec);
	Vector3D& operator += (const Vector3D& vec);
	Vector3D& operator -= (const Vector3D& vec);
	double operator[](int i) const;
	double& operator[](int i);
	double dotMul(const Vector3D& vec) const;
	Vector3D crossMul(const Vector3D& vec) const;

	friend Vector3D operator*(double n, const Vector3D& vec){ return vec * n; }
	void printInfo() const{ cout << "x:" << x << "\ty:" << y << "\tz:" << z << endl; }
	void getData(double* data) const;
	double getLength() const;
	double getLenSq() const;
	void normalize();
	friend Vector3D Normalize(const Vector3D &vec);
	friend Vector3D Cross(const Vector3D& vec0, const Vector3D &vec1);
};
class Vector4D//:public Vector3D
{
public:
	double x, y, z, w;// = 0;

	Vector4D(){}
	~Vector4D(){}
	Vector4D(double posX, double posY, double posZ, double posW) :x(posX), y(posY), z(posZ), w(posW){}
	Vector4D(const Vector3D& vec, double posW) :x(vec.x), y(vec.y), z(vec.z), w(posW){}

	Vector4D operator-() const;
	Vector4D operator+(const Vector4D& vec) const;
	Vector4D operator-(const Vector4D& vec) const;
	double operator*(const Vector4D& vec) const;
	Vector4D operator*(double n) const;
	Vector4D operator/(double n) const;

	Vector4D& operator = (const Vector3D& vec);
	Vector4D& operator += (const Vector4D& vec);
	Vector4D& operator -= (const Vector4D& vec);
	Vector4D& operator *= (double n);
	Vector4D& operator /= (double n);
	double dotMul(const Vector4D& vec) const;
	//Vector4D crossMul(const Vector4D& vec) const;
	void getData(double* data) const;
	double getLength() const;
	double getLenSq() const;
	Vector4D getNorm() const;
	void normalize();

protected:
private:
};
class Versor
{
public:
	double q[4] = {};

	Versor(){}
	~Versor(){}

	Versor operator+ (const Versor& rhs);
	Versor operator- (const Versor& rhs);
	Versor operator* (const Versor& rhs);
	Versor operator/ (double n);
	Versor operator* (double n);

	void quat_from_degree(const Vector3D& vec, double deg);
	void quat_from_radian(const Vector3D& vec, double rad);
protected:
	
private:
};
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
inline double Vector2D::dotMul(const Vector2D& vec) const
{
    return (x * vec.x + y * vec.y);
}
inline double Vector2D::getDist(const Vector2D& vec) const
{
    return (double)sqrt((this->x - vec.x) * (this->x - vec.x) + (this->y - vec.y) * (this->y - vec.y));
}
inline double Vector2D::getDistSq(const Vector2D& vec) const
{
    return (double)((x - vec.x) * (x - vec.x) + (y - vec.y) * (y - vec.y));
}
/*
Vector2D Vector2D::crossMul(Vector2D v2)
{
    Vector2D vNorm;
    vNorm.x = y * v2.z - z * v2.y;
    vNorm.y = z * v2.x - x * v2.z;
    return vNorm;
}*/
inline void Vector2D::getData(double* &data) const
{
	data[0] = x;
	data[1] = y;
}
inline double Vector2D::getLength() const
{
	return (double)sqrt(x * x + y * y);
}
inline double Vector2D::getLenSq() const
{
	return x * x + y * y;
}
inline double Vector2D::getTheta() const
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
inline double Vector2D::getRadian() const
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
	double len = getLength();
	x /= len;
	y /= len;
}
inline Vector2D Vector2D::getNorm() const
{
	double len = getLength();
	return Vector2D(x / len, y / len);
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
inline double Vector3D::operator*(const Vector3D& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}
inline Vector3D Vector3D::operator*(double n) const
{
	Vector3D ret = Vector3D(this->x * n, this->y * n, this->z * n);
	return ret;
}
inline Vector3D Vector3D::operator/(double n) const
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
inline double Vector3D::operator[](int i) const
{
	assert(i >= 0 && i <= 2);
	return (&x)[i];
}
inline double& Vector3D::operator[](int i)
{
	assert(i >= 0 && i <= 2);
	return (&x)[i];
}
inline double Vector3D::dotMul(const Vector3D& vec) const
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
//inline void Vector3D::getData(double* &data) const
// {
// 	data[0] = x;
// 	data[1] = y;
// 	data[2] = z;
// }
inline double Vector3D::getLength() const
{
	return sqrt(x*x + y*y + z*z);
}
inline double Vector3D::getLenSq() const
{
	return x * x + y * y + z * z;
}

inline void Vector3D::normalize()
{
	double rvsLen = 1.0 / getLength();
	x *= rvsLen;
	y *= rvsLen;
	z *= rvsLen;
}

inline Vector3D Normalize(const Vector3D &vec)
{
	double rvsLen = 1.0 / vec.getLength();
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
inline double Vector4D::operator*(const Vector4D& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
}
inline Vector4D Vector4D::operator*(double n) const
{
	return Vector4D(x * n, y * n, z * n, w * n);
}
inline Vector4D Vector4D::operator/(double n) const
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
inline Vector4D& Vector4D::operator*=(double n)
{
	*this = *this * n;
	return *this;
}
inline Vector4D& Vector4D::operator/=(double n)
{
	*this = *this / n;
	return *this;
}
/*
inline double Vector4D::dotMul(const Vector4D& vec) const
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

// inline void Vector4D::getData(double* &data) const
// {
// 	data[0] = x;
// 	data[1] = y;
// 	data[2] = z;
// 	data[3] = w;
// }
inline double Vector4D::getLength() const
{
	return (double)sqrt(x*x + y*y + z*z + w*w);
}
inline Vector4D Vector4D::getNorm() const
{
	double rvsLen = 1.0 / getLength();
	return (*this) * rvsLen;
}
inline void Vector4D::normalize()
{
	double invLen = 1.0 / getLength();
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