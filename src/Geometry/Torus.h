#pragma once
#include "Core/Kaguya.h"
#include "Geometry/Shape.h"

/************************************************************************/
/* Torus Fuction Definition                                             */
/************************************************************************/
class geoTorus :public Shape
{
public:
	geoTorus() {};
	geoTorus(const Vector3D &pos, const Float& radius, const Float& secRadius);
	~geoTorus();

	void setCenter(const Vector3D &pos);
	void setRadius(Float radius);
	void setSecRadius(Float secRadius);//set section radius
	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;
	bool isInside(const Vector3D &pPos) const;

public:
	Vector3D c;//center
	Float r = 1, sr = 0.5;// radius and section radius
};
