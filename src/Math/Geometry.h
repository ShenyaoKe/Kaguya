#if defined(_MSC_VER)
#pragma once
#endif
#ifndef KAGUYA_GEOMETRY_H
#define KAGUYA_GEOMETRY_H

#include "Core/Kaguya.h"

template <typename T>
inline bool isNaN(const T x)
{
	return std::isnan(x);
}
inline bool isNaN(const int x)
{
	return false;
}
/************************************************************************/
/* Vector                                                               */
/************************************************************************/
template <typename T>
class Vector3
{
public:

	Vector3() { x = y = z = 0; }
	Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	Vector3(T val[3]) : x(val[0]), y(val[1]), z(val[2]) {}
	explicit Vector3(const Point3<T> &p);
	explicit Vector3(const Normal3<T> &p);

	bool hasNaN() const { return isNaN(x) || isNaN(y) || isNaN(z); }
	
	T operator[](int i) const {
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	T &operator[](int i) {
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	Vector3<T> operator+(const Vector3<T> &v) const {
		Assert(!v.hasNaN());
		return Vector3(x + v.x, y + v.y, z + v.z;);
	}
	Vector3<T> &operator+=(const Vector3<T> &v) {
		Assert(!v.hasNaN());
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector3<T> operator-(const Vector3<T> &v) const {
		Assert(!v.hasNaN());
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3<T> &operator-=(const Vector3<T> &v) {
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vector3<T> operator-() const {
		return Vector3(-x, -y, -z);
	}
	bool operator==(const Vector3<T> &v) const {
		return x == v.x && y == v.y && z == v.z;
	}
	bool operator!=(const Vector3<T> &v) const {
		return x != v.x || y != v.y || z != v.z;
	}

	template <typename U>
	Vector3<T> operator*(U s) const {
		return Vector3(x * s, y * s, z * s);
	}
	template <typename U>
	Vector3<T> &operator*=(U s) {
		Assert(!isNaN(f));
		x *= s; y *= s; z *= s;
		return *this;
	}
	template <typename U>
	Vector3<T> operator/(U f) const {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		return Vector3<T>(x * inv, y * inv, z * inv);
	}
	template <typename U>
	Vector3<T> &operator/=(U f) {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	Float lengthSqared() const { return x * x + y * y + z * z; }
	Float length() const { return std::sqrt(lengthSqared()); }
	void normalize() { *this /= length(); }

	friend std::ostream &operator<<(ostream& os, const Vector3<T> &v)
	{
		os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return os;
	}
public:
	T x, y, z;
};

typedef Vector3<Float> Vector3f;
typedef Vector3<int> Vector3i;

/************************************************************************/
/* Point                                                                */
/************************************************************************/
template <typename T>
class Point3
{
public:
	Point3() { x = y = z = 0; }
	Point3(T x, T y, T z) : x(x), y(y), z(z) {}
	Point3(T val[3]) : x(val[0]), y(val[1]), z(val[2]) {}

	bool hasNaN() const { return isNaN(x) || isNaN(y) || isNaN(z); }

	T operator[](int i) const {
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	T &operator[](int i) {
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	Point3<T> operator+(const Point3<T> &p) const {
		Assert(!p.hasNaN());
		return Point3(x + p.x, y + p.y, z + p.z;);
	}
	Point3<T> &operator+=(const Point3<T> &p) {
		Assert(!p.hasNaN());
		x += p.x; y += p.y; z += p.z;
		return *this;
	}
	Point3<T> operator+(const Vector3<T> &v) const {
		Assert(!v.hasNaN());
		return Point3(x + v.x, y + v.y, z + v.z);
	}
	Point3<T> &operator+=(const Vector3<T> &v) {
		Assert(!v.hasNaN());
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector3<T> operator-(const Point3<T> &p) const {
		Assert(!p.hasNaN());
		return Vector3(x - p.x, y - p.y, z - p.z);
	}
	Vector3<T> operator-(const Vector3<T> &v) const {
		Assert(!v.hasNaN());
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Point3<T> &operator-=(const Vector3<T> &v) {
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Point3<T> operator-() const {
		return Point3(-x, -y, -z);
	}
	bool operator==(const Point3<T> &p) const {
		return x == p.x && y == p.y && z == p.z;
	}
	bool operator!=(const Point3<T> &p) const {
		return x != p.x || y != p.y || z != p.z;
	}

	template <typename U>
	Point3<T> operator*(U s) const {
		return Point3(x * s, y * s, z * s);
	}
	template <typename U>
	Point3<T> operator*=(U s) const {
		Assert(!isNaN(f));
		x *= s; y *= s; z *= s;
		return *this;
	}
	template <typename U>
	Point3<T> operator/(U f) const {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		return Point3<T>(x * inv, y * inv, z * inv);
	}
	template <typename U>
	Point3<T> operator/=(U f) const {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	friend std::ostream &operator<<(ostream& os, const Point3<T> &v)
	{
		os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
		return os;
	}
public:
	T x, y, z;
};

typedef Point3<Float> Point3f;
typedef Point3<int> Point3i;

/************************************************************************/
/* Normal                                                               */
/************************************************************************/
template <typename T>
class Normal3
{
public:
	Normal3() { x = y = z = 0; }
	Normal3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	Normal3(T val[3]) : x(val[0]), y(val[1]), z(val[2]) {}
	explicit Normal3(const Vector3<T> &vec) : x(vec.x), y(vec.y), z(vec.z) {}
	
	bool hasNaN() const { return isNaN(x) || isNaN(y) || isNaN(z); }

	T operator[](int i) const {
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	T &operator[](int i) {
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	Normal3<T> operator+(const Normal3<T> &v) const {
		Assert(!v.hasNaN());
		return Normal3(x + v.x, y + v.y, z + v.z;);
	}
	Normal3<T> &operator+=(const Normal3<T> &v) {
		Assert(!v.hasNaN());
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Normal3<T> operator-(const Normal3<T> &v) const {
		Assert(!v.hasNaN());
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Normal3<T> &operator-=(const Normal3<T> &v) {
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Normal3<T> operator-() const {
		return Normal3(-x, -y, -z);
	}
	bool operator==(const Normal3<T> &v) const {
		return x == v.x && y == v.y && z == v.z;
	}
	bool operator!=(const Normal3<T> &v) const {
		return x != v.x || y != v.y || z != v.z;
	}

	template <typename U>
	Normal3<T> operator*(U s) const {
		return Normal3(x * s, y * s, z * s);
	}
	template <typename U>
	Normal3<T> &operator*=(U s) {
		Assert(!isNaN(f));
		x *= s; y *= s; z *= s;
		return *this;
	}
	template <typename U>
	Normal3<T> operator/(U f) const {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		return Normal3<T>(x * inv, y * inv, z * inv);
	}
	template <typename U>
	Normal3<T> &operator/=(U f) {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	Float lengthSqared() const { return x * x + y * y + z * z; }
	Float length() const { return std::sqrt(lengthSqared()); }
	void normalize() { *this /= length(); }


	friend std::ostream &operator<<(ostream& os, const Normal3<T> &v) {
		os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
		return os;
	}
public:
	T x, y, z;
};
typedef Normal3<Float> Normal3f;

/************************************************************************/
/* Vector Implementation                                                */
/************************************************************************/
template<typename T>
inline Vector3<T>::Vector3(const Point3<T>& p)
	: x(p.x), y(p.y), z(p.z)
{
}

template<typename T>
inline Vector3<T>::Vector3(const Normal3<T>& n)
	: x(n.x), y(n.y), z(n.z)
{
}

template <typename T>
inline T Dot(const Vector3<T> &v1, const Vector3<T> &v2)
{
	Assert(!v.hasNaN() && !n.hasNaN());
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template <typename T>
inline T Dot(const Vector3<T> &v, const Normal3<T> &n)
{
	Assert(!v.hasNaN() && !n.hasNaN());
	return v.x * n.x + v.y * n.y + v.z * n.z;
}

template <typename T>
inline T Dot(const Normal3<T> &n, const Vector3<T> &v)
{
	Assert(!v.hasNaN() && !n.hasNaN());
	return v.x * n.x + v.y * n.y + v.z * n.z;
}

template <typename T>
inline Vector3<T> Normalize(const Vector3<T> &v)
{
	return v / v.length();
}
template <typename T>
inline Normal3<T> Normalize(const Normal3<T> &n)
{
	return n / n.length();
}
#endif // KAGUYA_GEOMETRY_H