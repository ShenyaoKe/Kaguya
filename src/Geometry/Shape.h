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

#include "Core/rtdef.h"
#include "Math/CGVector.h"
#include "Image/ColorData.h"
#include "Accel/BBox.h"

const Float reCE = 5e-8;//ray epsillon coefficiency
//////////////////////////////////////////////////////////////////////////
//Shader defaultShader(ColorRGBA(1.0, 1.0, 1.0), ColorRGBA(), 0, 1);
/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
class Shape
{
public:
	Shape();
	virtual ~Shape();
	virtual void bounding();
	virtual const BBox& getBounding() const;
	virtual void refine(vector<Shape*> &refined);
	virtual bool intersectP(const Ray& inRay) const;
	virtual bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const;

	virtual Vector3D getNormal(const Vector3D& pos) const;
	virtual void getNormal(const DifferentialGeometry *queryPoint) const;
	virtual bool isInside(const Vector3D& pPos) const;
	virtual void assignShader(Shader* shader);
	virtual void assignTextureMapping(TextureMapping* &mapping);// { UV_Mapping = mapping; }
	virtual void assignNormalMap(Texture* nMap);

	virtual void printInfo() const;
	virtual void getUV(const DifferentialGeometry *queryPoint) const;
	virtual Vector3D getCenter() const;

	virtual ColorRGBA getAmbient(const DifferentialGeometry *queryPoint) const;
	virtual ColorRGBA getDiffuse(const DifferentialGeometry *queryPoint, const Light* light) const;
	virtual ColorRGBA getColor(const DifferentialGeometry *queryPoint, const Light* light) const;

	virtual bool getOpacity() const;
protected:
	Point3D c;//center
	BBox ObjBound;
	Shader* material = NULL;
	TextureMapping* UV_Mapping = NULL;
	Texture* normalMap = NULL;
};
/************************************************************************/
/* Sphere Function Definition                                           */
/************************************************************************/
class geoSphere :public Shape
{
public:
	geoSphere();
	geoSphere(const Vector3D& pos, const Float& radius);
	~geoSphere();

	void bounding();
	void setCenter(const Vector3D& pos);
	void setRadius(Float radius);

	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const;
	Vector3D getNormal(const Vector3D& pos) const;
	void getNormal(const DifferentialGeometry *queryPoint) const;
	bool isInside(const Vector3D& pPos) const;

private:
	Float r;//radius
};
/************************************************************************/
/* Plane Function Definition                                            */
/************************************************************************/
class geoPlane :public Shape
{
public:
	geoPlane();
	geoPlane(const Vector3D& pos, const Vector3D& norm);
	~geoPlane();

	void bounding();
	void setOrigin(const Vector3D& pos);
	void setNorm(const Vector3D& norm);

	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const;
	Vector3D getNormal(const Vector3D& pos) const;
	void getNormal(const DifferentialGeometry *queryPoint) const;

	bool isInside(const Vector3D& pPos) const;

private:
	Vector3D n;
};
/************************************************************************/
/* Torus Fuction Definition                                             */
/************************************************************************/
class geoTorus :public Shape
{
public:
	geoTorus(){};
	geoTorus(const Vector3D& pos, const Float& radius, const Float& secRadius);
	~geoTorus();

	void setCenter(const Vector3D& pos);
	void setRadius(Float radius);
	void setSecRadius(Float secRadius);//set section radius
	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const;
	bool isInside(const Vector3D& pPos) const;

private:
	Vector3D c;//center
	Float r = 1, sr = 0.5;// radius and section radius
};
/************************************************************************/
/* Ellipsoid Function Definition                                        */
/************************************************************************/
class geoEllipsoid :public Shape
{
public:
	geoEllipsoid();
	geoEllipsoid(const Vector3D& pos, const Float& semiA, const Float& semiB, const Float& semiC);
	~geoEllipsoid();

	void setCenter(const Vector3D& pos);
	void setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC);
	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const;
	Vector3D getNormal(const Vector3D& pos) const;

	bool isInside(const Vector3D& pPos) const;

private:
	Vector3D c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
};
/************************************************************************/
/* Paraboloid Function Definition                                       */
/************************************************************************/
typedef enum
{
	ELLIPTIC_PARABOLOID = 1,
	HYPERBOLIC_PARABOLOID = -1
}PARABOLOID_TYPE;
class geoParaboloid :public Shape
{
public:
	geoParaboloid();
	geoParaboloid(const Vector3D& pos, const Float& semiA, const Float& semiB, const Float& semiC, PARABOLOID_TYPE newType);
	~geoParaboloid();

	void setCenter(const Vector3D& pos);
	void setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC);
	void setParaboloidType(PARABOLOID_TYPE newType);

	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const;
	Vector3D getNormal(const Vector3D& pos) const;

	bool isInside(const Vector3D& pPos) const;

private:
	Vector3D c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
	PARABOLOID_TYPE pbType;
};
/************************************************************************/
/* Hyperboloid Function Definition                                      */
/************************************************************************/
typedef enum
{
	ONE_SHEET = 1,
	TWO_SHEET = -1,
	CONE = 0
}HYPERBOLOID_TYPE;

class geoHyperboloid :public Shape
{
public:
	geoHyperboloid();
	geoHyperboloid(const Vector3D& pos, const Float& semiA, const Float& semiB, const Float& semiC, HYPERBOLOID_TYPE newType);
	~geoHyperboloid();

	void setCenter(const Vector3D& pos);
	void setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC);
	void setHyperboloidType(HYPERBOLOID_TYPE newType);
	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const;
	Vector3D getNormal(const Vector3D& pos) const;

	bool isInside(const Vector3D& pPos) const;

private:
	Vector3D c;//center
	Float sa, sb, sc;//semi-principal axes of length a, b, c
	HYPERBOLOID_TYPE hbType;
};
#endif // __SHAPE__