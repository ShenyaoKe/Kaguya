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

//#include "Core/rtdef.h"
#include "Math/CGVector.h"
#include "Math/Geometry.h"
#include "Image/ColorData.h"
#include "Math/Transform.h"
#include "Accel/BBox.h"
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
	virtual ~Shape() = 0;

	virtual void bounding() = 0;
	virtual BBox getWorldBounding() const;
	virtual void refine(vector<Shape*> &refined);
	virtual bool intersectP(const Ray& inRay) const;
	/*virtual bool intersect(const Ray &ray, DifferentialGeometry *dg,
		Float *tHit, Float *rayEpsilon) const = 0;*/
	virtual bool intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const = 0;

	// Surface Area
	virtual Float area() const;
	// Probability Density Function
	virtual Float Pdf() const;
	// Shading
	virtual void getShadingGeometry(const Transform &obj2world,
		const DifferentialGeometry &dg,
		DifferentialGeometry *dgShading) const;

	virtual bool isInside(const Point3f &pPos) const;
	virtual void assignTextureMapping(TextureMapping* &mapping);
	virtual void assignNormalMap(Texture* nMap);

	virtual void printInfo() const;

public:
	const uint32_t shapeId;
	static uint32_t nextshapeId;

	//Vector3f center;
	Point3f c;
	Transform ObjectToWorld;
	BBox ObjBound;

	TextureMapping* UV_Mapping = nullptr;
	Texture* normalMap = nullptr;
};


#endif // __SHAPE__