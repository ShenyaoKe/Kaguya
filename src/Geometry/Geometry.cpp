#include "Geometry/Geometry.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/Intersection.h"

/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
namespace Kaguya
{

uint32_t Geometry::sNextGeomID = 0;

bool Geometry::intersectP(const Ray &inRay) const
{
	return mObjBound.intersectP(inRay);
}

void Geometry::printInfo(const std::string &msg) const
{
	std::cout << msg << std::endl;
}

}
