#include "Geometry/Shape.h"
#include "Geometry/DifferentialGeometry.h"

DifferentialGeometry::DifferentialGeometry(
	const Point3f &_p, const Normal3f &_n,
	const Vector3f &_dpdu, const Vector3f &_dpdv,
	const Normal3f &_dndu, const Normal3f &_dndv,
	const Point2f &_uv, const Shape* shp)
	: P(_p), Ng(_n)
	, dPdu(_dpdu), dPdv(_dpdv)
	, dNdu(_dndu), dNdv(_dndv), uv(_uv), shape(shp)
{
	shading.n = Ng;
	shading.dPdu = dPdu;
	shading.dPdv = dPdv;
	shading.dNdu = dNdu;
	shading.dNdv = dNdv;
}

void DifferentialGeometry::calculateDir(const Vector3f &inDir, const Normal3f &nVec)
{
	Ng = nVec;
	//reflectDir = inDir - Vector3f(normal * Dot(inDir, normal) * 2);
}
void DifferentialGeometry::calculateDir(const Vector3f &inDir)
{
	//reflectDir = inDir - Vector3f(normal * Dot(inDir, normal) * 2);
}