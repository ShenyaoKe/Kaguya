#include "Geometry/Shape.h"
#include "Tracer/Ray.h"
#include "Shading/TextureMapping.h"
#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"
/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
Shape::Shape()
{
}
Shape::~Shape()
{
	material = NULL;
	UV_Mapping = NULL;
	normalMap = NULL;
}
void Shape::bounding()
{
	//return;
}
const BBox& Shape::getBounding() const
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
bool Shape::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, double *tHit, double *rayEpsilon) const
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

Vector3D Shape::getCenter() const
{
	return c;
}

/************************************************************************/
/* Sphere Function Definition                                           */
/************************************************************************/
geoSphere::geoSphere()
{
	r = 1;
	bounding();
}
geoSphere::geoSphere(const Vector3D& pos, const double& radius)
{
	c = pos;
	r = radius;
	bounding();
}
geoSphere::~geoSphere()
{

}
void geoSphere::bounding()
{
	ObjBound = BBox(c);
	ObjBound.expand(r);
	//return false;
}
void geoSphere::setCenter(const Vector3D& pos)
{
	c = pos;
	bounding();
}
void geoSphere::setRadius(double radius)
{
	r = radius;
	bounding();
}
bool geoSphere::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, double *tHit, double *rayEpsilon) const
{
	double coeB = inRay.getDir() * (c - inRay.getPos());
	double coeC = (inRay.getPos() - c).getLenSq() - sqr(r);
	double delta = sqr(coeB) - coeC;
	if (delta > 0)//delta > 0
	{
		double t1 = coeB - sqrt(delta);
		double t2 = 2 * coeB - t1;
		//inRay.setT(t1, t2);
		if (t1 <= 0 && t2 <= 0)
		{
			return false;
		}
		else if (t1 > 0)
		{
			*tHit = t1;
		}
		else
		{
			*tHit = t2;
		}
		if (*tHit > inRay.tmin && *tHit < inRay.tmax)
		{
			*rayEpsilon = reCE * *tHit;
			return true;
		}
	}
	return false;
}
Vector3D geoSphere::getNormal(const Vector3D& pos) const
{
	return Normalize(pos - c);
}

void geoSphere::getNormal(const DifferentialGeometry *queryPoint) const
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
		Point3D pp = queryPoint->pos - c;
		double rsintheta = r * sqrt(1 - sqr(pp.y / r));
		if (rsintheta == 0.0)
		{
			queryPoint->uDir = X_AXIS3D;
			queryPoint->vDir = Z_AXIS3D;
		}
		else
		{
			queryPoint->uDir = Normalize(Vector3D(-pp.z, 0, pp.x));// Vector3D(2 * PI * pp.z, 0, 2 * PI * pp.x).getNorm()
			queryPoint->vDir = Normalize(-Vector3D(pp.x * pp.y / rsintheta, -rsintheta, pp.y * pp.z / rsintheta));
		}
	}
}

