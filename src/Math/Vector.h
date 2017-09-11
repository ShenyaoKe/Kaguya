#pragma once

#include "Core/Kaguya.h"

namespace Kaguya
{

template <typename T>
inline bool isNaN(const T x)
{
	return std::isnan(x);
}
inline bool isNaN(const int /*x*/)
{
	return false;
}
/************************************************************************/
/* Vector                                                               */
/************************************************************************/
template <typename T>
class Vector2
{
public:
	Vector2() { x = y = 0; }
	Vector2(T _x, T _y) : x(_x), y(_y) {}
	Vector2(const T val[2]) : x(val[0]), y(val[1]) {}
	explicit Vector2(const Point2<T> &p);

	bool hasNaN() const { return isNaN(x) || isNaN(y); }

	T operator[](int i) const
	{
		Assert(i >= 0 && i <= 1);
		if (i == 0) return x;
		return y;
	}
	T &operator[](int i)
	{
		Assert(i >= 0 && i <= 1);
		if (i == 0) return x;
		return y;
	}
	Vector2<T> operator+(const Vector2<T> &v) const
	{
		Assert(!v.hasNaN());
		return Vector2<T>(x + v.x, y + v.y);
	}
	Vector2<T> &operator+=(const Vector2<T> &v)
	{
		Assert(!v.hasNaN());
		x += v.x; y += v.y;
		return *this;
	}
	Vector2<T> operator-(const Vector2<T> &v) const
	{
		Assert(!v.hasNaN());
		return Vector2<T>(x - v.x, y - v.y);
	}
	Vector2<T> &operator-=(const Vector2<T> &v)
	{
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y;
		return *this;
	}
	Vector2<T> operator-() const
	{
		return Vector2<T>(-x, -y);
	}
	bool operator==(const Vector2<T> &v) const
	{
		return x == v.x && y == v.y;
	}
	bool operator!=(const Vector2<T> &v) const
	{
		return x != v.x || y != v.y;
	}
	template <typename U>
	Vector2<T> operator*(U s) const
	{
		return Vector2<T>(x * s, y * s);
	}
	template <typename U>
	friend Vector2<T> operator*(U s, const Vector2<T> &v)
	{
		return v * s;
	}
	template <typename U>
	Vector2<T> &operator*=(U s)
	{
		Assert(!isNaN(s));
		x *= s; y *= s;
		return *this;
	}
	template <typename U>
	Vector2<T> operator/(U f) const
	{
		Assert(f != 0);
		Float inv = (Float)1 / f;
		return Vector2<T>(x * inv, y * inv);
	}
	template <typename U>
	Vector2<T> &operator/=(U f)
	{
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv;
		return *this;
	}

	Float lengthSquared() const { return x * x + y * y; }
	Float length() const { return std::sqrt(lengthSquared()); }
	void normalize() { *this /= length(); }

	friend std::ostream &operator<<(std::ostream &os, const Vector2<T> &v)
	{
		os << "[ " << v.x << ", " << v.y << " ]";
		return os;
	}
public:
	T x, y;
};

using Vector2f = Vector2<Float>;
using Vector2i = Vector2<int32_t>;

template <typename T>
class Vector3
{
public:
	Vector3() { x = y = z = 0; }
	Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	Vector3(const T val[3]) : x(val[0]), y(val[1]), z(val[2]) {}
	explicit Vector3(const Point3<T> &p);
	explicit Vector3(const Normal3<T> &p);

	bool hasNaN() const { return isNaN(x) || isNaN(y) || isNaN(z); }

