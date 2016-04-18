#include "Plane.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"


/************************************************************************/
/* Plane Function Definition                                            */
/************************************************************************/
geoPlane::geoPlane()
	: n(0., 1., 0.)
{
	//n = Y_AXIS3D;
	bounding();
}
geoPlane::geoPlane(const Point3f &pos, const Normal3f &norm)
{
	c = pos;
	n = norm;
	bounding();
}

bool geoPlane::intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const
{
	Float t = Dot(this->n, inRay.d);
	if (t >= 0)
	{
		//cout << "Ray parallels to the plane or in the plane." << endl;
		return false;
	}
	else
	{
		t = Dot(n, (this->c - inRay.o)) / t;
		if (t > 0 && t > inRay.tmin && t < inRay.tmax)
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

/*
bool geoPlane::isInside(const Point3f &pPos) const
{
	if ((pPos - c) * n == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}*/

void geoPlane::bounding()
{
	ObjBound = BBox(Point3f(-INFINITY, -INFINITY, -INFINITY), Point3f(INFINITY, INFINITY, INFINITY));
}