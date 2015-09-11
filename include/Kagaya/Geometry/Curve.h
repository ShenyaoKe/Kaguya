#ifndef __CURVE__
#define __CURVE__


#pragma once
#include "Math/CGVector.h"
class Curve
{
public:
	Curve();
	virtual ~Curve() = 0;
};

template<typename T>
class HermitSpline : public Curve
{
public:
	HermitSpline();
	HermitSpline(const T &p0, const T &p1, const T &v0, const T &v1);
	HermitSpline(const T *points, const T *vs);
	HermitSpline(T *points, T *vs);

	T& getPos(double tVal) const;
protected:
private:
	T *p[2], *v[2], *t[4];
	void calculateT();
};

template<typename T>
class CatmullRomSpline : public Curve
{
public:
	CatmullRomSpline();
	CatmullRomSpline(const T &p0, const T &p1, const T &p2, const T &p3);
	CatmullRomSpline(const T *points);
	CatmullRomSpline(T *points);

	T& getPos(double Val) const;
protected:
private:
	T *p[4], *t[4];
	void calculateT();
};

template class HermitSpline < double > ;
template class HermitSpline < Point2D > ;
template class HermitSpline < Point3D > ;
template class CatmullRomSpline < double > ;
template class CatmullRomSpline < Point2D > ;
template class CatmullRomSpline < Point3D > ;
#endif // __CURVE__