#pragma once

#include "Core/Kaguya.h"
#include "Math/Vector.h"
#include "Math/Matrix4x4.h"
#include "Tracer/Ray.h"

namespace Kaguya
{

template <typename T>
class Bounds2
{
public:
	Point2<T> pMin, pMax;

	Bounds2()
	{
		T minval = std::numeric_limits<T>::lowest();
		T maxval = std::numeric_limits<T>::max();
		pMin = Point2<T>(minval, minval);
		pMax = Point2<T>(maxval, maxval);
	}
	Bounds2(const Point2<T> &p) : pMin(p), pMax(p) {}
	Bounds2(const Point2<T> &p1, const Point2<T> &p2)
		: pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y))
		, pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y))
	{
	}
	Bounds2(const std::vector<Point2<T>*> &pts)
	{
		T minval = std::numeric_limits<T>::lowest();
		T maxval = std::numeric_limits<T>::max();
		pMin = Point2<T>(minval, minval);
		pMax = Point2<T>(maxval, maxval);
		for (auto pt : pts) Union(*pt);
	}

	void printInfo() const
	{
		std::cout << "Min Point:" << pMin
			<< "\nMax Point:" << pMax
			<< "\nDiagnal:" << (pMax - pMin) << std::endl;
	}

	const Point2<T> midpoint() const
	{
		return (pMax + pMin) * 0.5;
	}
	const Vector2<T> diagnal() const
	{
		return pMax - pMin;
	}
	bool isInside(const Point2<T> &pos) const
	{
		if (pos.x < pMin.x || pos.x > pMax.x) return false;
		if (pos.y < pMin.y || pos.y > pMax.y) return false;

		return true;
	}

	T sqDist(const Point2<T> &p) const
	{
		T sqD = 0;

		for (int i = 0; i < 2; i++)
		{
			T v = p[i];
			if (v < pMin[i]) sqD += sqr(pMin[i] - v);
			if (v > pMax[i]) sqD += sqr(pMax[i] - v);
		}
		return sqD;
	}
	void expand(T delta)
	{
		pMin -= Vector2<T>(delta, delta);
		pMax += Vector2<T>(delta, delta);
	}
	void Union(const Point2<T> &p)
	{
		for (int i = 0; i < 2; i++)
		{
			pMin[i] = std::min(pMin[i], p[i]);
			pMax[i] = std::max(pMax[i], p[i]);
		}
	}
	void Union(const Bounds2<T> &box)
	{
		for (int i = 0; i < 2; i++)
		{
			pMin[i] = std::min(pMin[i], box.pMin[i]);
			pMax[i] = std::max(pMax[i], box.pMax[i]);
		}
	}
	friend Bounds2 Union(const Bounds2<T> &box, const Point2<T> &p);
	friend Bounds2 Union(const Bounds2<T> &box1, const Bounds2<T> &box2);
	friend bool overlaps(const Bounds2<T> &box0, const Bounds2<T> &box1);

	int maxExtent() const
	{
		if (pMax.x - pMin.x > pMax.y - pMin.y) return 0;
		else return 1;
	}
	Float surfaceArea() const
	{
		return (pMax.x - pMin.x) * (pMax.y - pMin.y);
	}
};

using Bounds2i = Bounds2<int32_t>;
using Bounds2f = Bounds2<Float>;

//Bounding box
template <typename T>
class Bounds3
{
public:
	Point3<T> pMin, pMax;

