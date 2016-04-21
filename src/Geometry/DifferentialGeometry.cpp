#include "Geometry/Shape.h"
#include "Geometry/DifferentialGeometry.h"

DifferentialGeometry::DifferentialGeometry()
	: object(nullptr), sIndexX(0), sIndexY(0)
	, sample(1), sampleOffset(0)
{
}
DifferentialGeometry::DifferentialGeometry(const Shape* obj,
	const Point3f &pVec, const Normal3f &nVec,
	const Vector3f &rVec, const int& sX, const int& sY)
	: object(obj), pos(pVec), normal(nVec), reflectDir(rVec)
	, sIndexX(sX), sIndexY(sY)
	//, lightDist(INFINITY), sample(1), sampleOffset(0)
{
}

void DifferentialGeometry::calculateDir(const Vector3f &inDir, const Normal3f &nVec)
{
	normal = nVec;
	reflectDir = inDir - Vector3f(normal * Dot(inDir, normal) * 2);
}
void DifferentialGeometry::calculateDir(const Vector3f &inDir)
{
	reflectDir = inDir - Vector3f(normal * Dot(inDir, normal) * 2);
}