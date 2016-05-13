//
//  simpleGeomtry.h
//
//  Created by Shenyao Ke on 1/26/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __SHAPE__
#define __SHAPE__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

#include "Math/Geometry.h"
#include "Accel/Bounds.h"
#include "Image/ColorData.h"
#include "Math/Transform.h"
#include "Geometry/DifferentialGeometry.h"

const Float reCE = 5e-8;//ray epsilon coefficiency
const Float FloatEps = std::numeric_limits<Float>::epsilon();//Distance epsilon coefficiency
/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
class Shape
{
public:
	Shape(const Point3f &pos = Point3f());
	Shape(const Transform* o2w, const Transform* w2o)
		: ObjectToWorld(o2w), WorldToObject(w2o)
		, shapeID(nextshapeID++)
	{
	}
	virtual ~Shape();

	virtual void bounding() = 0;
	virtual Bounds3f getWorldBounding() const;
	virtual void refine(vector<Shape*> &refined);
	virtual bool intersectP(const Ray& inRay) const;
	/*virtual bool intersect(const Ray &ray, DifferentialGeometry *dg,
		Float *tHit, Float *rayEpsilon) const = 0;*/
	virtual bool intersect(const Ray& inRay, DifferentialGeometry* queryPoint,
		Float *tHit, Float *rayEpsilon) const = 0;

	// Surface Area
	virtual Float area() const;
	// Probability Density Function
	virtual Float Pdf() const;
	// Shading
	virtual void getShadingGeometry(const Transform &obj2world,
		DifferentialGeometry* dg) const;

	virtual bool isInside(const Point3f &pPos) const;
	virtual void assignTextureMapping(TextureMapping* &mapping);
	virtual void assignNormalMap(Texture* nMap);

	virtual void printInfo() const;

public:
	const uint32_t shapeID;
	static uint32_t nextshapeID;

	//Vector3f center;
	Point3f c;
	const Transform* ObjectToWorld;
	const Transform* WorldToObject;
	Bounds3f ObjBound;

	TextureMapping* UV_Mapping = nullptr;
	Texture* normalMap = nullptr;
};


#endif // __SHAPE__