#include "Torus.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

/************************************************************************/
/* Torus Fuction Definition                                             */
/************************************************************************/
geoTorus::geoTorus(const Vector3D &pos, const Float& radius, const Float& secRadius)
{
	c = pos;
	r = radius;
	sr = secRadius;
}
geoTorus::~geoTorus()
{

}
void geoTorus::setCenter(const Vector3D &pos)
{
	c = pos;
}
void geoTorus::setRadius(Float radius)
{
	r = radius;
}
void geoTorus::setSecRadius(Float secRadius)
{
	sr = secRadius;
}
bool geoTorus::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const
{
	//Float coeA = inRay.getDir() * inRay.getDir();//len == 1
	Float coeB = inRay.getDir() * (inRay.getPos() - c);
	Float coeC = (inRay.getPos() - c).getLenSq() - r * r;
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
bool geoTorus::isInside(const Vector3D &pPos) const
{
	Float tmp = (pPos - c).getLenSq() - (r * r + sr * sr);
	if (tmp * tmp - 4 * r * r * (sr * sr - (pPos.y - c.y) * (pPos.y - c.y)) <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

