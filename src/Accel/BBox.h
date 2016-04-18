#pragma once
#ifndef __BBox__
#define __BBox__

//#include "Core/rtdef.h"
#include "Math/CGVector.h"
#include "Math/Matrix4x4.h"
#include "Tracer/Ray.h"

//Bounding box
class BBox
{
public:
	Point3f pMin, pMax;
	
	BBox();
	BBox(const Point3f &p);
	BBox(const Point3f &p1, const Point3f &p2);
	BBox(const vector<Point3f*> &pts);
	BBox(const BBox &bound, const Matrix4x4 &mat);
	~BBox();

	void printInfo() const;

	const Point3f getMidPoint() const;
	const Vector3f getDiagnal() const;
	bool isInside(const Point3f &pos) const;
	/*bool intersectP(const Ray& inRay) const;*/
	bool intersectP(const Ray& inRay, Float *hitt0 = nullptr, Float *hitt1 = nullptr) const;
	Float sqDist(const Point3f &p) const;
	void expand(Float delta);
	void Union(const Point3f &p);
	void Union(const BBox& box);
	friend BBox Union(const BBox& box, const Point3f &p);
	friend BBox Union(const BBox& box1, const BBox& box2);
	friend bool overlaps(const BBox &box0, const BBox& box1);
	//friend bool touch(const BBox &box0, const BBox& box1);

	int maxExtent() const;
	Float surfaceArea() const;
};
#endif
