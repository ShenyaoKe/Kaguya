#include "Torus.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/Intersection.h"

namespace Kaguya
{

/************************************************************************/
/* Torus Fuction Definition                                             */
/************************************************************************/
geoTorus::geoTorus(const Point3f &pos, Float radius, Float secRadius)
{
	c = pos;
	r = radius;
	sr = secRadius;
}

bool geoTorus::intersect(const Ray &inRay, Intersection* isec, Float* tHit, Float* rayEpsilon) const
{
	//Float coeA = inRay.d * inRay.d;//len == 1
	Float coeB = dot(inRay.d, (inRay.o - c));
	Float coeC = (inRay.o - c).lengthSquared() - r * r;
	//Float delta = coeB * coeB - 4 * coeA * coeB;
	Float delta = coeB * coeB - coeC;
	if (delta < 0)
	{
		//std::cout << "No Intersection!" << std::endl;
		return false;
	}
	else if (delta == 0)
	{
		//std::cout << "haha" << std::endl;
		return true;
	}
	else//delta > 0
	{
		Float t1 = coeB - sqrt(delta);
		//Float t2 = 2 * coeB - t1;
		// 		if (t1 <= 0)
		// 		{
		// 			std::cout << "No!" << std::endl;
		// 			return nullptrptr;
		// 		}
		// 		else
		// 		{
		// 			//std::cout << t1 << std::endl;
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

}
