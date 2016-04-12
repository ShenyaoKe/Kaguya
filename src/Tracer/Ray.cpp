#include "Tracer/Ray.h"

Ray::Ray() : time(0), dp(0)
{
	d.x = 1;
	d.y = 0;
	d.z = 0;
}

Ray::~Ray()
{
}

Ray::Ray(const Vector3D &pos, const Vector3D &dir,
	double minT, double maxT)
	: o(pos)
	, time(0), dp(0)
	, tmin(minT), tmax(maxT)
{
	//pos = p;
	d = Normalize(dir);
}
Point3D Ray::operator()(const Float& t) const
{
	return o + t * d;
}
void Ray::setPos(Vector3D &vp)
{
	o = vp;
}

void Ray::setDir(Vector3D &vd)
{
	d = vd;
}
void Ray::setT(const Float& t1, const Float& t2) const
{
	tmin = t1;
	tmax = t2;
}
Point3D Ray::getPos() const
{
	return o;
}

Vector3D Ray::getDir() const
{
	return d;
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
	d.normalize();
	//return dir;
}

void Ray::printInfo() const
{
	cout << "Origin point position: ";
	o.printInfo();
	cout << "Ray direction: ";
	d.printInfo();
}