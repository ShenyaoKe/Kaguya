#include "Hyperboloid.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

/************************************************************************/
/* Hyperboloid Function Definition                                      */
/************************************************************************/
geoHyperboloid::geoHyperboloid()
{
	sa = 1;
	sb = 1;
	sc = 1;
	hbType = ONE_SHEET;
}
geoHyperboloid::geoHyperboloid(const Vector3D &pos, const Float& semiA, const Float& semiB, const Float& semiC, HYPERBOLOID_TYPE newType)
{
	c = pos;
	sa = semiA;
	sb = semiB;
	sc = semiC;
	hbType = newType;
}
geoHyperboloid::~geoHyperboloid()
{
}
void geoHyperboloid::setCenter(const Vector3D &pos)
{
	c = pos;
}
void geoHyperboloid::setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
void geoHyperboloid::setHyperboloidType(HYPERBOLOID_TYPE newType)
{
	hbType = newType;
}
bool geoHyperboloid::intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const
{
	Point3f rp = inRay.o;
	Vector3f rd = inRay.d;// Ray postion and ray direction.	
	Float coeA = (rd.x / sa) * (rd.x / sa) - (rd.y / sb) * (rd.y / sb) + (rd.z / sc) * (rd.z / sc);
	Float coeB = (c.x - rp.x) * rd.x / (sa * sa) - (c.y - rp.y) * rd.y / (sb * sb) + (c.z - rp.z) * rd.z / (sc * sc);
	Float coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) - (rp.y - c.y) * (rp.y - c.y) / (sb * sb) + (rp.z - c.z) * (rp.z - c.z) / (sc * sc) - hbType;

	Float delta = coeB * coeB - coeA * coeC;
	if (delta < 0)
	{
		inRay.setT(NAN, NAN);
		//cout << "No DifferentialGeometry!" << endl;
		return false;
	}
	else//delta > 0
	{
		Float t1 = (coeB - sqrt(delta)) / coeA;
		Float t2 = 2 * coeB / coeA - t1;
		// 		t1 /= coeA;
		// 		t2 /= coeA;
		inRay.setT(t1, t2);
		if (t1 > 0 && t2 > 0)
		{
			*tHit = t1;
			return true;
		}
		else
		{
			return false;
		}
	}
}
Vector3D geoHyperboloid::getNormal(const Vector3D &pos) const
{
	// 2(x-c.x) / sa^2, -1/sb, 2* type *(z - c.z) / sb^2
	return Normalize(Vector3D(
		2 * (pos.x - c.x) / sqr(sa),
		-2 * (pos.y - c.y) / sqr(sb),
		2 * (pos.z - c.z) / sqr(sc)));
}
bool geoHyperboloid::isInside(const Vector3D &pPos) const
{
	return 0;
}