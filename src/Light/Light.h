//
//  Light.h
//
//  Created by Shenyao Ke on 2/13/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __Light__
#define __Light__

#include "Math/Geometry.h"
#include "Light/Spectrum.h"

enum LIGHT_TYPE
{
	LT_NULL = 0,
	LT_POINT_LIGHT,
	LT_DIRECTIONAL_LIGHT,
	LT_SPOT_LIGHT,
	LT_AREA_LIGHT
};
enum LIGHT_DECAY_TYPE
{
	DECAY_CONSTANT = 0,
	DECAY_QUADRATIC = 1,

};
/************************************************************************/
/* Basic Light                                                          */
/************************************************************************/
class Light
{
protected:
	LIGHT_TYPE type = LT_NULL;
	Spectrum lightSpectrum;
	Float exposure = 0;
	//ColorRGB color = ColorRGB(1.0, 1.0, 1.0);
	LIGHT_DECAY_TYPE decayType = DECAY_CONSTANT;
	Point3f pos;
	Float radius = 0;
public:

	Light();
	Light(const Spectrum& its);
	virtual ~Light();

	virtual void setExposure(const Float& xps);
	virtual void setDecayType(LIGHT_DECAY_TYPE dctype);
	virtual void setRadius(const Float& rd);

	virtual LIGHT_TYPE getLightType() const;
	virtual Spectrum getSpectrum(const DifferentialGeometry* queryPoint) const;
	virtual Float getDistance(const DifferentialGeometry* queryPoint) const;
	virtual void printInfo() const;
};
/************************************************************************/
/* Directional Light                                                    */
/************************************************************************/
class directionalLight :public Light
{
	Vector3f dir = Vector3f(0, 0, -1);
public:
	directionalLight();
	directionalLight(const Vector3f &vec);
	directionalLight(const Vector3f &vec, const Spectrum& spt);
	~directionalLight();

	void printInfo() const;
	Float getDistance(const DifferentialGeometry* queryPoint) const;
protected:

private:
};
/************************************************************************/
/* Point Light                                                          */
/************************************************************************/
class pointLight :public Light
{
	//Point3f pos;
public:
	pointLight();
	pointLight(const Point3f &p, const Float& its);
	pointLight(const Point3f &p, const Spectrum& spt);
	~pointLight();

	void printInfo() const;
protected:

private:

};
/************************************************************************/
/* Spot Light                                                           */
/************************************************************************/
class spotLight :public Light
{
protected:
	//Point3f pos;
	Vector3f dir;
	Float coneAngle = 40;
	Float penumbraAngle = 0;
	Float cosCA = cos(DegToRad(cosCA));
	Float cosPA = cos(DegToRad(coneAngle + penumbraAngle));
	Float dropoff = 0;
public:
	spotLight();
	spotLight(const Point3f &p, const Vector3f &d);
	spotLight(const Point3f &p, const Vector3f &d, const Float& ca, const Float& pa, const Float& dpo);
	spotLight(const Point3f &p, const Vector3f &d, const Float& ca, const Float& pa, const Float& dpo, const Spectrum& spt);
	~spotLight();

	void printInfo() const;
	void setAngles(const Float& ca, const Float& pa);
	void updateCosAngle();
	void setDropOff(const Float& dpo);

	Float getIntensity(const DifferentialGeometry* queryPoint) const;
};
/************************************************************************/
/* Area Light                                                           */
/************************************************************************/
enum AREA_LIGHT_SHAPE
{
	QUAD = 0,
	DISK = 1,
	CYLINDER = 2
};
class areaLight :public Light
{
	Vector3f nx, ny, nz;
	//Point3f pos;
	Float size = 1;//radius
	AREA_LIGHT_SHAPE shapeType = QUAD;
public:
	areaLight();
	areaLight(const Point3f &p, const Float& shpSize);
	areaLight(const Point3f &p, const Float& shpSize, const Spectrum& spt);
	areaLight(const Point3f &p, const Vector3f &dir, const Vector3f &up, const Float& shpSize, const Spectrum& spt);
	~areaLight();

	Float getIntensity(const DifferentialGeometry* queryPoint) const;
protected:

private:
};
#endif