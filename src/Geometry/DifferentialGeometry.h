//
//  DifferentialGeometry.h
//
//  Created by Shenyao Ke on 3/1/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#ifndef __DifferentialGeometry__
#define __DifferentialGeometry__

#define KAGUYA_DOUBLE_AS_FLOAT

#include "Core/Kaguya.h"
#include "Math/CGVector.h"

/************************************************************************/
/* DifferentialGeometry                                                 */
/************************************************************************/
struct DifferentialGeometry
{
	const Shape* object;
	//int objID;
	mutable Point3D pos;
	mutable Vector3D normal, reflectDir;
	mutable Vector3D lightDir;
	mutable Float lightDist;
	//Sample related viriables
	int sIndexX, sIndexY, sample;
	int shiftX, shiftY, sampleOffset;

	mutable Point3D UV;
	mutable Vector3D uDir, vDir;//Partial derivation of the surface position, marked as dpdu, dpdv
	Vector3D dndu, dndv;//Partial derivation of the surface normal

	DifferentialGeometry();
	DifferentialGeometry(const Shape* obj, const Vector3D& pVec, const Vector3D& nVec, const Vector3D& rVec, const int& sX, const int& sY);
	~DifferentialGeometry();

	void setObject(const Shape* obj);
	void setPos(const Point3D& pVec);
	void setNormal(const Vector3D& nVec);
	void setReflectDir(const Vector3D& rVec);
 	void calculateDir(const Vector3D& inDir, const Vector3D& nVec);
 	void calculateDir(const Vector3D& inDir);
 	void setSample(const int& aaS, const int& aaOffset);
 	void setSampleIndex(const int& x, const int& y);
 	Float getDiffuseTheta() const;
 	Float getSpecularTheta() const;
};
inline DifferentialGeometry::DifferentialGeometry()
{
	object = NULL;
	lightDist = INFINITY;
	//Sample related viriables
	sIndexX = 0;
	sIndexY = 0;
	sample = 1;
	sampleOffset = 0;
}
inline DifferentialGeometry::DifferentialGeometry(const Shape* obj, const Vector3D& pVec, const Vector3D& nVec, const Vector3D& rVec, const int& sX, const int& sY)
{
	object = obj;
	pos = pVec;
	normal = nVec;
	reflectDir = rVec;

	lightDist = INFINITY;
	//Sample related viriables
	sIndexX = sX;
	sIndexY = sY;
	sample = 1;
	sampleOffset = 0;
}
inline DifferentialGeometry::~DifferentialGeometry()
{
	object = NULL;
}
inline void DifferentialGeometry::setObject(const Shape* obj)
{
	object = obj;
}
inline void DifferentialGeometry::setPos(const Point3D& pVec)
{
	pos = pVec;
}
inline void DifferentialGeometry::setNormal(const Vector3D& nVec)
{
	normal = nVec;
}
inline void DifferentialGeometry::setReflectDir(const Vector3D& rVec)
{
	reflectDir = rVec;
}
inline void DifferentialGeometry::calculateDir(const Vector3D& inDir, const Vector3D& nVec)
{
	normal = nVec;
	reflectDir = inDir - normal * (inDir * normal) * 2;
}
inline void DifferentialGeometry::calculateDir(const Vector3D& inDir)
{
	reflectDir = inDir - normal * (inDir * normal) * 2;
}
inline void DifferentialGeometry::setSample(const int& aaS, const int& aaOffset)
{
	sample = aaS;
	sampleOffset = aaOffset;
}
inline void DifferentialGeometry::setSampleIndex(const int& x, const int& y)
{
	sIndexX = x;
	sIndexY = y;
	int newIndex = (y * sample + x + sampleOffset) % (sample * sample);
	shiftX = newIndex % sample;
	shiftY = newIndex / sample;
}
inline Float DifferentialGeometry::getDiffuseTheta() const
{
	return lightDir * normal;
}
inline Float DifferentialGeometry::getSpecularTheta() const
{
	return lightDir * reflectDir;
}
//
#endif // __DifferentialGeometry__