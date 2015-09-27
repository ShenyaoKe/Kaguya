#pragma once
#ifndef __COLLISION__
#define __COLLISION__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

#include "Accel/BBox.h"
#include "Geometry/Shape.h"
#include "Geometry/Mesh.h"

inline bool collideP(const BBox &box0, const BBox &box1)
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

inline bool collideP(const geoSphere &sphere, const BBox &box)
{
	return box.sqDist(sphere.getCenter()) <= sqr(sphere.getRadius());
}
inline bool collideP(const geoSphere &sphere, const Triangle &triangle)
{
	Point3D p = triangle.closestPoint(sphere.getCenter());
	return (p - sphere.getCenter()).getLenSq() <= sqr(sphere.getRadius());
}
inline bool collideP(const geoSphere &sph0, const geoSphere &sph1)
{
	Float sqDist = (sph0.getCenter() - sph1.getCenter()).getLenSq();
	return sqDist <= sqr(sph0.getRadius() + sph1.getRadius());
}

#endif // __COLLISION__