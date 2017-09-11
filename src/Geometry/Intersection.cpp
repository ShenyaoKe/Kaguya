#include "Geometry/Geometry.h"
#include "Geometry/Intersection.h"

namespace Kaguya
{

Intersection::Intersection(
	const Point3f &_p, const Normal3f &_n,
	const Vector3f &_dpdu, const Vector3f &_dpdv,
	const Normal3f &_dndu, const Normal3f &_dndv,
	const Point2f &_uv, const Geometry* shp)
	: P(_p), Ng(_n)
	, dPdu(_dpdu), dPdv(_dpdv)
	, dNdu(_dndu), dNdv(_dndv), UV(_uv), shape(shp)
{
	shading.N = Ng;
	shading.dPds = dPdu;
	shading.dPdt = dPdv;
	shading.dNds = dNdu;
	shading.dNdt = dNdv;
}

void Intersection::calculateDir(const Vector3f &inDir, const Normal3f &nVec)
{
	Ng = nVec;
	//reflectDir = inDir - Vector3f(normal * Dot(inDir, normal) * 2);
}
void Intersection::calculateDir(const Vector3f &inDir)
{
	//reflectDir = inDir - Vector3f(normal * Dot(inDir, normal) * 2);
}

}
