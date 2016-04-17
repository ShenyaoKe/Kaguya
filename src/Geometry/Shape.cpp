#include "Geometry/Shape.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
uint32_t Shape::nextshapeId = 1;
Shape::Shape(const Point3D &pos)
	: shapeId(nextshapeId++)
{
}
Shape::~Shape()
{
}
BBox Shape::getWorldBounding() const
{
	return this->ObjBound;
}
void Shape::refine(vector<Shape*> &refined)
{
}
bool Shape::intersectP(const Ray& inRay) const
{
	return ObjBound.intersectP(inRay);
}

Float Shape::area() const
{

}

Float Shape::Pdf() const
{

}

void Shape::getShadingGeometry(const Transform &obj2world, const DifferentialGeometry &dg, DifferentialGeometry *dgShading) const
{

}

bool Shape::isInside(const Vector3D &pPos) const
{
	return false;
}
void Shape::assignShader(Shader* shader)
{
	material = shader;
}
void Shape::assignTextureMapping(TextureMapping* &mapping)
{
	UV_Mapping = mapping;
}
void Shape::assignNormalMap(Texture* nMap)
{
	normalMap = nMap;
}

void Shape::printInfo() const
{

}

bool Shape::getOpacity() const
{
	return material->getOpacity();
}