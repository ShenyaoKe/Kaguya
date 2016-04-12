#include "Shading/TextureMapping.h"
#include "Geometry/DifferentialGeometry.h"

TextureMapping::TextureMapping()
{
	nx = Vector3D(0, 0, 1);
	ny = Vector3D(0, 1, 0);
	nz = Vector3D(1, 0, 0);
}
TextureMapping::~TextureMapping()
{
}
TextureMapping::TextureMapping(const Vector3D &center)
{
	c = center;
	nx = Vector3D(1, 0, 0);
	ny = Vector3D(0, 1, 0);
	nz = Vector3D(0, 0, 1);
}
TextureMapping::TextureMapping(const Vector3D &center, const Vector3D &view, const Vector3D &upVec) : c(center)
{
	setDir(view, upVec);
}
TextureMapping::TextureMapping(const Vector3D &center, const Vector3D &xDir, const Vector3D &yDir, const Vector3D &zDir)
{
	c = center;
	nx = Normalize(xDir);
	ny = Normalize(yDir);
	nz = Normalize(zDir);
}
void TextureMapping::printInfo() const
{
	c.printInfo(); nx.printInfo();
	ny.printInfo(); nz.printInfo();
}
void TextureMapping::setPos(const Vector3D &pos)
{
	c = pos;
}
void TextureMapping::setDir(const Vector3D &view, const Vector3D &upVec)
{
	nz = Normalize(view);
	nx = Normalize(nz.crossMul(upVec));
	ny = Normalize(nx.crossMul(nz));
}
Point3D TextureMapping::posToUV(const Vector3D &pos) const
{
	return Point3D();
}
void TextureMapping::mapToUV(const DifferentialGeometry* queryPoint) const
{
	queryPoint->UV = posToUV(queryPoint->pos);
}
void TextureMapping::getUVDir(const DifferentialGeometry* queryPoint) const
{
}
/************************************************************************/
/* Spherical Mapping                                                    */
/************************************************************************/
SphericalMapping2D::SphericalMapping2D() :TextureMapping()
{
}
SphericalMapping2D::SphericalMapping2D(const Vector3D &center) : TextureMapping(center)
{
}
SphericalMapping2D::SphericalMapping2D(const Vector3D &center, const Vector3D &view, const Vector3D &upVec) : TextureMapping(center, view, upVec)
{
}
SphericalMapping2D::SphericalMapping2D(const Vector3D &center, const Vector3D &xDir, const Vector3D &yDir, const Vector3D &zDir) : TextureMapping(center, xDir, yDir, zDir)
{
}
SphericalMapping2D::~SphericalMapping2D()
{
}
Point3D SphericalMapping2D::posToUV(const Vector3D &pos) const
{
	Vector3D np = Normalize(pos - c);
	Float x = nx * np;
	Float y = ny * np;
	Float z = nz * np;

	Float v = acos(clamp(z, -1.0, 1.0)) / M_PI;
	Float u = acos(clamp(y / sqrt(1.0 - sqr(z)), -1.0, 1.0)) / (2.0 * M_PI);
	if (z == 1 || z == -1)
	{
		u = unitRandom(20);
	}
	if (x > 0)
	{
		//cout << nx.getLenSq() << endl;
		u = 1.0 - u;
	}
	Point3D ret = Vector3D(u, v, 0);
	return ret;
	//queryPoint->UV.printInfo();
}
void SphericalMapping2D::getUVDir(const DifferentialGeometry* queryPoint) const
{
	Float threshold = 0.005;
	Vector3D uPos, vPos;
	uPos.x = sin(M_PI * queryPoint->UV.y) * sin(2 * M_PI * (queryPoint->UV.x + threshold));
	uPos.y = sin(M_PI * queryPoint->UV.y) * cos(2 * M_PI * (queryPoint->UV.x + threshold));
	uPos.z = cos(M_PI * queryPoint->UV.y);
	uPos = c + uPos.x * nx + uPos.y * ny + uPos.z * nz - queryPoint->pos;

	vPos.x = sin(M_PI * queryPoint->UV.y + threshold) * sin(2 * M_PI*(queryPoint->UV.x));
	vPos.y = sin(M_PI * queryPoint->UV.y + threshold) * cos(2 * M_PI*(queryPoint->UV.x));
	vPos.z = cos(M_PI * queryPoint->UV.y + threshold);
	vPos = c + vPos.x * nx + vPos.y * ny + vPos.z * nz - queryPoint->pos;

	queryPoint->dpdu = Normalize(uPos - uPos * queryPoint->normal * uPos);
	queryPoint->dpdv = Normalize(vPos - vPos * queryPoint->normal * vPos);
}
/************************************************************************/
/* Planar Mapping                                                       */
/************************************************************************/
PlanarMapping2D::PlanarMapping2D() :TextureMapping()
{
}
PlanarMapping2D::PlanarMapping2D(const Float& us, const Float& vs) : TextureMapping()
{
	setSize(us, vs);
}
PlanarMapping2D::PlanarMapping2D(const Vector3D &center) : TextureMapping(center)
{
}
PlanarMapping2D::PlanarMapping2D(const Vector3D &center, const Float& us, const Float& vs) : TextureMapping(center)
{
	setSize(us, vs);
}
PlanarMapping2D::PlanarMapping2D(const Vector3D &center, const Vector3D &view, const Vector3D &upVec) : TextureMapping(center, view, upVec)
{
}
PlanarMapping2D::PlanarMapping2D(const Vector3D &center, const Vector3D &xDir, const Vector3D &yDir, const Vector3D &zDir) : TextureMapping(center, xDir, yDir, zDir)
{
}
PlanarMapping2D::~PlanarMapping2D()
{
}
void PlanarMapping2D::setSize(const Float& us, const Float& vs)
{
	uSize = us;
	vSize = vs;
}
void PlanarMapping2D::mapToUV(const DifferentialGeometry* queryPoint) const
{
	//cout << uSize << endl;
	Vector3D np = queryPoint->pos - c;
	//c.printInfo();
	Float x = nx * np / uSize + 0.5;
	Float y = ny * np / vSize + 0.5;

	queryPoint->UV = Vector3D(x, y, 0);
}
/************************************************************************/
/* Planar Mapping 3D                                                    */
/************************************************************************/
PlanarMapping3D::PlanarMapping3D() :TextureMapping()
{
}
PlanarMapping3D::PlanarMapping3D(const Float& us, const Float& vs, const Float& d) : TextureMapping()
{
	setSize(us, vs, d);
}
PlanarMapping3D::PlanarMapping3D(const Vector3D &center) : TextureMapping(center)
{
}
PlanarMapping3D::PlanarMapping3D(const Vector3D &center, const Float& us, const Float& vs, const Float& d) : TextureMapping(center)
{
	setSize(us, vs, d);
}
PlanarMapping3D::PlanarMapping3D(const Vector3D &center, const Vector3D &view, const Vector3D &upVec) : TextureMapping(center, view, upVec)
{
}
PlanarMapping3D::PlanarMapping3D(const Vector3D &center, const Vector3D &xDir, const Vector3D &yDir, const Vector3D &zDir) : TextureMapping(center, xDir, yDir, zDir)
{
}
PlanarMapping3D::~PlanarMapping3D()
{
}
void PlanarMapping3D::setSize(const Float& us, const Float& vs, const Float& d)
{
	uSize = us;
	vSize = vs;
	dist = d;
}
void PlanarMapping3D::mapToUV(const DifferentialGeometry* queryPoint) const
{
	//cout << uSize << endl;
	Vector3D np = queryPoint->pos - c;
	//c.printInfo();
	Float x = nx * np / uSize + 0.5;
	Float y = ny * np / vSize + 0.5;
	Float z = fmod(nz * np / dist, 1);

	queryPoint->UV = Vector3D(x, y, z);
}