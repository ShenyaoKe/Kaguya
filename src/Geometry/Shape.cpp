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
bool Shape::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const
{
	queryPoint->object = this;
	return false;
}
Vector3D Shape::getNormal(const Vector3D& pos) const
{
	return Vector3D(0, 0, -1);
}
void Shape::getNormal(const DifferentialGeometry *queryPoint) const
{
	queryPoint->normal = getNormal(queryPoint->pos);

	if (normalMap != NULL && UV_Mapping != NULL)
	{
		UV_Mapping->getUVDir(queryPoint);
		ColorRGBA tmpNormal = normalMap->getColor(queryPoint) * 2 - ColorRGBA(1, 1, 1, 1);
		//tmpNormal.printInfo();
		queryPoint->normal = Normalize(
			- queryPoint->uDir * tmpNormal.r
			- queryPoint->vDir * tmpNormal.g
			+ queryPoint->normal * tmpNormal.b);
	}
}
bool Shape::isInside(const Vector3D& pPos) const
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
void Shape::getUV(const DifferentialGeometry *queryPoint) const
{
	if (UV_Mapping != NULL)
	{
		UV_Mapping->mapToUV(queryPoint);
	}
}
ColorRGBA Shape::getAmbient(const DifferentialGeometry *queryPoint) const
{
	ColorRGBA ret;
	if (material != nullptr)
	{
		ret = material->getAmbient(queryPoint);
	}
	return ret;
}
ColorRGBA Shape::getDiffuse(const DifferentialGeometry *queryPoint, const Light* light) const
{
	ColorRGBA ret = material->getDiffuseAt(queryPoint, light);
	return ret;
}
ColorRGBA Shape::getColor(const DifferentialGeometry *queryPoint, const Light* light) const
{
	ColorRGBA ret = material->getColor(queryPoint, light);
	return ret;
}
bool Shape::getOpacity() const
{
	return material->getOpacity();
}

const Point3D& Shape::closestPoint(const Point3D &p) const
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


/************************************************************************/
/* Plane Function Definition                                            */
/************************************************************************/
geoPlane::geoPlane()
{
	n = Y_AXIS3D;
	bounding();
}
geoPlane::geoPlane(const Vector3D& pos, const Vector3D& norm)
{
	c = pos;
	n = norm;
	bounding();
}

geoPlane::~geoPlane()
{
}
void geoPlane::setOrigin(const Vector3D& pos)
{
	c = pos;
}
void geoPlane::setNorm(const Vector3D& norm)
{
	n = norm;
}
bool geoPlane::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const
{
	Float t = this->n * inRay.getDir();
	if (t >= 0)
	{
		//cout << "Ray parallels to the plane or in the plane." << endl;
		return false;
	}
	else
	{
		t = n * (this->c - inRay.getPos()) / t;
		if (t > 0 && t > inRay.tmin && t < inRay.tmax)
		{

			//inRay.setT(t, INFINITY);
			*tHit = t;
			*rayEpsilon = reCE * *tHit;
			return true;
		}
		else
		{
			return false;
		}
	}
}
Vector3D geoPlane::getNormal(const Vector3D& pos) const
{
	return this->n;
}

void geoPlane::getNormal(const DifferentialGeometry *queryPoint) const
{
	queryPoint->normal = getNormal(queryPoint->pos);

	if (normalMap != NULL && UV_Mapping != NULL)
	{
		UV_Mapping->getUVDir(queryPoint);
		ColorRGBA tmpNormal = normalMap->getColor(queryPoint) * 2 - ColorRGBA(1, 1, 1, 1);
		//tmpNormal.printInfo();
		queryPoint->normal = Normalize(
			- queryPoint->uDir * tmpNormal.r
			- queryPoint->vDir * tmpNormal.g
			+ queryPoint->normal * tmpNormal.b);
	}
	else
	{
		queryPoint->uDir = X_AXIS3D;// Vector3D(2 * PI * pp.z, 0, 2 * PI * pp.x).getNorm()
		queryPoint->vDir = Z_AXIS3D;
		
	}
}

