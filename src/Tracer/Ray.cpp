#include "Tracer/Ray.h"

Ray::Ray() : time(0), dp(0)
{
	dir.x = 1;
	dir.y = 0;
	dir.z = 0;
}

Ray::~Ray()
{
}

Ray::Ray(const Point3D& p, const Vector3D& d,
	double minT, double maxT)
	: pos(p)
	, time(0), dp(0)
	, tmin(minT), tmax(maxT)
{
	//pos = p;
	dir = Normalize(d);
}
Point3D Ray::operator()(const Float& t) const
{
	return pos + t * dir;
}
void Ray::setPos(Point3D& vp)
{
	pos = vp;
}

void Ray::setDir(Vector3D& vd)
{
	dir = vd;
}
void Ray::setT(const Float& t1, const Float& t2) const
{
	tmin = t1;
	tmax = t2;
}
Point3D Ray::getPos() const
{
	return pos;
}

Vector3D Ray::getDir() const
{
	return dir;
}
Float Ray::getDifferenceT() const
{
	return tmax - tmin;
}
// Vector3D Ray::getPoint(Float t) const
// {
// 	return pos + dir * t;
// }
// Float Ray::getDist(Float t) const
// {
// 	return (dir * t).getLength();
// }
void Ray::normalize()
{
	dir.normalize();
	//return dir;
}

void Ray::printInfo() const
{
	cout << "Origin point position: ";
	pos.printInfo();
	cout << "Ray direction: ";
	dir.printInfo();
}