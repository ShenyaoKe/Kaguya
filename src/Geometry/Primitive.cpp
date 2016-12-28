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

uint32_t Primitive::nextshapeID = 1;

bool Primitive::intersectP(const Ray &inRay) const
{
    return ObjBound.intersectP(inRay);
}

void Primitive::printInfo() const
{
}

}
