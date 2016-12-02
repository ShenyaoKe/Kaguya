#include "Tracer/Ray.h"

Ray::Ray() : time(0), dp(0)
{
	d.x = 1;
	d.y = 0;
	d.z = 0;
}

Ray::Ray(const Point3f &pos, const Vector3f &dir,
	Float minT, Float maxT)
	: o(pos)
	, time(0), dp(0)
	, tmin(minT), tmax(maxT)
{
	//pos = p;
	d = Normalize(dir);
}

Point3f Ray::operator()(Float t) const
{
	return o + d * t;
}

void Ray::setT(Float t1, Float t2) const
{
	tmin = t1;
	tmax = t2;
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

void Ray::printInfo() const
{
	cout << "Origin point position: " << o << endl;
	cout << "Ray direction: " << d << endl;
}