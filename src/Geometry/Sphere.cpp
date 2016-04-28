#include "Sphere.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/Texture.h"
#include "Shading/TextureMapping.h"

geoSphere::geoSphere(const Transform* o2w, const Transform* w2o,
	Float radius)
	: Shape(o2w, w2o), r(radius)
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
bool geoSphere::intersect(const Ray& inRay,
	DifferentialGeometry* queryPoint, Float* tHit, Float* rayEpsilon) const
{
	Ray ray = (*WorldToObject)(inRay);
	// (ox + t*dx)^2 + (oy + t*dy)^2 + (oz + t*dz)^2 = r^2
	// A * t^2 + 2B * t + C = 0
	// A = dx^2 + dy^2 + dz^2
	// B = dx*ox + dy*oy + dz*oz
	// C = ox^2 + oy^2 + oz^2 - r^2
	// t = (-B - sqrt(B^2 - A*C)) / A
	//  or (-B + sqrt(B^2 - A*C)) / A
	Float coeA = ray.d.lengthSquared();
	Float coeB = 2 * (ray.d.x * ray.o.x + ray.d.y * ray.o.y + ray.d.z * ray.o.z);
	Float coeC = ray.o.x * ray.o.x + ray.o.y * ray.o.y + ray.o.z * ray.o.z - sqr(r);
	Float t1, t2;
	
	if (!quadratic(coeA, coeB, coeC, t1, t2))//delta > 0
	{
		return false;
	}
	// (tmin, tmax) doesn't overlap (t1, t2)
	if (t1 > ray.tmax || t2 < ray.tmin)
	{
		return false;
	}
	Float tIntersect = t1;
	if (tIntersect < ray.tmin)
	{
		tIntersect = t2; // Always use the nearest intersection
		// (tmin, tmax) inside (t1, t2)
		if (tIntersect > ray.tmax)
		{
			return false;
		}
	}

	*tHit = tIntersect;
	*rayEpsilon = reCE * *tHit;
	queryPoint->shape = this;
	queryPoint->pos = inRay(tIntersect);
	//*queryPoint = DifferentialGeometry(inRay(tIntersect), );
	return true;
}
bool geoSphere::isInside(const Point3f &p) const
{
	if ((p - c).lengthSquared() <= sqr(r))
	{
		return true;
	}
	else
	{
		return false;
	}
}