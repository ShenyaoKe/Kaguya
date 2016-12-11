#include "Geometry/Shape.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
uint32_t Shape::nextshapeID = 1;

bool Shape::intersectP(const Ray &inRay) const
{
	return ObjBound.intersectP(inRay);
}

void Shape::printInfo() const
{
}