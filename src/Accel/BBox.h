#pragma once
#ifndef __BBox__
#define __BBox__

//#include "Core/rtdef.h"
#include "Math/CGVector.h"
#include "Math/Matrix4D.h"
#include "Tracer/Ray.h"

//Bounding box
class BBox
{
public:
	Point3D pMin, pMax;
	
	BBox();
	BBox(const Point3D& p);
	BBox(const Point3D& p1, const Point3D& p2);
	BBox(const vector<Point3D*> &pts);
	BBox(const BBox &bound, const Matrix4D &mat);
	~BBox();

	void printInfo() const;

	const Point3D getMidPoint() const;
	const Vector3D getDiagnal() const;
	bool isInside(const Point3D& pos) const;
	/*bool intersectP(const Ray& inRay) const;*/
	bool intersectP(const Ray& inRay, Float *hitt0 = nullptr, Float *hitt1 = nullptr) const;
	Float sqDist(const Point3D &p) const;
	void expand(Float delta);
	void Union(const Point3D& p);
	void Union(const BBox& box);
	friend BBox Union(const BBox& box, const Point3D& p);
	friend BBox Union(const BBox& box1, const BBox& box2);
	friend bool overlaps(const BBox &box0, const BBox& box1);
	//friend bool touch(const BBox &box0, const BBox& box1);

	int maxExtent() const;
	Float surfaceArea() const;
};
#endif
