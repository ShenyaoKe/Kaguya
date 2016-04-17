//
//  TextureMapping.h
//
//  Created by Shenyao Ke on 3/6/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __TextureMapping__
#define __TextureMapping__
//#include "Core/rtdef.h"
#include "Math/CGVector.h"

class TextureMapping
{
protected:
	Point3D c;
	Vector3D nx, ny, nz;
public:

	TextureMapping();
	TextureMapping(const Vector3D &center);
	TextureMapping(const Vector3D &center, const Vector3D &view, const Vector3D &upVec);
	TextureMapping(const Vector3D &center, const Vector3D &xDir, const Vector3D &yDir, const Vector3D &zDir);
	virtual ~TextureMapping();

	virtual void printInfo() const;
	virtual void setPos(const Vector3D &pos);
	virtual void setDir(const Vector3D &view, const Vector3D &upVec);
	virtual Point3D posToUV(const Vector3D &pos) const;
	virtual void mapToUV(const DifferentialGeometry* queryPoint) const;
	virtual void getUVDir(const DifferentialGeometry* queryPoint) const;
	//virtual Vector2D mapToXY(const Float& x, const Float& y) const;
};
/************************************************************************/
/* Spherical Mapping                                                    */
/************************************************************************/
class SphericalMapping2D :public TextureMapping
{
public:
	SphericalMapping2D();
	SphericalMapping2D(const Vector3D &center);
	SphericalMapping2D(const Vector3D &center, const Vector3D &view, const Vector3D &upVec);
	SphericalMapping2D(const Vector3D &center, const Vector3D &xDir, const Vector3D &yDir, const Vector3D &zDir);
	~SphericalMapping2D();

	Point3D posToUV(const Vector3D &pos) const;
	//void mapToUV(const DifferentialGeometry* queryPoint) const;
	void getUVDir(const DifferentialGeometry* queryPoint) const;
protected:
private:
};
/************************************************************************/
/* Planar Mapping                                                       */
/************************************************************************/
class PlanarMapping2D :public TextureMapping
{
	Float uSize = 1, vSize = 1;
public:
	PlanarMapping2D();
	PlanarMapping2D(const Float& us, const Float& vs);
	PlanarMapping2D(const Vector3D &center);
	PlanarMapping2D(const Vector3D &center, const Float& us, const Float& vs);
	PlanarMapping2D(const Vector3D &center, const Vector3D &view, const Vector3D &upVec);
	PlanarMapping2D(const Vector3D &center, const Vector3D &xDir, const Vector3D &yDir, const Vector3D &zDir);
	~PlanarMapping2D();

	void setSize(const Float& us, const Float& vs);
	void mapToUV(const DifferentialGeometry* queryPoint) const;
protected:

private:
};
/************************************************************************/
/* Planar Mapping 3D                                                    */
/************************************************************************/
class PlanarMapping3D :public TextureMapping
{
	Float uSize = 1, vSize = 1, dist = 1;
public:
	PlanarMapping3D();
	PlanarMapping3D(const Float& us, const Float& vs, const Float& d);
	PlanarMapping3D(const Vector3D &center);
	PlanarMapping3D(const Vector3D &center, const Float& us, const Float& vs, const Float& d);
	PlanarMapping3D(const Vector3D &center, const Vector3D &view, const Vector3D &upVec);
	PlanarMapping3D(const Vector3D &center, const Vector3D &xDir, const Vector3D &yDir, const Vector3D &zDir);
	~PlanarMapping3D();

	void setSize(const Float& us, const Float& vs, const Float& d);
	void mapToUV(const DifferentialGeometry* queryPoint) const;
protected:

private:
};
#endif