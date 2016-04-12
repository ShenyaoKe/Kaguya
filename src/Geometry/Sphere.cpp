#include "Sphere.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/Texture.h"
#include "Shading/TextureMapping.h"

geoSphere::geoSphere(const Vector3D &pos, const Float& radius)
	: Shape(pos), r(radius)
{
	bounding();
}
geoSphere::~geoSphere()
{

}
void geoSphere::bounding()
{
	ObjBound = BBox(c);
	ObjBound.expand(r);
	//return false;
}

BBox geoSphere::getWorldBounding() const
{
	BBox ret(c);
	ret.expand(r);
	return ret;
}

void geoSphere::setCenter(const Vector3D &pos)
{
	c = pos;
	bounding();
}
void geoSphere::setRadius(Float radius)
{
	r = radius;
	bounding();
}
bool geoSphere::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const
{
	Float coeB = inRay.getDir() * (c - inRay.getPos());
	Float coeC = (inRay.getPos() - c).getLenSq() - sqr(r);
	Float delta = sqr(coeB) - coeC;
	if (delta > 0)//delta > 0
	{
		Float t1 = coeB - sqrt(delta);
		Float t2 = 2 * coeB - t1;
		//inRay.setT(t1, t2);
		if (t1 <= 0 && t2 <= 0)
		{
			return false;
		}
		else if (t1 > 0)
		{
			*tHit = t1;
		}
		else
		{
			*tHit = t2;
		}
		if (*tHit > inRay.tmin && *tHit < inRay.tmax)
		{
			*rayEpsilon = reCE * *tHit;
			return true;
		}
	}
	return false;
}
Vector3D geoSphere::getNormal(const Vector3D &pos) const
{
	return Normalize(pos - c);
}

void geoSphere::getNormal(const DifferentialGeometry* queryPoint) const
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
		Point3D pp = queryPoint->pos - c;
		Float rsintheta = r * sqrt(1 - sqr(pp.y / r));
		if (rsintheta == 0.0)
		{
			queryPoint->dpdu = X_AXIS3D;
			queryPoint->dpdv = Z_AXIS3D;
		}
		else
		{
			queryPoint->dpdu = Normalize(Vector3D(-pp.z, 0, pp.x));// Vector3D(2 * PI * pp.z, 0, 2 * PI * pp.x).getNorm()
			queryPoint->dpdv = Normalize(-Vector3D(pp.x * pp.y / rsintheta, -rsintheta, pp.y * pp.z / rsintheta));
		}
	}
}

Float geoSphere::getRadius() const
{
	return r;
}

bool geoSphere::isInside(const Vector3D &pPos) const
{
	if ((pPos - c).getLenSq() <= sqr(r))
	{
		return true;
	}
	else
	{
		return false;
	}
}