#if defined(_MSC_VER)
#pragma once
#endif
#ifndef KAGUYA_GEOMETRY_H
#define KAGUYA_GEOMETRY_H

#include "Core/Kaguya.h"

template <typename T>
class Vector3
{
public:

	Vector3() { x = y = z = 0; }
	Vector3(T _x, T _y, T _z) : x(_x), y(_y), (_z) {}
	explicit Vector3(const Point3<T> &p);

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

template <typename T>
class Point3
{
public:
	Point3() { x = y = z = 0; }
	Point3(T x, T y, T z) : x(x), y(y), z(z) {}
	friend std::ostream &operator<<(ostream& os, const Point3<T> &v)
	{
		os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return os;
	}
public:
	T x, y, z;
};

typedef Point3<Float> Point3f;
typedef Point3<int> Point3i;

#endif // KAGUYA_GEOMETRY_H