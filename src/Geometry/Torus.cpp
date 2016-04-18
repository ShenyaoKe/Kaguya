#include "Torus.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

/************************************************************************/
/* Torus Fuction Definition                                             */
/************************************************************************/
geoTorus::geoTorus(const Point3f &pos, const Float& radius, const Float& secRadius)
{
	c = pos;
	r = radius;
	sr = secRadius;
}

bool geoTorus::intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const
{
	//Float coeA = inRay.d * inRay.d;//len == 1
	Float coeB = Dot(inRay.d, (inRay.o - c));
	Float coeC = (inRay.o - c).lengthSquared() - r * r;
	//Float delta = coeB * coeB - 4 * coeA * coeB;
	Float delta = coeB * coeB - coeC;
	if (delta < 0)
	{
		//cout << "No DifferentialGeometry!" << endl;
		return false;
	}
	else if (delta == 0)
	{
		//cout << "haha" << endl;
		return true;
	}
	else//delta > 0
	{
		Float t1 = coeB - sqrt(delta);
		//Float t2 = 2 * coeB - t1;
		// 		if (t1 <= 0)
		// 		{
		// 			cout << "No!" << endl;
		// 			return nullptrptr;
		// 		}
		// 		else
		// 		{
		// 			//cout << t1 << endl;
		// 			return t1;
		// 		}
		return true;
	}
}
bool geoTorus::isInside(const Point3f &pPos) const
{
	Float tmp = (pPos - c).lengthSquared() - (r * r + sr * sr);
	if (tmp * tmp - 4 * r * r * (sr * sr - (pPos.y - c.y) * (pPos.y - c.y)) <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

