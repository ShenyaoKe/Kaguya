#include "simpleGeometry.h"

/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
inline void Shape::assignTextureMapping(TextureMapping* &mapping)
{
	mapping->setPos(c);
	UV_Mapping = mapping;
}
Vector3D Shape::getNormal(const Vector3D& pos) const
{
	return Vector3D(0, 0, -1);
}
inline void Shape::getNormal(const Intersection& queryPoint) const
{
	queryPoint.normal = getNormal(queryPoint.pos);

	if (normalMap != NULL && UV_Mapping != NULL)
	{
		UV_Mapping->getUVDir(queryPoint);
		//queryPoint.uDir.printInfo();
		//queryPoint.UV.printInfo();
		ColorRGBA tmpNormal = normalMap->getColor(queryPoint) * 2 - ColorRGBA(1, 1, 1, 1);
		//tmpNormal.printInfo();
		queryPoint.normal = (-queryPoint.uDir * tmpNormal.r - queryPoint.vDir * tmpNormal.g + queryPoint.normal * tmpNormal.b).getNorm();
		//(queryPoint.normal - tn).printInfo();
		//queryPoint.normal.printInfo();
	}
}
inline void Shape::printInfo() const
{
	cout << "Center:\t";
	c.printInfo();
}
inline void Shape::getUV(const Intersection& queryPoint) const
{
	if (UV_Mapping != NULL)
	{
		UV_Mapping->mapToUV(queryPoint);

	}
	// 	else
	// 	{
	// 		cout << "No UV mapping defined!" << endl;
	// 	}
}
inline ColorRGBA Shape::getAmbient(const Intersection& queryPoint) const
{
	return material->getAmbient(queryPoint);
}
inline ColorRGBA Shape::getDiffuse(const Intersection& querryPoint, const Light* light) const
{
	return material->getDiffuseAt(querryPoint, light);
}
inline ColorRGBA Shape::getColor(const Intersection& querryPoint, const Light* light) const
{
	return material->getColor(querryPoint, light);
}
/************************************************************************/
/* Sphere Function Definition                                           */
/************************************************************************/
inline geoSphere::geoSphere(const Vector3D& pos, const float& radius)
{
	c = pos;
	r = radius;
}
// inline geoSphere::~geoSphere()
// {
// 
// }
inline void geoSphere::setCenter(const Vector3D& pos)
{
	c = pos;
}
inline void geoSphere::setRadius(float radius)
{
	r = radius;
}
inline bool geoSphere::getIntersection(const Ray& inRay) const
{
	float coeB = inRay.getDir() * (c - inRay.getPos());
	float coeC = (inRay.getPos() - c).getLenSq() - sqr(r);
	float delta = sqr(coeB) - coeC;
	if (delta < 0)
	{
		//cout << "No intersection!" << endl;
		return false;
	}
	else//delta > 0
	{
		float t1 = coeB - sqrt(delta);
		float t2 = 2 * coeB - t1;
		inRay.setT(t1, t2);
		if (t1 <= 0 || t2 <= 0)
		{
			return false;
		}
		else if (t1 < t2)
		{
			return true;
		}
		else
		{
			return true;
		}
	}
}
inline Vector3D geoSphere::getNormal(const Vector3D& pos) const
{
	return (pos - c).getNorm();
}
inline bool geoSphere::isInside(const Vector3D& pPos) const
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
inline geoPlane::geoPlane(const Vector3D& pos, const Vector3D& norm)
{
	c = pos;
	n = norm;
}
inline geoPlane::~geoPlane()
{

}
inline void geoPlane::setOrigin(const Vector3D& pos)
{
	c = pos;
}
inline void geoPlane::setNorm(const Vector3D& norm)
{
	n = norm;
}
inline bool geoPlane::getIntersection(const Ray& inRay) const
{
	float tmp = this->n * inRay.getDir();
	if (tmp >= 0)
	{
		//cout << "Ray parallels to the plane or in the plane." << endl;
		return false;
	}
	else
	{
		float t = n * (this->c - inRay.getPos()) / tmp;
		if (t > 0)
		{

			inRay.setT(t, NUM_INFINITY);
			return true;
		}
		else
		{
			return false;
		}
	}
}
inline Vector3D geoPlane::getNormal(const Vector3D& pos) const
{
	return this->n;
}
inline bool geoPlane::isInside(const Vector3D& pPos) const
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
/************************************************************************/
/* Torus Fuction Definition                                             */
/************************************************************************/
inline geoTorus::geoTorus(const Vector3D& pos, const float& radius, const float& secRadius)
{
	c = pos;
	r = radius;
	sr = secRadius;
}
inline geoTorus::~geoTorus()
{

}
inline void geoTorus::setCenter(const Vector3D& pos)
{
	c = pos;
}
inline void geoTorus::setRadius(float radius)
{
	r = radius;
}
inline void geoTorus::setSecRadius(float secRadius)
{
	sr = secRadius;
}
inline bool geoTorus::getIntersection(const Ray& inRay) const
{
	//float coeA = inRay.getDir() * inRay.getDir();//len == 1
	float coeB = inRay.getDir() * (inRay.getPos() - c);
	float coeC = (inRay.getPos() - c).getLenSq() - r * r;
	//float delta = coeB * coeB - 4 * coeA * coeB;
	float delta = coeB * coeB - coeC;
	if (delta < 0)
	{
		//cout << "No intersection!" << endl;
		return false;
	}
	else if (delta == 0)
	{
		//cout << "haha" << endl;
		return true;
	}
	else//delta > 0
	{
		float t1 = coeB - sqrt(delta);
		//float t2 = 2 * coeB - t1;
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
inline bool geoTorus::isInside(const Vector3D& pPos) const
{
	float tmp = (pPos - c).getLenSq() - (r * r + sr * sr);
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
inline geoEllipsoid::geoEllipsoid(const Vector3D& pos, const float& semiA, const float& semiB, const float& semiC)
{
	c = pos;
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
inline geoEllipsoid::~geoEllipsoid()
{

}
inline void geoEllipsoid::setCenter(const Vector3D& pos)
{
	c = pos;
}
inline void geoEllipsoid::setSemiAxes(const float& semiA, const float& semiB, const float& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
inline bool geoEllipsoid::getIntersection(const Ray& inRay) const
{
	Vector3D rp = inRay.getPos(), rd = inRay.getDir();// Ray postion and ray direction.
	float coeA = (rd.x / sa) * (rd.x / sa) + (rd.y / sb) * (rd.y / sb) + (rd.z / sc) * (rd.z / sc);
	float coeB = (c.x - rp.x) * rd.x / (sa * sa) + (c.y - rp.y) * rd.y / (sb * sb) + (c.z - rp.z) * rd.z / (sc * sc);
	float coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) + (rp.y - c.y) * (rp.y - c.y) / (sb * sb) + (rp.z - c.z) * (rp.z - c.z) / (sc * sc) - 1;
	float delta = coeB * coeB - coeA * coeC;
	if (delta < 0)
	{
		//cout << "No intersection!" << endl;
		return false;
	}
	else//delta > 0
	{
		float t1 = coeB - sqrt(delta);
		float t2 = 2 * coeB - t1;

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
inline Vector3D geoEllipsoid::getNormal(const Vector3D& pos) const
{
	// 2(x-c.x) / sa^2, 2(y - c.y) / sb^2, 2(z - c.z) / sc^2
	return Vector3D(2 * (pos.x - c.x) / (sa * sa), 2 * (pos.y - c.y) / (sb * sb), 2 * (pos.z - c.z) / (sc * sc)).getNorm();
}
inline bool geoEllipsoid::isInside(const Vector3D& pPos) const
{
	return 0;
}
/************************************************************************/
/* Paraboloid Function Definition                                       */
/************************************************************************/
inline geoParaboloid::geoParaboloid(const Vector3D& pos, const float& semiA, const float& semiB, const float& semiC, PARABOLOID_TYPE newType)
{
	c = pos;
	sa = semiA;
	sb = semiB;
	sc = semiC;
	pbType = newType;
}
inline geoParaboloid::~geoParaboloid()
{
}
inline void geoParaboloid::setCenter(const Vector3D& pos)
{
	c = pos;
}
inline void geoParaboloid::setSemiAxes(const float& semiA, const float& semiB, const float& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
inline void geoParaboloid::setParaboloidType(PARABOLOID_TYPE newType)
{
	pbType = newType;
}
inline bool geoParaboloid::getIntersection(const Ray& inRay) const
{
	Vector3D rp = inRay.getPos(), rd = inRay.getDir();// Ray postion and ray direction.	
	float coeA = (rd.x / sa) * (rd.x / sa) + pbType * (rd.z / sc) * (rd.z / sc);
	float coeB = (c.x - rp.x) * rd.x / (sa * sa) + pbType * (c.z - rp.z) * rd.z / (sc * sc) + rd.y * 0.5 / sb;
	float coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) + pbType * (rp.z - c.z) * (rp.z - c.z) / (sc * sc) + (c.y - rp.y) / sb;
	float delta = coeB * coeB - coeA * coeC;
	if (coeA == 0)
	{
		inRay.setT(coeC * 0.5 / coeB, coeC * 0.5 / coeB);
		return true;
	}
	else
	{
		if (delta < 0)
		{

			inRay.setT(NUM_NAN, NUM_NAN);
			//cout << "No intersection!" << endl;
			return false;
		}
		else//delta > 0
		{
			float t1 = (coeB - sqrt(delta)) / coeA;
			float t2 = 2 * coeB / coeA - t1;
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
inline Vector3D geoParaboloid::getNormal(const Vector3D& pos) const
{
	// 2(x-c.x) / sa^2, -1/sb, 2* type *(z - c.z) / sb^2
	return Vector3D(2 * (pos.x - c.x) / (sa * sa), -1 / sb, pbType * 2 * (pos.z - c.z) / (sc * sc)).getNorm();
}
inline bool geoParaboloid::isInside(const Vector3D& pPos) const
{
	return 0;
}
/************************************************************************/
/* Hyperboloid Function Definition                                      */
/************************************************************************/
inline geoHyperboloid::geoHyperboloid(const Vector3D& pos, const float& semiA, const float& semiB, const float& semiC, HYPERBOLOID_TYPE newType)
{
	c = pos;
	sa = semiA;
	sb = semiB;
	sc = semiC;
	hbType = newType;
}
inline geoHyperboloid::~geoHyperboloid()
{
}
inline void geoHyperboloid::setCenter(const Vector3D& pos)
{
	c = pos;
}
inline void geoHyperboloid::setSemiAxes(const float& semiA, const float& semiB, const float& semiC)
{
	sa = semiA;
	sb = semiB;
	sc = semiC;
}
inline void geoHyperboloid::setHyperboloidType(HYPERBOLOID_TYPE newType)
{
	hbType = newType;
}
inline bool geoHyperboloid::getIntersection(const Ray& inRay) const
{
	Vector3D rp = inRay.getPos(), rd = inRay.getDir();// Ray postion and ray direction.	
	float coeA = (rd.x / sa) * (rd.x / sa) - (rd.y / sb) * (rd.y / sb) + (rd.z / sc) * (rd.z / sc);
	float coeB = (c.x - rp.x) * rd.x / (sa * sa) - (c.y - rp.y) * rd.y / (sb * sb) + (c.z - rp.z) * rd.z / (sc * sc);
	float coeC = (rp.x - c.x) * (rp.x - c.x) / (sa * sa) - (rp.y - c.y) * (rp.y - c.y) / (sb * sb) + (rp.z - c.z) * (rp.z - c.z) / (sc * sc) - hbType;

	float delta = coeB * coeB - coeA * coeC;
	if (delta < 0)
	{
		inRay.setT(NUM_NAN, NUM_NAN);
		//cout << "No intersection!" << endl;
		return false;
	}
	else//delta > 0
	{
		float t1 = (coeB - sqrt(delta)) / coeA;
		float t2 = 2 * coeB / coeA - t1;
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
inline Vector3D geoHyperboloid::getNormal(const Vector3D& pos) const
{
	// 2(x-c.x) / sa^2, -1/sb, 2* type *(z - c.z) / sb^2
	return Vector3D(2 * (pos.x - c.x) / sqr(sa), -2 * (pos.y - c.y) / sqr(sb), 2 * (pos.z - c.z) / sqr(sc)).getNorm();
}
inline bool geoHyperboloid::isInside(const Vector3D& pPos) const
{
	return 0;
}