bool geoSphere::isInside(const Vector3D& pPos) const
{
	if ((pPos - c).getLenSq() <= r * r)
	{
		return true;
	}
	else
	{
		return false;
	}
}
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
bool geoPlane::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, double *tHit, double *rayEpsilon) const
{
	double t = this->n * inRay.getDir();
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
geoTorus::geoTorus(const Vector3D& pos, const double& radius, const double& secRadius)
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
void geoTorus::setRadius(double radius)
{
	r = radius;
}
void geoTorus::setSecRadius(double secRadius)
{
	sr = secRadius;
}
bool geoTorus::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, double *tHit, double *rayEpsilon) const
{
	//double coeA = inRay.getDir() * inRay.getDir();//len == 1
	double coeB = inRay.getDir() * (inRay.getPos() - c);
	double coeC = (inRay.getPos() - c).getLenSq() - r * r;
	//double delta = coeB * coeB - 4 * coeA * coeB;
	double delta = coeB * coeB - coeC;
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
		double t1 = coeB - sqrt(delta);
		//double t2 = 2 * coeB - t1;
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
	double tmp = (pPos - c).getLenSq() - (r * r + sr * sr);
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
geoEllipsoid::geoEllipsoid(const Vector3D& pos, const double& semiA, const double& semiB, const double& semiC)
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
void geoEllipsoid::setSemiAxes(const double& semiA, const double& semiB, const double& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
bool geoEllipsoid::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, double *tHit, double *rayEpsilon) const
{
	Vector3D rp = inRay.getPos(), rd = inRay.getDir();// Ray postion and ray direction.
	double coeA = (rd.x / sa) * (rd.x / sa) + (rd.y / sb) * (rd.y / sb) + (rd.z / sc) * (rd.z / sc);
	double coeB = (c.x - rp.x) * rd.x / (sa * sa) + (c.y - rp.y) * rd.y / (sb * sb) + (c.z - rp.z) * rd.z / (sc * sc);
	double coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) + (rp.y - c.y) * (rp.y - c.y) / (sb * sb) + (rp.z - c.z) * (rp.z - c.z) / (sc * sc) - 1;
	double delta = coeB * coeB - coeA * coeC;
	if (delta < 0)
	{
		//cout << "No DifferentialGeometry!" << endl;
		return false;
	}
	else//delta > 0
	{
		double t1 = coeB - sqrt(delta);
		double t2 = 2 * coeB - t1;

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
geoParaboloid::geoParaboloid(const Vector3D& pos, const double& semiA, const double& semiB, const double& semiC, PARABOLOID_TYPE newType)
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
void geoParaboloid::setSemiAxes(const double& semiA, const double& semiB, const double& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
void geoParaboloid::setParaboloidType(PARABOLOID_TYPE newType)
{
	pbType = newType;
}
bool geoParaboloid::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, double *tHit, double *rayEpsilon) const
{
	Vector3D rp = inRay.getPos(), rd = inRay.getDir();// Ray postion and ray direction.	
	double coeA = (rd.x / sa) * (rd.x / sa) + pbType * (rd.z / sc) * (rd.z / sc);
	double coeB = (c.x - rp.x) * rd.x / (sa * sa) + pbType * (c.z - rp.z) * rd.z / (sc * sc) + rd.y * 0.5 / sb;
	double coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) + pbType * (rp.z - c.z) * (rp.z - c.z) / (sc * sc) + (c.y - rp.y) / sb;
	double delta = coeB * coeB - coeA * coeC;
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
			double t1 = (coeB - sqrt(delta)) / coeA;
			double t2 = 2 * coeB / coeA - t1;
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
geoHyperboloid::geoHyperboloid(const Vector3D& pos, const double& semiA, const double& semiB, const double& semiC, HYPERBOLOID_TYPE newType)
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
void geoHyperboloid::setSemiAxes(const double& semiA, const double& semiB, const double& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
void geoHyperboloid::setHyperboloidType(HYPERBOLOID_TYPE newType)
{
	hbType = newType;
}
bool geoHyperboloid::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, double *tHit, double *rayEpsilon) const
{
	Vector3D rp = inRay.getPos(), rd = inRay.getDir();// Ray postion and ray direction.	
	double coeA = (rd.x / sa) * (rd.x / sa) - (rd.y / sb) * (rd.y / sb) + (rd.z / sc) * (rd.z / sc);
	double coeB = (c.x - rp.x) * rd.x / (sa * sa) - (c.y - rp.y) * rd.y / (sb * sb) + (c.z - rp.z) * rd.z / (sc * sc);
	double coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) - (rp.y - c.y) * (rp.y - c.y) / (sb * sb) + (rp.z - c.z) * (rp.z - c.z) / (sc * sc) - hbType;

	double delta = coeB * coeB - coeA * coeC;
	if (delta < 0)
	{
		inRay.setT(NAN, NAN);
		//cout << "No DifferentialGeometry!" << endl;
		return false;
	}
	else//delta > 0
	{
		double t1 = (coeB - sqrt(delta)) / coeA;
		double t2 = 2 * coeB / coeA - t1;
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