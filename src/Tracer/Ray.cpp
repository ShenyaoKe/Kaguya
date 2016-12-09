#include "Tracer/Ray.h"

Ray::Ray() : d(1, 0, 0), time(0), dp(0)
{
}

Ray::Ray(const Point3f &pos, const Vector3f &dir,
	     Float minT, Float maxT)
	: o(pos), d(normalize(dir))
	, time(0), dp(0)
	, tmin(minT), tmax(maxT)
{
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

void Ray::printInfo() const
{
	cout << "Origin point position: " << o << endl;
	cout << "Ray direction: " << d << endl;
}