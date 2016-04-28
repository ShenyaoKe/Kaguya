#include "Geometry/Shape.h"
#include "Geometry/DifferentialGeometry.h"

DifferentialGeometry::DifferentialGeometry()
	: shape(nullptr)
{
}
DifferentialGeometry::DifferentialGeometry(const Shape* obj,
	const Point3f &pVec, const Normal3f &nVec,
	const int& sX, const int& sY)
	: shape(obj), pos(pVec), norm(nVec)
	//, lightDist(INFINITY), sample(1), sampleOffset(0)
{
}

DifferentialGeometry::DifferentialGeometry(const Point3f &_p,
	const Vector3f &_dpdu, const Vector3f &_dpdv,
	const Normal3f &_dndu, const Normal3f &_dndv,
	const Point2f &_uv, const Shape* shp)
	: pos(_p), dpdu(_dpdu), dpdv(_dpdv)
	, dndu(_dndu), dndv(_dndv), uv(_uv), shape(shp)
	, norm(Normalize(Cross(dpdu, dpdv)))
{
	shading.n = norm;
	shading.dpdu = dpdu;
	shading.dpdv = dpdv;
	shading.dndu = dndu;
	shading.dndv = dndv;
}

void DifferentialGeometry::calculateDir(const Vector3f &inDir, const Normal3f &nVec)
{
	norm = nVec;
	//reflectDir = inDir - Vector3f(normal * Dot(inDir, normal) * 2);
}
void DifferentialGeometry::calculateDir(const Vector3f &inDir)
{
	//reflectDir = inDir - Vector3f(normal * Dot(inDir, normal) * 2);
}