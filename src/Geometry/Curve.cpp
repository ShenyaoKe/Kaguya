#include "Curve.h"


Curve::Curve()
{
}


Curve::~Curve()
{
}
//////////////////////////////////////////////////////////////////////////
template<typename T>
void HermitSpline<T>::calculateT()
{
	*t[3] = 2 * *p[0] - 2 * *p[1] + *v[0] + *v[1];
	*t[2] = -3 * *p[0] + 3 * *p[1] - 2 * *v[0] - *v[1];
	*t[1] = *v[0];
	*t[0] = *p[0];
}
template<typename T>
HermitSpline<T>::HermitSpline()
{
	T* tmpPnt = new T[8];
	p[0] = tmpPnt++;
	p[1] = tmpPnt++;
	v[0] = tmpPnt++;
	v[1] = tmpPnt++;
	for (int i = 0; i < 4; i++)
	{
		t[i] = tmpPnt++;
	}
	
}
template<typename T>
HermitSpline<T>::HermitSpline(const T &p0, const T &p1, const T &v0, const T &v1)
{
	T* tmpPnt = new T[8];
	p[0] = tmpPnt++;
	p[1] = tmpPnt++;
	v[0] = tmpPnt++;
	v[1] = tmpPnt++;
	for (int i = 0; i < 4; i++)
	{
		t[i] = tmpPnt++;
	}

	*p[0] = p0; *p[1] = p1;
	*v[0] = v0; *v[1] = v1;

	calculateT();
}

template<typename T>
HermitSpline<T>::HermitSpline(const T *points, const T *vs)
{
	T* tmpPnt = new T[8];
	p[0] = tmpPnt++;
	p[1] = tmpPnt++;
	v[0] = tmpPnt++;
	v[1] = tmpPnt++;
	for (int i = 0; i < 4; i++)
	{
		t[i] = tmpPnt++;
	}

	*p[0] = points[0];
	*p[1] = points[1];
	*v[0] = vs[0];
	*v[1] = vs[1];

	calculateT();
}
template<typename T>
HermitSpline<T>::HermitSpline(T *points, T *vs)
{
	p[0] = points;
	p[1] = points + 1;
	v[0] = vs;
	v[1] = vs + 1;
}
template<typename T>
T& HermitSpline<T>::getPos(Float Val) const
{
	T ret = *t[0];
	Float t_value = Val;
	for (int i = 1; i < 4; i++)
	{
		ret += t_value * *t[i];
		t_value *= Val;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
template<typename T>
void CatmullRomSpline<T>::calculateT()
{
	*t[3] = - 0.5 * *p[0] + 1.5 * *p[1] - 1.5 * *p[2] + 0.5 * *p[3];
	*t[2] = *p[0] - 2.5 * *p[1] + 2 * *p[2] - 0.5 * *p[3];
	*t[1] = - 0.5 * *p[0] + 0.5 * *p[2];
	*t[0] = *p[1];
}

template<typename T>
CatmullRomSpline<T>::CatmullRomSpline()
{
	T* tmpPnt = new T[8];
	
	for (int i = 0; i < 4; i++)
	{
		p[i] = tmpPnt++;
	}
	for (int i = 0; i < 4; i++)
	{
		t[i] = tmpPnt++;
	}
	calculateT();
}

template<typename T>
CatmullRomSpline<T>::CatmullRomSpline(const T &p0, const T &p1, const T &p2, const T &p3)
{
	T* tmpPnt = new T[8];

	for (int i = 0; i < 4; i++)
	{
		p[i] = tmpPnt++;
	}
	for (int i = 0; i < 4; i++)
	{
		t[i] = tmpPnt++;
	}
	*p[0] = p0; *p[1] = p1;
	*p[2] = p2; *p[3] = p3;

	calculateT();
}

template<typename T>
CatmullRomSpline<T>::CatmullRomSpline(const T *points)
{
	for (int i = 0; i < 4; i++)
	{
		*p[i] = points[i];
	}
	calculateT();
}

template<typename T>
CatmullRomSpline<T>::CatmullRomSpline(T *points)
{
	for (int i = 0; i < 4; i++)
	{
		p[i] = points + i;
	}
	calculateT();
}


template<typename T>
T& CatmullRomSpline<T>::getPos(Float Val) const
{
	T ret = *t[0];
	Float t_value = Val;
	for (int i = 1; i < 4; i++)
	{
		ret += t_value * *t[i];
		t_value *= Val;
	}
	return ret;
}
