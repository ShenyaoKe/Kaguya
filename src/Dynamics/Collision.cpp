#include "Dynamics/Collision.h"
#include "Accel/KdTreeAccel.h"
bool Collision::collideP(const BBox &box0, const BBox &box1)
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

bool Collision::collideP(const geoSphere &sphere, const BBox &box)
{
	return box.sqDist(sphere.getCenter()) <= sqr(sphere.getRadius());
}
bool Collision::collideP(const geoSphere &sphere, const Triangle &triangle)
{
	Point3D p = triangle.closestPoint(sphere.getCenter());
	return (p - sphere.getCenter()).getLenSq() <= sqr(sphere.getRadius());
}
bool Collision::collideP(const geoSphere &sph0, const geoSphere &sph1)
{
	Float sqDist = (sph0.getCenter() - sph1.getCenter()).getLenSq();
	return sqDist <= sqr(sph0.getRadius() + sph1.getRadius());
}
bool Collision::collide(const Point3D& prePos, const Point3D& curPos,
	const KdTreeAccel *tree, DifferentialGeometry *queryPoint,
	Float *tHit, Float* hitEpsilon)
{
	Ray ray = Ray(prePos, curPos - prePos);
	ray.tmax = (curPos - prePos).getLength();
	*tHit = ray.tmax;
	return tree->hit(ray, queryPoint, tHit, hitEpsilon);
}

/*
bool Collision::collide(const Point3D& prePos, const Point3D& curPos, const KdAccelNode *node, DifferentialGeometry *queryPoint)
{
	return false;
}
*/
