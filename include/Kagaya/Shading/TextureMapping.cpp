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
TextureMapping::TextureMapping(const Point3D& center)
{
	c = center;
	nx = Vector3D(1, 0, 0);
	ny = Vector3D(0, 1, 0);
	nz = Vector3D(0, 0, 1);
}
TextureMapping::TextureMapping(const Point3D& center, const Vector3D& view, const Vector3D& upVec) : c(center)
{
	setDir(view, upVec);
}
TextureMapping::TextureMapping(const Point3D& center, const Vector3D& xDir, const Vector3D& yDir, const Vector3D& zDir)
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
void TextureMapping::setPos(const Point3D& pos)
{
	c = pos;
}
void TextureMapping::setDir(const Vector3D& view, const Vector3D& upVec)
{
	nz = Normalize(view);
	nx = Normalize(nz.crossMul(upVec));
	ny = Normalize(nx.crossMul(nz));
}
Point3D TextureMapping::posToUV(const Point3D& pos) const
{
	return Point3D();
}
void TextureMapping::mapToUV(const DifferentialGeometry *queryPoint) const
{
	queryPoint->UV = posToUV(queryPoint->pos);
}
void TextureMapping::getUVDir(const DifferentialGeometry *queryPoint) const
{
}
/************************************************************************/
/* Spherical Mapping                                                    */
/************************************************************************/
SphericalMapping2D::SphericalMapping2D() :TextureMapping()
{
}
SphericalMapping2D::SphericalMapping2D(const Point3D& center) : TextureMapping(center)
{
}
SphericalMapping2D::SphericalMapping2D(const Point3D& center, const Vector3D& view, const Vector3D& upVec) : TextureMapping(center, view, upVec)
{
}
SphericalMapping2D::SphericalMapping2D(const Point3D& center, const Vector3D& xDir, const Vector3D& yDir, const Vector3D& zDir) : TextureMapping(center, xDir, yDir, zDir)
{
}
SphericalMapping2D::~SphericalMapping2D()
{
}
Point3D SphericalMapping2D::posToUV(const Point3D& pos) const
{
	Vector3D np = Normalize(pos - c);
	double x = nx * np;
	double y = ny * np;
	double z = nz * np;

	double v = acos(clamp(z, -1.0, 1.0)) / M_PI;
	double u = acos(clamp(y / sqrt(1 - sqr(z)), -1.0, 1.0)) / (2 * M_PI);
	if (z == 1 || z == -1)
	{
		u = unitRandom(20);
	}
	if (x > 0)
	{
		//cout << nx.getLenSq() << endl;
		u = 1 - u;
	}
	Point3D ret = Vector3D(u, v, 0);
	return ret;
	//queryPoint->UV.printInfo();
}
void SphericalMapping2D::getUVDir(const DifferentialGeometry *queryPoint) const
{
	double threshold = 0.005;
	Vector3D uPos, vPos;
	uPos.x = sin(M_PI * queryPoint->UV.y) * sin(2 * M_PI * (queryPoint->UV.x + threshold));
	uPos.y = sin(M_PI * queryPoint->UV.y) * cos(2 * M_PI * (queryPoint->UV.x + threshold));
	uPos.z = cos(M_PI * queryPoint->UV.y);
	uPos = c + uPos.x * nx + uPos.y * ny + uPos.z * nz - queryPoint->pos;

	vPos.x = sin(M_PI * queryPoint->UV.y + threshold) * sin(2 * M_PI*(queryPoint->UV.x));
	vPos.y = sin(M_PI * queryPoint->UV.y + threshold) * cos(2 * M_PI*(queryPoint->UV.x));
	vPos.z = cos(M_PI * queryPoint->UV.y + threshold);
	vPos = c + vPos.x * nx + vPos.y * ny + vPos.z * nz - queryPoint->pos;

	queryPoint->uDir = Normalize(uPos - uPos * queryPoint->normal * uPos);
	queryPoint->vDir = Normalize(vPos - vPos * queryPoint->normal * vPos);
}
/************************************************************************/
/* Planar Mapping                                                       */
/************************************************************************/
PlanarMapping2D::PlanarMapping2D() :TextureMapping()
{
}
PlanarMapping2D::PlanarMapping2D(const double& us, const double& vs) : TextureMapping()
{
	setSize(us, vs);
}
PlanarMapping2D::PlanarMapping2D(const Point3D& center) : TextureMapping(center)
{
}
PlanarMapping2D::PlanarMapping2D(const Point3D& center, const double& us, const double& vs) : TextureMapping(center)
{
	setSize(us, vs);
}
PlanarMapping2D::PlanarMapping2D(const Point3D& center, const Vector3D& view, const Vector3D& upVec) : TextureMapping(center, view, upVec)
{
}
PlanarMapping2D::PlanarMapping2D(const Point3D& center, const Vector3D& xDir, const Vector3D& yDir, const Vector3D& zDir) : TextureMapping(center, xDir, yDir, zDir)
{
}
PlanarMapping2D::~PlanarMapping2D()
{
}
void PlanarMapping2D::setSize(const double& us, const double& vs)
{
	uSize = us;
	vSize = vs;
}
void PlanarMapping2D::mapToUV(const DifferentialGeometry *queryPoint) const
{
	//cout << uSize << endl;
	Vector3D np = queryPoint->pos - c;
	//c.printInfo();
	double x = nx * np / uSize + 0.5;
	double y = ny * np / vSize + 0.5;

	queryPoint->UV = Vector3D(x, y, 0);
}
/************************************************************************/
/* Planar Mapping 3D                                                    */
/************************************************************************/
PlanarMapping3D::PlanarMapping3D() :TextureMapping()
{
}
PlanarMapping3D::PlanarMapping3D(const double& us, const double& vs, const double& d) : TextureMapping()
{
	setSize(us, vs, d);
}
PlanarMapping3D::PlanarMapping3D(const Point3D& center) : TextureMapping(center)
{
}
PlanarMapping3D::PlanarMapping3D(const Point3D& center, const double& us, const double& vs, const double& d) : TextureMapping(center)
{
	setSize(us, vs, d);
}
PlanarMapping3D::PlanarMapping3D(const Point3D& center, const Vector3D& view, const Vector3D& upVec) : TextureMapping(center, view, upVec)
{
}
PlanarMapping3D::PlanarMapping3D(const Point3D& center, const Vector3D& xDir, const Vector3D& yDir, const Vector3D& zDir) : TextureMapping(center, xDir, yDir, zDir)
{
}
PlanarMapping3D::~PlanarMapping3D()
{
}
void PlanarMapping3D::setSize(const double& us, const double& vs, const double& d)
{
	uSize = us;
	vSize = vs;
	dist = d;
}
void PlanarMapping3D::mapToUV(const DifferentialGeometry *queryPoint) const
{
	//cout << uSize << endl;
	Vector3D np = queryPoint->pos - c;
	//c.printInfo();
	double x = nx * np / uSize + 0.5;
	double y = ny * np / vSize + 0.5;
	double z = fmod(nz * np / dist, 1);

	queryPoint->UV = Vector3D(x, y, z);
}