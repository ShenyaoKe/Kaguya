#pragma once
#include <cmath>
#include <algorithm>

/************************************************************************/
/* Definition                                                           */
/************************************************************************/
static const double M_PI = 3.14159265358979323846;
static const double M_TWOPI = M_PI * 2.0;
static const double INV_PI = 1.0 / M_PI;
static const double INV_TWOPI = INV_PI / 2.0;
static const double INV_FOURPI = INV_PI / 4.0;
static const double NUM_ZERO = 0;
static const double NUM_INFINITY = INFINITY;// std::numeric_limits<double>::infinity();
static const double NUM_NAN = NAN;// std::numeric_limits<double>::quiet_NaN();
/************************************************************************/
/* Functions                                                            */
/************************************************************************/
/*
template <typename T>
inline const T& min(const T& d1, const T& d2)
{
	return (d1 < d2 ? d1 : d2);
}
//template min<int>(int, int);
template <typename T>
inline const T& max(const T& d1, const T& d2)
{
	return (d1 > d2 ? d1 : d2);
}
*/
template <typename T>
inline const T& clamp(const T& data, const T& minValue, const T& maxValue)
{
	return data > maxValue ? maxValue : (data < minValue ? minValue : data);
}
template <typename T2>
inline const T2& clampFromZeroToOne(const T2& data)
{
	return clamp(data, static_cast<T2>(0), static_cast<T2>(1));
}
template <typename T> inline bool inRange(const T& data, const T& minv, const T& maxv)
{
	if (data >= minv && data <= maxv)
	{
		return true;
	}
	else
	{
		return false;
	}
}
inline bool inUnitRange(double data)
{
	double epsilon = 5e-4f;
	return inRange(data, 0. - epsilon, 1. + epsilon);
}
inline bool inUnitRange(double data, double epsilon)
{
	//double epsilon = 5e-4f;
	return inRange(data, 0. - epsilon, 1. + epsilon);
}
inline double RadianToDegree(double radian)
{
	return radian * 180.0 * INV_PI;
}
inline double DegreeToRadian(double degree)
{
	return degree * M_PI / 180.0;
}
inline int Ceil2Int(double val)
{
	return static_cast<int>(ceil(val));
}
inline int Floor2Int(double val)
{
	return static_cast<int>(floor(val));
}
inline int Round2Int(double val)
{
	return Floor2Int(val + 0.5);
}
inline double Log2Int(double val)
{
	//static double invLog2 = 1.0 / log(2.0);
	return Floor2Int(log2(val));
}
inline int findMinPositiveT(double* &ts, int size)
{
	double tmp = INFINITY;
	int interID = -1;
	for (int i = 0; i < size; i++)
	{
		if (ts[i] <= tmp && ts[i] > 0)
		{
			tmp = ts[i];
			interID = i;
		}
	}
	return interID;
}

inline int Random(int range)
{
	return rand() % range;
}
inline double unitRandom(int segment)
{
	return Random(segment) / static_cast<double>(segment);
}
template <typename T>
inline T sqr(const T& data)
{
	return data * data;
}
template <typename T>
inline T cubic(const T& data)
{
	return data * data * data;
}
/************************************************************************/
/* Interpolation Methods                                                */
/************************************************************************/
template <typename T>
inline T lerp(const T& data1, const T& data2, const double& t)//Linear Interpolation
{
	return data1 * (1 - t) + data2 * t;
}
template <typename T>
inline T cosInterpolate(const T& data1, const T& data2, const double& t)//cosInterpolate
{
	double nt = (1 - cos(t * M_PI)) * 0.5;
	return data1 * (1 - nt) + data2 * nt;
}
template <typename T>
inline T cubicInterpolate(const T& data0, const T& data1, const T& data2, const T& data3, const double& t)
{
	T P = (data3 - data2) - (data0 - data1);
	T Q = (data0 - data1) - P;
	T R = data2 - data0;
	T S = data1;
	return P * cubic(t) + Q * sqr(t) + R * t + S;
}
inline double curve_t(const double& t)//Mainly used for perlin noise
{
	return 6 * cubic(t) * sqr(t) - 15 * sqr(sqr(t)) + 10 * cubic(t);
}