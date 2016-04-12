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
//////////////////////////////////////////////////////////////////////////
//Shader defaultShader(ColorRGBA(1.0, 1.0, 1.0), ColorRGBA(), 0, 1);
/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
class Shape
{
	static int uid;
public:
	Shape(const Point3D &pos = Point3D(0, 0, 0));
	virtual ~Shape() = 0;

	static int assignIndex() { return uid++; }
	static void offsetUID(int offset) { uid += offset; }

	virtual void bounding();
	virtual BBox getWorldBounding() const;
	virtual void refine(vector<Shape*> &refined);
	virtual bool intersectP(const Ray& inRay) const;
	virtual bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const;

	virtual Vector3D getNormal(const Vector3D &pos) const;
	virtual void getNormal(const DifferentialGeometry* queryPoint) const;
	virtual bool isInside(const Vector3D &pPos) const;
	virtual void assignShader(Shader* shader);
	virtual void assignTextureMapping(TextureMapping* &mapping);// { UV_Mapping = mapping; }
	virtual void assignNormalMap(Texture* nMap);

	virtual int getIndex() const;
	//virtual int assignIndex(int);
	virtual void printInfo() const;
	virtual void getUV(const DifferentialGeometry* queryPoint) const;
	virtual Vector3D getCenter() const;

	// Shading
	virtual ColorRGBA getAmbient(const DifferentialGeometry* queryPoint) const;
	virtual ColorRGBA getDiffuse(const DifferentialGeometry* queryPoint, const Light* light) const;
	virtual ColorRGBA getColor(const DifferentialGeometry* queryPoint, const Light* light) const;

	virtual bool getOpacity() const;
public:
	virtual const Vector3D &closestPoint(const Point3D &p) const;

public:
	int index;
	Transform ObjectToWorld;
	Point3D c;//center
	BBox ObjBound;
	Shader* material = nullptr;
	TextureMapping* UV_Mapping = nullptr;
	Texture* normalMap = nullptr;
};


#endif // __SHAPE__