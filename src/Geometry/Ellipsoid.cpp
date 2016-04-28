#include "Ellipsoid.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

/************************************************************************/
/* Ellipsoid Function Definition                                        */
/************************************************************************/
geoEllipsoid::geoEllipsoid()
{
	sa = 1;
	sb = 1;
	sc = 1;
}
geoEllipsoid::geoEllipsoid(const Point3f &pos, const Float& semiA, const Float& semiB, const Float& semiC)
{
	c = pos;
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
void geoEllipsoid::setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
bool geoEllipsoid::intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const
{
	Point3f rp = inRay.o;
	Vector3f rd = inRay.d;// Ray postion and ray direction.
	Float coeA = (rd.x / sa) * (rd.x / sa) + (rd.y / sb) * (rd.y / sb) + (rd.z / sc) * (rd.z / sc);
	Float coeB = (c.x - rp.x) * rd.x / (sa * sa) + (c.y - rp.y) * rd.y / (sb * sb) + (c.z - rp.z) * rd.z / (sc * sc);
	Float coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) + (rp.y - c.y) * (rp.y - c.y) / (sb * sb) + (rp.z - c.z) * (rp.z - c.z) / (sc * sc) - 1;
	Float delta = coeB * coeB - coeA * coeC;
	if (delta < 0)
	{
		//cout << "No DifferentialGeometry!" << endl;
		return false;
	}
	else//delta > 0
	{
		Float t1 = coeB - sqrt(delta);
		Float t2 = 2 * coeB - t1;

		inRay.setT(t1 / coeA, t2 / coeA);
		if (t1 <= 0 || t2 <= 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return 0;
}
/*
Vector3D geoEllipsoid::getNormal(const Point3f &pos) const
{
	// 2(x-c.x) / sa^2, 2(y - c.y) / sb^2, 2(z - c.z) / sc^2
	return Normalize(Vector3D(
		2 * (pos.x - c.x) / (sa * sa),
		2 * (pos.y - c.y) / (sb * sb),
		2 * (pos.z - c.z) / (sc * sc)));
}*/
bool geoEllipsoid::isInside(const Point3f &pPos) const
{
	return 0;
}