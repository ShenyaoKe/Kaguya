/*
#include "Geometry/DifferentialGeometry.h"
#include "Geometry/Shape.h"
DifferentialGeometry::DifferentialGeometry()
{
	object = NULL;
	lightDist = INFINITY;
	//Sample related viriables
	sIndexX = 0;
	sIndexY = 0;
	sample = 1;
	sampleOffset = 0;
}
DifferentialGeometry::DifferentialGeometry(const Shape* obj, const Vector3D& pVec, const Vector3D& nVec, const Vector3D& rVec, const int& sX, const int& sY)
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
DifferentialGeometry::~DifferentialGeometry()
{
	object = NULL;
}
void DifferentialGeometry::setObject(const Shape* obj)
{
	object = obj;
}
void DifferentialGeometry::setPos(const Point3D& pVec)
{
	pos = pVec;
}
void DifferentialGeometry::setNormal(const Vector3D& nVec)
{
	normal = nVec;
}
void DifferentialGeometry::setReflectDir(const Vector3D& rVec)
{
	reflectDir = rVec;
}
void DifferentialGeometry::calculateDir(const Vector3D& inDir, const Vector3D& nVec)
{
	normal = nVec;
	reflectDir = inDir - normal * (inDir * normal) * 2;
}
void DifferentialGeometry::calculateDir(const Vector3D& inDir)
{
	reflectDir = inDir - normal * (inDir * normal) * 2;
}
void DifferentialGeometry::setSample(const int& aaS, const int& aaOffset)
{
	sample = aaS;
	sampleOffset = aaOffset;
}
void DifferentialGeometry::setSampleIndex(const int& x, const int& y)
{
	sIndexX = x;
	sIndexY = y;
	int newIndex = (y * sample + x + sampleOffset) % (sample * sample);
	shiftX = newIndex % sample;
	shiftY = newIndex / sample;
}
Float DifferentialGeometry::getDiffuseTheta() const
{
	return lightDir * normal;
}
Float DifferentialGeometry::getSpecularTheta() const
{
	return lightDir * reflectDir;
}*/