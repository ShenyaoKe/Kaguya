/************************************************************************/
/* Sphere Function Definition                                           */
/************************************************************************/
#pragma once
#include "Geometry/Shape.h"

class geoSphere :public Shape
{
public:
	geoSphere(const Vector3D &pos = Vector3D(0, 0, 0), const Float& radius = 1);
	~geoSphere();

	void bounding();
	BBox getWorldBounding() const;

	void setCenter(const Vector3D &pos);
	void setRadius(Float radius);

	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	Vector3D getNormal(const Vector3D &pos) const;
	void getNormal(const DifferentialGeometry* queryPoint) const;
	Float getRadius() const;
	bool isInside(const Vector3D &pPos) const;

public:
	Float r;//radius
};

