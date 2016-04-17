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
bool geoSphere::intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const
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