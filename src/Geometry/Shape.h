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
	Shape(const Point3D &pos = Point3D(0, 0, 0));
	virtual ~Shape() = 0;

	virtual void bounding() = 0;
	virtual BBox getWorldBounding() const;
	virtual void refine(vector<Shape*> &refined);
	virtual bool intersectP(const Ray& inRay) const;
	/*virtual bool intersect(const Ray &ray, DifferentialGeometry *dg,
		Float *tHit, Float *rayEpsilon) const = 0;*/
	virtual bool intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const = 0;

	virtual Float area() const;
	virtual Float Pdf() const;
	// Shading
	virtual void getShadingGeometry(const Transform &obj2world,
		const DifferentialGeometry &dg,
		DifferentialGeometry *dgShading) const;

	virtual bool isInside(const Vector3D &pPos) const;
	virtual void assignShader(Shader* shader);
	virtual void assignTextureMapping(TextureMapping* &mapping);
	virtual void assignNormalMap(Texture* nMap);

	virtual void printInfo() const;

	virtual bool getOpacity() const;

public:
	const uint32_t shapeId;
	static uint32_t nextshapeId;

	Point3D c;
	Transform ObjectToWorld;
	BBox ObjBound;
	Shader* material = nullptr;
	TextureMapping* UV_Mapping = nullptr;
	Texture* normalMap = nullptr;
};


#endif // __SHAPE__