	Bounds3()
	{
		T minval = std::numeric_limits<T>::lowest();
		T maxval = std::numeric_limits<T>::max();
		pMin = Point3<T>(minval, minval, minval);
		pMax = Point3<T>(maxval, maxval, maxval);
	}
	Bounds3(const Point3<T> &p) : pMin(p), pMax(p) {}
	Bounds3(const Point3<T> &p1, const Point3<T> &p2)
		: pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z))
		, pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z))
	{
	}
	Bounds3(const std::vector<Point3<T>*> &pts)
	{
		T minval = std::numeric_limits<T>::lowest();
		T maxval = std::numeric_limits<T>::max();
		pMin = Point3<T>(minval, minval, minval);
		pMax = Point3<T>(maxval, maxval, maxval);
		for (auto pt : pts) Union(*pt);
	}
	Bounds3(const Bounds3 &bound, const Matrix4x4 &mat)
	{
		for (int i = 0; i < 3; i++)
		{
			pMin[i] = pMax[i] = mat[3][i];
			for (int j = 0; j < 3; j++)
			{
				Float e = mat[j][i] * bound.pMin[j];
				Float f = mat[j][i] * bound.pMax[j];
				if (e < f)
				{
					pMin[i] += e;
					pMax[i] += f;
				}
				else
				{
					pMin[i] += f;
					pMax[i] += e;
				}
			}
		}
	}

	void printInfo() const
	{
		std::cout << "Min Point:" << pMin
			<< "\nMax Point:" << pMax
			<< "\nDiagnal:" << (pMax - pMin) << std::endl;
	}

	const Point3<T> midpoint() const
	{
		return (pMax + pMin) * 0.5;
	}
	const Vector3<T> diagnal() const
	{
		return pMax - pMin;
	}
	bool isInside(const Point3<T> &pos) const
	{
		if (pos.x < pMin.x || pos.x > pMax.x) return false;
		if (pos.y < pMin.y || pos.y > pMax.y) return false;
		if (pos.z < pMin.z || pos.z > pMax.z) return false;

		return true;
	}
	bool intersectP(const Ray &inRay, Float* hitt0 = nullptr, Float* hitt1 = nullptr) const
	{
		Float t0 = inRay.tMin, t1 = inRay.tMax;

		for (int i = 0; i < 3; i++)
		{
			Float invRayDir = 1.0 / inRay.d[i];
			Float tNear = (pMin[i] - inRay.o[i]) * invRayDir;
			Float tFar = (pMax[i] - inRay.o[i]) * invRayDir;
			if (tNear > tFar) std::swap(tNear, tFar);

			t0 = tNear > t0 ? tNear : t0;
			t1 = tFar < t1 ? tFar : t1;
			if (t0 > t1) return false;
		}
		if (hitt0) *hitt0 = t0;
		if (hitt1) *hitt1 = t1;

		return true;
	}
	T sqDist(const Point3<T> &p) const
	{
		T sqD = 0;

		for (int i = 0; i < 3; i++)
		{
			T v = p[i];
			if (v < pMin[i]) sqD += sqr(pMin[i] - v);
			if (v > pMax[i]) sqD += sqr(pMax[i] - v);
		}
		return sqD;
	}
	void expand(T delta)
	{
		pMin -= Vector3<T>(delta, delta, delta);
		pMax += Vector3<T>(delta, delta, delta);
	}
	void Union(const Point3<T> &p)
	{
		for (int i = 0; i < 3; i++)
		{
			pMin[i] = std::min(pMin[i], p[i]);
			pMax[i] = std::max(pMax[i], p[i]);
		}
	}
	void Union(const Bounds3<T> &box)
	{
		for (int i = 0; i < 3; i++)
		{
			pMin[i] = std::min(pMin[i], box.pMin[i]);
			pMax[i] = std::max(pMax[i], box.pMax[i]);
		}
	}
	/*friend Bounds3 Union(const Bounds3<T> &box, const Point3<T> &p);
	friend Bounds3 Union(const Bounds3<T> &box1, const Bounds3<T> &box2);
	friend bool overlaps(const Bounds3<T> &box0, const Bounds3<T> &box1);*/

	int maxExtent() const
	{
		Vector3<T> diag = diagnal();
		if (diag.x > diag.y && diag.x > diag.z) return 0;
		else if (diag.y > diag.z) return 1;
		else return 2;
	}
	Float surfaceArea() const
	{
		Vector3<T> d = diagnal();
		return 2 * (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
	}
};

using Bounds3i = Bounds3<int32_t>;
using Bounds3f = Bounds3<Float>;

template <typename T>
inline Bounds2<T> Union(const Bounds2<T> &box, const Point2<T> &p)
{
	Bounds2<T> &ret = box;
	for (int i = 0; i < 2; i++)
	{
		ret.pMin[i] = std::min(box.pMin[i], p[i]);
		ret.pMax[i] = std::max(box.pMax[i], p[i]);
	}
	return ret;
}

template <typename T>
inline Bounds2<T> Union(const Bounds2<T> &box1, const Bounds2<T> &box2)
{
	Bounds2<T> ret;
	for (int i = 0; i < 2; i++)
	{
		ret.pMin[i] = std::min(box1.pMin[i], box2.pMin[i]);
		ret.pMax[i] = std::max(box1.pMax[i], box2.pMax[i]);
	}
	return ret;
}

template <typename T>
inline bool overlaps(const Bounds2<T> &box0, const Bounds2<T> &box1)
{
	if (box0.pMax.x < box1.pMin.x || box0.pMin.x > box1.pMax.x)
	{
		return false;
	}
	if (box0.pMax.y < box1.pMin.y || box0.pMin.y > box1.pMax.y)
	{
		return false;
	}
	return true;
}
template <typename T>
inline Bounds3<T> Union(const Bounds3<T> &box, const Point3<T> &p)
{
	Bounds3<T> ret = box;
	for (int i = 0; i < 3; i++)
	{
		ret.pMin[i] = std::min(box.pMin[i], p[i]);
		ret.pMax[i] = std::max(box.pMax[i], p[i]);
	}
	return ret;
}

template <typename T>
inline Bounds3<T> Union(const Bounds3<T> &box1, const Bounds3<T> &box2)
{
	Bounds3<T> ret;
	for (int i = 0; i < 3; i++)
	{
		ret.pMin[i] = std::min(box1.pMin[i], box2.pMin[i]);
		ret.pMax[i] = std::max(box1.pMax[i], box2.pMax[i]);
	}
	return ret;
}

template <typename T>
inline bool overlaps(const Bounds3<T> &box0, const Bounds3<T> &box1)
{
	if (box0.pMax.x < box1.pMin.x || box0.pMin.x > box1.pMax.x)
	{
		return false;
	}
	if (box0.pMax.y < box1.pMin.y || box0.pMin.y > box1.pMax.y)
	{
		return false;
	}
	if (box0.pMax.z < box1.pMin.z || box0.pMin.z > box1.pMax.z)
	{
		return false;
	}
	return true;
}

}
