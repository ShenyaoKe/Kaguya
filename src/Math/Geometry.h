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
class Vector2
{
	Vector2() { x = y = 0; }
	Vector2(T _x, T _y) : x(_x), y(_y) {}
	Vector2(T val[2]) : x(val[0]), y(val[1]) {}
	explicit Vector2(const Point2<T> &p);

	bool hasNaN() const { return isNaN(x) || isNaN(y); }

	T operator[](int i) const {
		Assert(i >= 0 && i <= 1);
		if (i == 0) return x;
		return y;
	}
	T &operator[](int i) {
		Assert(i >= 0 && i <= 1);
		if (i == 0) return x;
		return y;
	}
	Vector2<T> operator+(const Vector2<T> &v) const {
		Assert(!v.hasNaN());
		return Vector2<T>(x + v.x, y + v.y);
	}
	Vector2<T> &operator+=(const Vector2<T> &v) {
		Assert(!v.hasNaN());
		x += v.x; y += v.y;
		return *this;
	}
	Vector2<T> operator-(const Vector2<T> &v) const {
		Assert(!v.hasNaN());
		return Vector2<T>(x - v.x, y - v.y);
	}
	Vector2<T> &operator-=(const Vector2<T> &v) {
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vector2<T> operator-() const {
		return Vector2(-x, -y, -z);
	}
	bool operator==(const Vector2<T> &v) const {
		return x == v.x && y == v.y;
	}
	bool operator!=(const Vector2<T> &v) const {
		return x != v.x || y != v.y;
	}

	template <typename U>
	Vector2<T> operator*(U s) const {
		return Vector2(x * s, y * s);
	}
	template <typename U>
	Vector2<T> &operator*=(U s) {
		Assert(!isNaN(s));
		x *= s; y *= s;
		return *this;
	}
	template <typename U>
	Vector2<T> operator/(U f) const {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		return Vector2<T>(x * inv, y * inv);
	}
	template <typename U>
	Vector2<T> &operator/=(U f) {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv;
		return *this;
	}

	Float lengthSquared() const { return x * x + y * y; }
	Float length() const { return std::sqrt(lengthSquared()); }
	void normalize() { *this /= length(); }

	friend std::ostream &operator<<(ostream& os, const Vector2<T> &v) {
		os << "[ " << v.x << ", " << v.y << " ]";
		return os;
	}
public:
	T x, y;
};

typedef Vector2<Float> Vector2f;
typedef Vector2<int> Vector2i;

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
		return Vector3<T>(x + v.x, y + v.y, z + v.z);
	}
	Vector3<T> &operator+=(const Vector3<T> &v) {
		Assert(!v.hasNaN());
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector3<T> operator-(const Vector3<T> &v) const {
		Assert(!v.hasNaN());
		return Vector3<T>(x - v.x, y - v.y, z - v.z);
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
		return Vector3<T>(x * s, y * s, z * s);
	}
	template <typename U>
	Vector3<T> &operator*=(U s) {
		Assert(!isNaN(s));
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

	Float lengthSquared() const { return x * x + y * y + z * z; }
	Float length() const { return std::sqrt(lengthSquared()); }
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
class Point2
{
public:
	Point2() { x = y = 0; }
	Point2(T _x, T _y) : x(_x), y(_y) {}
	Point2(T val[2]) : x(val[0]), y(val[1]) {}

	bool hasNaN() const { return isNaN(x) || isNaN(y); }

	T operator[](int i) const {
		Assert(i >= 0 && i <= 1);
		if (i == 0) return x;
		return y;
	}
	T &operator[](int i) {
		Assert(i >= 0 && i <= 1);
		if (i == 0) return x;
		return y;
	}
	Point2<T> operator+(const Point2<T> &p) const {
		Assert(!p.hasNaN());
		return Point2<T>(x + p.x, y + p.y);
	}
	Point2<T> &operator+=(const Point2<T> &p) {
		Assert(!p.hasNaN());
		x += p.x; y += p.y;
		return *this;
	}
	Point2<T> operator+(const Vector3<T> &v) const {
		Assert(!v.hasNaN());
		return Point2<T>(x + v.x, y + v.y);
	}
	Point2<T> &operator+=(const Vector3<T> &v) {
		Assert(!v.hasNaN());
		x += v.x; y += v.y;
		return *this;
	}
	Vector3<T> operator-(const Point2<T> &p) const {
		Assert(!p.hasNaN());
		return Vector3<T>(x - p.x, y - p.y);
	}
	Vector3<T> operator-(const Vector3<T> &v) const {
		Assert(!v.hasNaN());
		return Vector3<T>(x - v.x, y - v.y);
	}
	Point2<T> &operator-=(const Vector3<T> &v) {
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y;
		return *this;
	}
	Point2<T> operator-() const {
		return Point2<T>(-x, -y, -z);
	}
	bool operator==(const Point2<T> &p) const {
		return x == p.x && y == p.y;
	}
	bool operator!=(const Point2<T> &p) const {
		return x != p.x || y != p.y;
	}

	template <typename U>
	Point2<T> operator*(U s) const {
		return Point2<T>(x * s, y * s);
	}
	template <typename U>
	Point2<T> operator*=(U s) const {
		Assert(!isNaN(s));
		x *= s; y *= s;
		return *this;
	}
	template <typename U>
	Point2<T> operator/(U f) const {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		return Point2<T>(x * inv, y * inv);
	}
	template <typename U>
	Point2<T> operator/=(U f) const {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv;
		return *this;
	}

	friend std::ostream &operator<<(ostream& os, const Point2<T> &p) {
		os << "[ " << p.x << ", " << p.y << " ]";
		return os;
	}
public:
	T x, y;
};

typedef Point2<Float> Point2f;
typedef Point2<int> Point2i;

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
		return Point3<T>(x + p.x, y + p.y, z + p.z);
	}
	Point3<T> &operator+=(const Point3<T> &p) {
		Assert(!p.hasNaN());
		x += p.x; y += p.y; z += p.z;
		return *this;
	}
	Point3<T> operator+(const Vector3<T> &v) const {
		Assert(!v.hasNaN());
		return Point3<T>(x + v.x, y + v.y, z + v.z);
	}
	Point3<T> &operator+=(const Vector3<T> &v) {
		Assert(!v.hasNaN());
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector3<T> operator-(const Point3<T> &p) const {
		Assert(!p.hasNaN());
		return Vector3<T>(x - p.x, y - p.y, z - p.z);
	}
	Vector3<T> operator-(const Vector3<T> &v) const {
		Assert(!v.hasNaN());
		return Vector3<T>(x - v.x, y - v.y, z - v.z);
	}
	Point3<T> &operator-=(const Vector3<T> &v) {
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Point3<T> operator-() const {
		return Point3<T>(-x, -y, -z);
	}
	bool operator==(const Point3<T> &p) const {
		return x == p.x && y == p.y && z == p.z;
	}
	bool operator!=(const Point3<T> &p) const {
		return x != p.x || y != p.y || z != p.z;
	}

	template <typename U>
	Point3<T> operator*(U s) const {
		return Point3<T>(x * s, y * s, z * s);
	}
	template <typename U>
	Point3<T> operator*=(U s) const {
		Assert(!isNaN(s));
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
	Point3<T> &operator/=(U f) {
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	friend std::ostream &operator<<(ostream& os, const Point3<T> &p)
	{
		os << "[ " << p.x << ", " << p.y << ", " << p.z << " ]";
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
		return Normal3<T>(x + v.x, y + v.y, z + v.z);
	}
	Normal3<T> &operator+=(const Normal3<T> &v) {
		Assert(!v.hasNaN());
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Normal3<T> operator-(const Normal3<T> &v) const {
		Assert(!v.hasNaN());
		return Normal3<T>(x - v.x, y - v.y, z - v.z);
	}
	Normal3<T> &operator-=(const Normal3<T> &v) {
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Normal3<T> operator-() const {
		return Normal3<T>(-x, -y, -z);
	}
	bool operator==(const Normal3<T> &v) const {
		return x == v.x && y == v.y && z == v.z;
	}
	bool operator!=(const Normal3<T> &v) const {
		return x != v.x || y != v.y || z != v.z;
	}

	template <typename U>
	Normal3<T> operator*(U s) const {
		return Normal3<T>(x * s, y * s, z * s);
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

	Float lengthSquared() const { return x * x + y * y + z * z; }
	Float length() const { return std::sqrt(lengthSquared()); }
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

template <typename T>
Vector2<T>::Vector2(const Point2<T> &p)
	: x(p.x), y(p.y)
{
}

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
inline Vector3<T> Cross(const Vector3<T> &v1, const Vector3<T> &v2)
{
	Assert(!v1.hasNaN() && !v2.hasNaN());
	return Vector3<T>(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
		);
}

template <typename T>
inline T Dot(const Vector2<T> &v1, const Vector2<T> &v2)
{
	Assert(!v1.hasNaN() && !v2.hasNaN());
	return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
inline T Dot(const Vector3<T> &v1, const Vector3<T> &v2)
{
	Assert(!v1.hasNaN() && !v2.hasNaN());
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