bool geoPlane::isInside(const Vector3D& pPos) const
{
	if ((pPos - c) * n == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void geoPlane::bounding()
{
	ObjBound = BBox(Point3D(-INFINITY, -INFINITY, -INFINITY), Point3D(INFINITY, INFINITY, INFINITY));
}

/************************************************************************/
/* Torus Fuction Definition                                             */
/************************************************************************/
geoTorus::geoTorus(const Vector3D& pos, const Float& radius, const Float& secRadius)
{
	c = pos;
	r = radius;
	sr = secRadius;
}
geoTorus::~geoTorus()
{

}
void geoTorus::setCenter(const Vector3D& pos)
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
bool geoTorus::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const
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
		// 			return NULL;
		// 		}
		// 		else
		// 		{
		// 			//cout << t1 << endl;
		// 			return t1;
		// 		}
		return true;
	}
}
bool geoTorus::isInside(const Vector3D& pPos) const
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
/************************************************************************/
/* Ellipsoid Function Definition                                        */
/************************************************************************/
geoEllipsoid::geoEllipsoid()
{
	sa = 1;
	sb = 1;
	sc = 1;
}
geoEllipsoid::geoEllipsoid(const Vector3D& pos, const Float& semiA, const Float& semiB, const Float& semiC)
{
	c = pos;
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
geoEllipsoid::~geoEllipsoid()
{

}
void geoEllipsoid::setCenter(const Vector3D& pos)
{
	c = pos;
}
void geoEllipsoid::setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
bool geoEllipsoid::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const
{
	Vector3D rp = inRay.getPos(), rd = inRay.getDir();// Ray postion and ray direction.
	Float coeA = (rd.x / sa) * (rd.x / sa) + (rd.y / sb) * (rd.y / sb) + (rd.z / sc) * (rd.z / sc);
	Float coeB = (c.x - rp.x) * rd.x / (sa * sa) + (c.y - rp.y) * rd.y / (sb * sb) + (c.z - rp.z) * rd.z / (sc * sc);
	Float coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) + (rp.y - c.y) * (rp.y - c.y) / (sb * sb) + (rp.z - c.z) * (rp.z - c.z) / (sc * sc) - 1;
	Float delta = coeB * coeB - coeA * coeC;
	if (delta < 0)
	{
		//cout << "No DifferentialGeometry!" << endl;
		return false;
	}
	else//delta > 0
	{
		Float t1 = coeB - sqrt(delta);
		Float t2 = 2 * coeB - t1;

		inRay.setT(t1 / coeA, t2 / coeA);
		if (t1 <= 0 || t2 <= 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return 0;
}
Vector3D geoEllipsoid::getNormal(const Vector3D& pos) const
{
	// 2(x-c.x) / sa^2, 2(y - c.y) / sb^2, 2(z - c.z) / sc^2
	return Normalize(Vector3D(
		2 * (pos.x - c.x) / (sa * sa),
		2 * (pos.y - c.y) / (sb * sb),
		2 * (pos.z - c.z) / (sc * sc)));
}
bool geoEllipsoid::isInside(const Vector3D& pPos) const
{
	return 0;
}
/************************************************************************/
/* Paraboloid Function Definition                                       */
/************************************************************************/
geoParaboloid::geoParaboloid()
{
	sa = 1;
	sb = 1;
	sc = 1;
	pbType = ELLIPTIC_PARABOLOID;
}
geoParaboloid::geoParaboloid(const Vector3D& pos, const Float& semiA, const Float& semiB, const Float& semiC, PARABOLOID_TYPE newType)
{
	c = pos;
	sa = semiA;
	sb = semiB;
	sc = semiC;
	pbType = newType;
}
geoParaboloid::~geoParaboloid()
{
}
void geoParaboloid::setCenter(const Vector3D& pos)
{
	c = pos;
}
void geoParaboloid::setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
void geoParaboloid::setParaboloidType(PARABOLOID_TYPE newType)
{
	pbType = newType;
}
bool geoParaboloid::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const
{
	Vector3D rp = inRay.getPos(), rd = inRay.getDir();// Ray postion and ray direction.	
	Float coeA = (rd.x / sa) * (rd.x / sa) + pbType * (rd.z / sc) * (rd.z / sc);
	Float coeB = (c.x - rp.x) * rd.x / (sa * sa) + pbType * (c.z - rp.z) * rd.z / (sc * sc) + rd.y * 0.5 / sb;
	Float coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) + pbType * (rp.z - c.z) * (rp.z - c.z) / (sc * sc) + (c.y - rp.y) / sb;
	Float delta = coeB * coeB - coeA * coeC;
	if (coeA == 0)
	{
		inRay.setT(coeC * 0.5 / coeB, coeC * 0.5 / coeB);
		return true;
	}
	else
	{
		if (delta < 0)
		{

			inRay.setT(NAN, NAN);
			//cout << "No DifferentialGeometry!" << endl;
			return false;
		}
		else//delta > 0
		{
			Float t1 = (coeB - sqrt(delta)) / coeA;
			Float t2 = 2 * coeB / coeA - t1;
			// 		t1 /= coeA;
			// 		t2 /= coeA;
			inRay.setT(t1, t2);

			if (t1 > 0 && t2 > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
Vector3D geoParaboloid::getNormal(const Vector3D& pos) const
{
	// 2(x-c.x) / sa^2, -1/sb, 2* type *(z - c.z) / sb^2
	return Normalize(Vector3D(
		2 * (pos.x - c.x) / (sa * sa),
		-1 / sb,
		pbType * 2 * (pos.z - c.z) / (sc * sc)));
}
bool geoParaboloid::isInside(const Vector3D& pPos) const
{
	return 0;
}
/************************************************************************/
/* Hyperboloid Function Definition                                      */
/************************************************************************/
geoHyperboloid::geoHyperboloid()
{
	sa = 1;
	sb = 1;
	sc = 1;
	hbType = ONE_SHEET;
}
geoHyperboloid::geoHyperboloid(const Vector3D& pos, const Float& semiA, const Float& semiB, const Float& semiC, HYPERBOLOID_TYPE newType)
{
	c = pos;
	sa = semiA;
	sb = semiB;
	sc = semiC;
	hbType = newType;
}
geoHyperboloid::~geoHyperboloid()
{
}
void geoHyperboloid::setCenter(const Vector3D& pos)
{
	c = pos;
}
void geoHyperboloid::setSemiAxes(const Float& semiA, const Float& semiB, const Float& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
void geoHyperboloid::setHyperboloidType(HYPERBOLOID_TYPE newType)
{
	hbType = newType;
}
bool geoHyperboloid::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const
{
	Vector3D rp = inRay.getPos(), rd = inRay.getDir();// Ray postion and ray direction.	
	Float coeA = (rd.x / sa) * (rd.x / sa) - (rd.y / sb) * (rd.y / sb) + (rd.z / sc) * (rd.z / sc);
	Float coeB = (c.x - rp.x) * rd.x / (sa * sa) - (c.y - rp.y) * rd.y / (sb * sb) + (c.z - rp.z) * rd.z / (sc * sc);
	Float coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) - (rp.y - c.y) * (rp.y - c.y) / (sb * sb) + (rp.z - c.z) * (rp.z - c.z) / (sc * sc) - hbType;

	Float delta = coeB * coeB - coeA * coeC;
	if (delta < 0)
	{
		inRay.setT(NAN, NAN);
		//cout << "No DifferentialGeometry!" << endl;
		return false;
	}
	else//delta > 0
	{
		Float t1 = (coeB - sqrt(delta)) / coeA;
		Float t2 = 2 * coeB / coeA - t1;
		// 		t1 /= coeA;
		// 		t2 /= coeA;
		inRay.setT(t1, t2);
		if (t1 > 0 && t2 > 0)
		{
			*tHit = t1;
			return true;
		}
		else
		{
			return false;
		}
	}
}
Vector3D geoHyperboloid::getNormal(const Vector3D& pos) const
{
	// 2(x-c.x) / sa^2, -1/sb, 2* type *(z - c.z) / sb^2
	return Normalize(Vector3D(
		2 * (pos.x - c.x) / sqr(sa),
		-2 * (pos.y - c.y) / sqr(sb),
		2 * (pos.z - c.z) / sqr(sc)));
}
bool geoHyperboloid::isInside(const Vector3D& pPos) const
{
	return 0;
}