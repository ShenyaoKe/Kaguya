#include "Geometry/Geometry.h"
#include "Geometry/Intersection.h"

namespace Kaguya
{

Intersection::Intersection(
	const Point3f &p, const Normal3f &n,
	const Vector3f &dPdu, const Vector3f &dPdv,
	const Normal3f &dNdu, const Normal3f &dNdv,
	const Point2f &uv, const Geometry* shape)
	:  mShape(shape)
	, mPos(p), mGeomN(n), mUV(uv)
	, mPu(dPdu), mPv(dPdv)
	, mNu(dNdu), mNv(dNdv)
{
	mShadingN = mGeomN;
	mPs = mPu;
	mPt = mPv;
	mNs = mNu;
	mNt = mNv;
}

void Intersection::calculateDir(const Vector3f &/*inDir*/, const Normal3f &nVec)
{
	mGeomN = nVec;
	//reflectDir = inDir - Vector3f(normal * Dot(inDir, normal) * 2);
}
void Intersection::calculateDir(const Vector3f &/*inDir*/)
{
	//reflectDir = inDir - Vector3f(normal * Dot(inDir, normal) * 2);
}

}
