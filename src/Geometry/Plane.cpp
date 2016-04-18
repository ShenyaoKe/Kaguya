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

void geoPlane::getNormal(const DifferentialGeometry* queryPoint) const
{
	queryPoint->normal = getNormal(queryPoint->pos);

	if (normalMap != nullptr && UV_Mapping != nullptr)
	{
		UV_Mapping->getUVDir(queryPoint);
		ColorRGBA tmpNormal = normalMap->getColor(queryPoint) * 2 - ColorRGBA(1, 1, 1, 1);
		//tmpNormal.printInfo();
		queryPoint->normal = Normalize(
			-queryPoint->dpdu * tmpNormal.r
			- queryPoint->dpdv * tmpNormal.g
			+ queryPoint->normal * tmpNormal.b);
	}
	else
	{
		queryPoint->dpdu = X_AXIS3D;// Vector3D(2 * PI * pp.z, 0, 2 * PI * pp.x).getNorm()
		queryPoint->dpdv = Z_AXIS3D;

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