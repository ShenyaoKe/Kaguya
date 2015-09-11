#pragma once
#ifndef __BBox__
#define __BBox__

#include "Core/rtdef.h"
#include "Math/CGVector.h"
#include "Tracer/Ray.h"

//Bounding box
class BBox
{
public:
	Point3D pMin, pMax;
	
	BBox();
	BBox(const Point3D& p);
	BBox(const Point3D& p1, const Point3D& p2);
	~BBox();

	void printInfo() const;

	const Point3D getMidPoint() const;
	/*bool intersectP(const Ray& inRay) const;*/
	bool intersectP(const Ray& inRay, double *hitt0 = nullptr, double *hitt1 = nullptr) const;
	bool overlaps(const BBox& box) const;
	void expand(double delta);
	void Union(const Point3D& p);
	void Union(const BBox& box);
	friend BBox Union(const BBox& box, const Point3D& p);
	friend BBox Union(const BBox& box1, const BBox& box2);

	int maxExtent() const;
	double surfaceArea() const;
};
#endif
