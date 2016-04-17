#include "Plane.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"


/************************************************************************/
/* Plane Function Definition                                            */
/************************************************************************/
geoPlane::geoPlane()
{
	n = Y_AXIS3D;
	bounding();
}
geoPlane::geoPlane(const Vector3D &pos, const Vector3D &norm)
{
	c = pos;
	n = norm;
	bounding();
}

geoPlane::~geoPlane()
{
}
void geoPlane::setOrigin(const Vector3D &pos)
{
	c = pos;
}
void geoPlane::setNorm(const Vector3D &norm)
{
	n = norm;
}
bool geoPlane::intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const
{
	Float t = this->n * inRay.getDir();
	if (t >= 0)
	{
		//cout << "Ray parallels to the plane or in the plane." << endl;
		return false;
	}
	else
	{
		t = n * (this->c - inRay.getPos()) / t;
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
Vector3D geoPlane::getNormal(const Vector3D &pos) const
{
	return this->n;
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

bool geoPlane::isInside(const Vector3D &pPos) const
{
	if ((pPos - c) * n == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void geoPlane::bounding()
{
	ObjBound = BBox(Point3D(-INFINITY, -INFINITY, -INFINITY), Point3D(INFINITY, INFINITY, INFINITY));
}