	T operator[](int i) const
	{
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	T &operator[](int i)
	{
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	Vector3<T> operator+(const Vector3<T> &v) const
	{
		Assert(!v.hasNaN());
		return Vector3<T>(x + v.x, y + v.y, z + v.z);
	}
	Vector3<T> &operator+=(const Vector3<T> &v)
	{
		Assert(!v.hasNaN());
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector3<T> operator-(const Vector3<T> &v) const
	{
		Assert(!v.hasNaN());
		return Vector3<T>(x - v.x, y - v.y, z - v.z);
	}
	Vector3<T> &operator-=(const Vector3<T> &v)
	{
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vector3<T> operator-() const
	{
		return Vector3(-x, -y, -z);
	}
	bool operator==(const Vector3<T> &v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}
	bool operator!=(const Vector3<T> &v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}
	Vector3<T> &operator=(const Normal3<T> &n)
	{
		x = n.x; y = n.y; z = n.z;
	}
	template <typename U>
	Vector3<T> operator*(U s) const
	{
		return Vector3<T>(x * s, y * s, z * s);
	}
	template <typename U>
	friend Vector3<T> operator*(U s, const Vector3<T> &v)
	{
		return v * s;
	}
	template <typename U>
	Vector3<T> &operator*=(U s)
	{
		Assert(!isNaN(s));
		x *= s; y *= s; z *= s;
		return *this;
	}
	template <typename U>
	Vector3<T> operator/(U f) const
	{
		Assert(f != 0);
		Float inv = (Float)1 / f;
		return Vector3<T>(x * inv, y * inv, z * inv);
	}
	template <typename U>
	Vector3<T> &operator/=(U f)
	{
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	Float lengthSquared() const { return x * x + y * y + z * z; }
	Float length() const { return std::sqrt(lengthSquared()); }
	void normalize() { *this /= length(); }

	friend std::ostream &operator<<(std::ostream &os, const Vector3<T> &v)
	{
		os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return os;
	}
public:
	T x, y, z;
};

using Vector3f = Vector3<Float>;
using Vector3i = Vector3<int32_t>;

/************************************************************************/
/* Point                                                                */
/************************************************************************/
template <typename T>
class Point2
{
public:
	Point2() { x = y = 0; }
	Point2(T _x, T _y) : x(_x), y(_y) {}
	Point2(const T val[2]) : x(val[0]), y(val[1]) {}

	bool hasNaN() const { return isNaN(x) || isNaN(y); }

	T operator[](int i) const
	{
		Assert(i >= 0 && i <= 1);
		if (i == 0) return x;
		return y;
	}
	T &operator[](int i)
	{
		Assert(i >= 0 && i <= 1);
		if (i == 0) return x;
		return y;
	}
	Point2<T> operator+(const Point2<T> &p) const
	{
		Assert(!p.hasNaN());
		return Point2<T>(x + p.x, y + p.y);
	}
	Point2<T> &operator+=(const Point2<T> &p)
	{
		Assert(!p.hasNaN());
		x += p.x; y += p.y;
		return *this;
	}
	Point2<T> operator+(const Vector2<T> &v) const
	{
		Assert(!v.hasNaN());
		return Point2<T>(x + v.x, y + v.y);
	}
	Point2<T> &operator+=(const Vector2<T> &v)
	{
		Assert(!v.hasNaN());
		x += v.x; y += v.y;
		return *this;
	}
	Vector2<T> operator-(const Point2<T> &p) const
	{
		Assert(!p.hasNaN());
		return Vector2<T>(x - p.x, y - p.y);
	}
	Vector2<T> operator-(const Vector2<T> &v) const
	{
		Assert(!v.hasNaN());
		return Vector2<T>(x - v.x, y - v.y);
	}
	Point2<T> &operator-=(const Vector2<T> &v)
	{
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y;
		return *this;
	}
	Point2<T> operator-() const
	{
		return Point2<T>(-x, -y);
	}
	bool operator==(const Point2<T> &p) const
	{
		return x == p.x && y == p.y;
	}
	bool operator!=(const Point2<T> &p) const
	{
		return x != p.x || y != p.y;
	}

	template <typename U>
	Point2<T> operator*(U s) const
	{
		return Point2<T>(x * s, y * s);
	}
	template <typename U>
	friend Point2<T> operator*(U s, const Point2<T> &p)
	{
		return p * s;
	}
	template <typename U>
	Point2<T> operator*=(U s)
	{
		Assert(!isNaN(s));
		x *= s; y *= s;
		return *this;
	}
	template <typename U>
	Point2<T> operator/(U f) const
	{
		Assert(f != 0);
		Float inv = (Float)1 / f;
		return Point2<T>(x * inv, y * inv);
	}
	template <typename U>
	Point2<T> operator/=(U f)
	{
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv;
		return *this;
	}

	friend std::ostream &operator<<(std::ostream &os, const Point2<T> &p)
	{
		os << "[ " << p.x << ", " << p.y << " ]";
		return os;
	}
public:
	T x, y;
};

using Point2f = Point2<Float>;
using Point2i = Point2<int32_t>;

template <typename T>
class Point3
{
public:
	Point3() { x = y = z = 0; }
	Point3(T x, T y, T z) : x(x), y(y), z(z) {}
	Point3(const T val[3]) : x(val[0]), y(val[1]), z(val[2]) {}

	bool hasNaN() const { return isNaN(x) || isNaN(y) || isNaN(z); }

	T operator[](int i) const
	{
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	T &operator[](int i)
	{
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	Point3<T> operator+(const Point3<T> &p) const
	{
		Assert(!p.hasNaN());
		return Point3<T>(x + p.x, y + p.y, z + p.z);
	}
	Point3<T> &operator+=(const Point3<T> &p)
	{
		Assert(!p.hasNaN());
		x += p.x; y += p.y; z += p.z;
		return *this;
	}
	Point3<T> operator+(const Vector3<T> &v) const
	{
		Assert(!v.hasNaN());
		return Point3<T>(x + v.x, y + v.y, z + v.z);
	}
	Point3<T> &operator+=(const Vector3<T> &v)
	{
		Assert(!v.hasNaN());
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector3<T> operator-(const Point3<T> &p) const
	{
		Assert(!p.hasNaN());
		return Vector3<T>(x - p.x, y - p.y, z - p.z);
	}
	Point3<T> operator-(const Vector3<T> &v) const
	{
		Assert(!v.hasNaN());
		return Point3<T>(x - v.x, y - v.y, z - v.z);
	}
	Point3<T> &operator-=(const Vector3<T> &v)
	{
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Point3<T> operator-() const
	{
		return Point3<T>(-x, -y, -z);
	}
	bool operator==(const Point3<T> &p) const
	{
		return x == p.x && y == p.y && z == p.z;
	}
	bool operator!=(const Point3<T> &p) const
	{
		return x != p.x || y != p.y || z != p.z;
	}

	template <typename U>
	Point3<T> operator*(U s) const
	{
		return Point3<T>(x * s, y * s, z * s);
	}
	template <typename U>
	friend Point3<T> operator*(U s, const Point3<T> &p)
	{
		return p * s;
	}
	template <typename U>
	Point3<T> operator*=(U s)
	{
		Assert(!isNaN(s));
		x *= s; y *= s; z *= s;
		return *this;
	}
	template <typename U>
	Point3<T> operator/(U f) const
	{
		Assert(f != 0);
		Float inv = (Float)1 / f;
		return Point3<T>(x * inv, y * inv, z * inv);
	}
	template <typename U>
	Point3<T> &operator/=(U f)
	{
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	friend std::ostream &operator<<(std::ostream &os, const Point3<T> &p)
	{
		os << "[ " << p.x << ", " << p.y << ", " << p.z << " ]";
		return os;
	}
public:
	T x, y, z;
};

using Point3f = Point3<Float>;
using Point3i = Point3<int32_t>;

/************************************************************************/
/* Normal                                                               */
/************************************************************************/
template <typename T>
class Normal3
{
public:
	Normal3() { x = y = z = 0; }
	Normal3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	Normal3(const T val[3]) : x(val[0]), y(val[1]), z(val[2]) {}
	explicit Normal3(const Vector3<T> &vec) : x(vec.x), y(vec.y), z(vec.z) {}

	bool hasNaN() const { return isNaN(x) || isNaN(y) || isNaN(z); }

	T operator[](int i) const
	{
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	T &operator[](int i)
	{
		Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	Normal3<T> operator+(const Normal3<T> &v) const
	{
		Assert(!v.hasNaN());
		return Normal3<T>(x + v.x, y + v.y, z + v.z);
	}
	Normal3<T> &operator+=(const Normal3<T> &v)
	{
		Assert(!v.hasNaN());
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Normal3<T> operator-(const Normal3<T> &v) const
	{
		Assert(!v.hasNaN());
		return Normal3<T>(x - v.x, y - v.y, z - v.z);
	}
	Normal3<T> &operator-=(const Normal3<T> &v)
	{
		Assert(!v.hasNaN());
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Normal3<T> operator-() const
	{
		return Normal3<T>(-x, -y, -z);
	}
	bool operator==(const Normal3<T> &n) const
	{
		return x == n.x && y == n.y && z == n.z;
	}
	bool operator!=(const Normal3<T> &n) const
	{
		return x != n.x || y != n.y || z != n.z;
	}
	Normal3<T> &operator=(const Vector3<T> &v)
	{
		x = v.x; y = v.y; z = v.z;
	}

	template <typename U>
	Normal3<T> operator*(U s) const
	{
		return Normal3<T>(x * s, y * s, z * s);
	}
	template <typename U>
	friend Normal3<T> operator*(U s, const Normal3<T> &n)
	{
		return n * s;
	}
	template <typename U>
	Normal3<T> &operator*=(U s)
	{
		Assert(!isNaN(f));
		x *= s; y *= s; z *= s;
		return *this;
	}
	template <typename U>
	Normal3<T> operator/(U f) const
	{
		Assert(f != 0);
		Float inv = (Float)1 / f;
		return Normal3<T>(x * inv, y * inv, z * inv);
	}
	template <typename U>
	Normal3<T> &operator/=(U f)
	{
		Assert(f != 0);
		Float inv = (Float)1 / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	Float lengthSquared() const { return x * x + y * y + z * z; }
	Float length() const { return std::sqrt(lengthSquared()); }
	void normalize() { *this /= length(); }


	friend std::ostream &operator<<(std::ostream &os, const Normal3<T> &v)
	{
		os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
		return os;
	}
public:
	T x, y, z;
};
using Normal3f = Normal3<Float>;

/************************************************************************/
/* Vector Implementation                                                */
/************************************************************************/

template <typename T>
Vector2<T>::Vector2(const Point2<T> &p)
	: x(p.x), y(p.y)
{
}

template<typename T>
inline Vector3<T>::Vector3(const Point3<T> &p)
	: x(p.x), y(p.y), z(p.z)
{
}

template<typename T>
inline Vector3<T>::Vector3(const Normal3<T> &n)
	: x(n.x), y(n.y), z(n.z)
{
}

template <typename T>
inline T cross(const Vector2<T> &v1, const Vector2<T> &v2)
{
	Assert(!v1.hasNaN() && !v2.hasNaN());
	return v1.x * v2.y - v1.y * v2.x;
}

template <typename T>
inline Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2)
{
	Assert(!v1.hasNaN() && !v2.hasNaN());
	return Vector3<T>(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
		);
}

template <typename T>
inline T dot(const Vector2<T> &v1, const Vector2<T> &v2)
{
	Assert(!v1.hasNaN() && !v2.hasNaN());
	return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
inline T dot(const Vector3<T> &v1, const Vector3<T> &v2)
{
	Assert(!v1.hasNaN() && !v2.hasNaN());
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
inline T dot(const Vector3<T> &v, const Normal3<T> &n)
{
	Assert(!v.hasNaN() && !n.hasNaN());
	return v.x * n.x + v.y * n.y + v.z * n.z;
}

template <typename T>
inline T dot(const Normal3<T> &n, const Vector3<T> &v)
{
	Assert(!v.hasNaN() && !n.hasNaN());
	return v.x * n.x + v.y * n.y + v.z * n.z;
}

template <typename T>
inline Vector2<T> normalize(const Vector2<T> &v)
{
	return v / v.length();
}

template <typename T>
inline Vector3<T> normalize(const Vector3<T> &v)
{
	return v / v.length();
}

template <typename T>
inline Normal3<T> normalize(const Normal3<T> &n)
{
	return n / n.length();
}

template <typename T>
Vector2<T> abs(const Vector2<T> &v)
{
	return Vector2<T>(std::abs(v.x), std::abs(v.y));
}

template <typename T>
Vector3<T> abs(const Vector3<T> &v)
{
	return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
Point2<T> abs(const Point2<T> &p)
{
	return Point2<T>(std::abs(p.x), std::abs(p.y));
}

template <typename T>
Point3<T> abs(const Point3<T> &p)
{
	return Point3<T>(std::abs(p.x), std::abs(p.y), std::abs(p.z));
}

template <typename T>
int maxDimension(const Vector3<T> &v)
{
	return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2)
		: ((v.y > v.z) ? 1 : 2);
}

template <typename T>
Point2<T> permute(const Point2<T> &p, int x, int y)
{
	return Point2<T>(p[x], p[y], p[z]);
}

template <typename T>
Point3<T> permute(const Point3<T> &p, int x, int y, int z)
{
	return Point3<T>(p[x], p[y], p[z]);
}

template <typename T>
Vector2<T> permute(const Vector2<T> &v, int x, int y)
{
	return Vector2<T>(v[x], v[y], v[z]);
}

template <typename T>
Vector3<T> permute(const Vector3<T> &v, int x, int y, int z)
{
	return Vector3<T>(v[x], v[y], v[z]);
}

template <typename T>
inline void coordinateSystem(const Vector3<T> &v1,
							 Vector3<T>* v2,
							 Vector3<T>* v3)
{
	if (std::abs(v1.x) > std::abs(v1.y))
	{
		*v2 = Vector3<T>(-v1.z, 0, v1.x)
			/ std::sqrt(v1.x * v1.x + v1.z * v1.z);
	}
	else
	{
		*v2 = Vector3<T>(0, v1.z, -v1.y)
			/ std::sqrt(v1.y * v1.y + v1.z * v1.z);
	}
	*v3 = cross(v1, *v2);
}

}
