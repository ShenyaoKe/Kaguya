#include "Geometry/Shape.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
int Shape::uid = 1;
Shape::Shape(const Point3D &pos)
	: c(pos)
{
}
Shape::~Shape()
{
}
void Shape::bounding()
{
	//return;
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
bool Shape::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const
{
	queryPoint->object = this;
	return false;
}
Vector3D Shape::getNormal(const Vector3D &pos) const
{
	return Vector3D(0, 0, -1);
}
void Shape::getNormal(const DifferentialGeometry* queryPoint) const
{
	queryPoint->normal = getNormal(queryPoint->pos);

	if (normalMap != nullptr && UV_Mapping != nullptr)
	{
		UV_Mapping->getUVDir(queryPoint);
		ColorRGBA tmpNormal = normalMap->getColor(queryPoint) * 2 - ColorRGBA(1, 1, 1, 1);
		//tmpNormal.printInfo();
		queryPoint->normal = Normalize(
			- queryPoint->dpdu * tmpNormal.r
			- queryPoint->dpdv * tmpNormal.g
			+ queryPoint->normal * tmpNormal.b);
	}
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

int Shape::getIndex() const
{
	return index;
}

void Shape::printInfo() const
{
	cout << "Center:\t";
	c.printInfo();
}
void Shape::getUV(const DifferentialGeometry* queryPoint) const
{
	if (UV_Mapping != nullptr)
	{
		UV_Mapping->mapToUV(queryPoint);
	}
}
ColorRGBA Shape::getAmbient(const DifferentialGeometry* queryPoint) const
{
	ColorRGBA ret;
	if (material != nullptr)
	{
		ret = material->getAmbient(queryPoint);
	}
	return ret;
}
ColorRGBA Shape::getDiffuse(const DifferentialGeometry* queryPoint, const Light* light) const
{
	ColorRGBA ret = material->getDiffuseAt(queryPoint, light);
	return ret;
}
ColorRGBA Shape::getColor(const DifferentialGeometry* queryPoint, const Light* light) const
{
	ColorRGBA ret = material->getColor(queryPoint, light);
	return ret;
}
bool Shape::getOpacity() const
{
	return material->getOpacity();
}

const Vector3D &Shape::closestPoint(const Point3D &p) const
{
	return this->c;
}

Vector3D Shape::getCenter() const
{
	return this->c;
}

/************************************************************************/
/* Sphere Function Definition                                           */
/************************************************************************/




