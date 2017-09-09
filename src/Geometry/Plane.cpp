#include "Plane.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/Intersection.h"

namespace Kaguya
{

/************************************************************************/
/* Plane Function Definition                                            */
/************************************************************************/
geoPlane::geoPlane()
	: n(0, 1, 0)
{
	bounding();
}
geoPlane::geoPlane(const Point3f &pos, const Normal3f &norm,
				   Float w, Float h)
	: p(pos), n(norm)
	, width(w), height(h)
{
	bounding();
}

bool geoPlane::intersect(const Ray &inRay, Intersection* isec, Float* tHit, Float* rayEpsilon) const
{
	Float t = dot(this->n, inRay.d);
	if (t >= 0)
	{
		//std::cout << "Ray parallels to the plane or in the plane." << std::endl;
		return false;
	}
	else
	{
		t = dot(n, (p - inRay.o)) / t;
		if (t > 0 && t > inRay.tMin && t < inRay.tMax)
		{

			//inRay.setT(t, INFINITY);
			*tHit = t;
			*rayEpsilon = reCE * *tHit;
			return true;
		}
		else
		{
			return false;
		}
	}
}

void geoPlane::bounding()
{
	// TODO: Plane bouding method
	mObjBound = Bounds3f(Point3f(-sNumInfinity, -sNumInfinity, -sNumInfinity),
						 Point3f(sNumInfinity, sNumInfinity, sNumInfinity));
}

}
