#include "Geometry/Primitive.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
namespace Kaguya
{

uint32_t Primitive::nextshapeID = 0;

bool Primitive::intersectP(const Ray &inRay) const
{
	return mObjBound.intersectP(inRay);
}

void Primitive::printInfo(const std::string &msg) const
{
	std::cout << msg << std::endl;
}

}
