#include "Shading/TextureMapping.h"
#include "Geometry/Intersection.h"

namespace Kaguya
{

TextureMapping::TextureMapping()
{
	nx = Vector3f(0, 0, 1);
	ny = Vector3f(0, 1, 0);
	nz = Vector3f(1, 0, 0);
}
TextureMapping::~TextureMapping()
{
}
TextureMapping::TextureMapping(const Point3f &center)
{
	c = center;
	nx = Vector3f(1, 0, 0);
	ny = Vector3f(0, 1, 0);
	nz = Vector3f(0, 0, 1);
}
TextureMapping::TextureMapping(const Point3f &center, const Vector3f &view, const Vector3f &upVec)
	: c(center)
{
	setDir(view, upVec);
}
TextureMapping::TextureMapping(const Point3f &center, const Vector3f &xDir, const Vector3f &yDir, const Vector3f &zDir)
	: c(center)
{
	nx = normalize(xDir);
	ny = normalize(yDir);
	nz = normalize(zDir);
}
void TextureMapping::printInfo() const
{
	std::cout << "Center:\t" << c
		<< "\nNx:\t" << nx
		<< "\nNy:\t" << ny
		<< "\nNz:\t" << nz << std::endl;
}
void TextureMapping::setPos(const Point3f &pos)
{
	c = pos;
}
void TextureMapping::setDir(const Vector3f &view, const Vector3f &upVec)
{
	nz = normalize(view);
	nx = normalize(cross(nz, upVec));
	ny = normalize(cross(nx, nz));
}

Point2f TextureMapping::posToUV(const Point3f &/*pos*/) const
{
	return Point2f();
}

/************************************************************************/
/* Spherical Mapping                                                    */
/************************************************************************/
SphericalMapping2D::SphericalMapping2D() :TextureMapping()
{
}
SphericalMapping2D::SphericalMapping2D(const Point3f &center)
	: TextureMapping(center)
{
}
SphericalMapping2D::SphericalMapping2D(const Point3f &center, const Vector3f &view, const Vector3f &upVec)
	: TextureMapping(center, view, upVec)
{
}
SphericalMapping2D::SphericalMapping2D(const Point3f &center, const Vector3f &xDir, const Vector3f &yDir, const Vector3f &zDir)
	: TextureMapping(center, xDir, yDir, zDir)
{
}
SphericalMapping2D::~SphericalMapping2D()
{
}

Point2f SphericalMapping2D::posToUV(const Point3f &pos) const
{
	Vector3f np = normalize(pos - c);
	Float x = dot(nx, np);
	Float y = dot(ny, np);
	Float z = dot(nz, np);

	Float v = acos(clamp(z, -1, 1)) / M_PI;
	Float u = acos(clamp(y / sqrt(1.0 - sqr(z)), -1, 1)) / (2.f * M_PI);
	if (z == 1 || z == -1)
	{
		u = unitRandom(20);
	}
	if (x > 0)
	{
		u = 1.0 - u;
	}
	return Point2f(u, v);
}
/*void SphericalMapping2D::getUVDir(const Intersection* isec) const
{
Float threshold = 0.005;
Vector3f uPos, vPos;
uPos.x = sin(M_PI * isec->UV.y) * sin(2 * M_PI * (isec->UV.x + threshold));
uPos.y = sin(M_PI * isec->UV.y) * cos(2 * M_PI * (isec->UV.x + threshold));
uPos.z = cos(M_PI * isec->UV.y);
uPos = c + nx * uPos.x + ny * uPos.y + nz * uPos.z - isec->pos;

vPos.x = sin(M_PI * isec->UV.y + threshold) * sin(2 * M_PI*(isec->UV.x));
vPos.y = sin(M_PI * isec->UV.y + threshold) * cos(2 * M_PI*(isec->UV.x));
vPos.z = cos(M_PI * isec->UV.y + threshold);
vPos = c + nx * vPos.x + ny * vPos.y + nz * vPos.z - isec->pos;

isec->dpdu = Normalize(uPos - uPos * Dot(uPos, isec->normal));
isec->dpdv = Normalize(vPos - vPos * Dot(vPos, isec->normal));
}
*/
/************************************************************************/
/* Planar Mapping                                                       */
/************************************************************************/
PlanarMapping2D::PlanarMapping2D() :TextureMapping()
{
}
PlanarMapping2D::PlanarMapping2D(Float us, Float vs) : TextureMapping()
{
	setSize(us, vs);
}
PlanarMapping2D::PlanarMapping2D(const Point3f &center)
	: TextureMapping(center)
{
}
PlanarMapping2D::PlanarMapping2D(const Point3f &center, Float us, Float vs)
	: TextureMapping(center)
{
	setSize(us, vs);
}
PlanarMapping2D::PlanarMapping2D(const Point3f &center, const Vector3f &view, const Vector3f &upVec)
	: TextureMapping(center, view, upVec)
{
}
PlanarMapping2D::PlanarMapping2D(const Point3f &center, const Vector3f &xDir, const Vector3f &yDir, const Vector3f &zDir)
	: TextureMapping(center, xDir, yDir, zDir)
{
}
PlanarMapping2D::~PlanarMapping2D()
{
}
void PlanarMapping2D::setSize(Float us, Float vs)
{
	uSize = us;
	vSize = vs;
}
/*
void PlanarMapping2D::mapToUV(const Intersection* isec) const
{
//std::cout << uSize << std::endl;
Vector3f np = isec->pos - c;
//c.printInfo();
Float x = Dot(nx, np) / uSize + 0.5;
Float y = Dot(ny, np) / vSize + 0.5;

isec->UV = Point3f(x, y, 0);
}
*/

}
