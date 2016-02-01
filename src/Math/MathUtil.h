#pragma once
#ifndef __MATHUTIL__
#define __MATHUTIL__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT


#include <cmath>
#include <algorithm>
#include "Core/Kaguya.h"
/************************************************************************/
/* Definition                                                           */
/************************************************************************/
#ifndef M_PI
static const Float M_PI = 3.14159265358979323846;
#endif
static const Float M_HALFPI = M_PI * 0.5;
static const Float M_TWOPI = M_PI * 2.0;
static const Float INV_PI = 1.0 / M_PI;
static const Float INV_TWOPI = INV_PI / 2.0;
static const Float INV_FOURPI = INV_PI / 4.0;
static const Float NUM_ZERO = 0;
static const Float NUM_INFINITY = INFINITY;// std::numeric_limits<Float>::infinity();
static const Float NUM_NAN = NAN;// std::numeric_limits<Float>::quiet_NaN();
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

inline bool chechSign(double val)
{
	//Positive 0
	if (val > std::numeric_limits<double>::epsilon())
	{
		return 0;
	}
	//Negative 1
	else
	{
		return 1;
	}
}
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
template <typename T>
inline const T& remap(const T& data, const T& oldMin, const T& oldMax,
	const T& newMin, const T& newMax)
{
	T tmp = (data * (newMax - newMin) - (newMin * oldMax - newMax * oldMin))
		/ (oldMax - oldMin);
	return clamp(tmp, newMin, newMax);
}
template <typename T>
inline const T& RemapToUnit(const T& data, const T& minV, const T& maxV)
{
	return clampFromZeroToOne((data - minV) / (maxV - minV));
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
inline bool inUnitRange(Float data)
{
	Float epsilon = 5e-4f;
	return inRange(data, 0.f - epsilon, 1.f + epsilon);
}
inline bool inUnitRange(Float data, Float epsilon)
{
	//Float epsilon = 5e-4f;
	return inRange(data, 0.f - epsilon, 1.f + epsilon);
}
inline Float RadianToDegree(Float radian)
{
	return radian * 180.0 * INV_PI;
}
inline Float DegreeToRadian(Float degree)
{
	return degree * M_PI / 180.0;
}
inline float isFuzzyNull(float val)
{
	return abs(val) < 0.00001f;
}
inline double isFuzzyNull(double val)
{
	return abs(val) < 0.000000000001;
}
inline int Ceil2Int(Float val)
{
	return static_cast<int>(ceil(val));
}
inline int Floor2Int(Float val)
{
	return static_cast<int>(floor(val));
}
inline int Round2Int(Float val)
{
	return Floor2Int(val + 0.5);
}
inline Float Log2Int(Float val)
{
	//static Float invLog2 = 1.0 / log(2.0);
	return Floor2Int(log2(val));
}
inline int findMinPositiveT(Float* &ts, int size)
{
	Float tmp = INFINITY;
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
inline Float unitRandom(int segment)
{
	return static_cast<Float>(Random(segment)) / static_cast<Float>(segment);
}
inline Float symUnitRandom(int segment)
{
	return (static_cast<Float>(Random(segment)) / static_cast<Float>(segment)-0.5) * 2.0;
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
inline T lerp(const T& data1, const T& data2, const Float& t)//Linear Interpolation
{
	return data1 * (1 - t) + data2 * t;
}
template <typename T>
inline T cosInterpolate(const T& data1, const T& data2, const Float& t)//cosInterpolate
{
	Float nt = (1 - cos(t * M_PI)) * 0.5;
	return data1 * (1 - nt) + data2 * nt;
}
template <typename T>
inline T cubicInterpolate(const T& data0, const T& data1, const T& data2, const T& data3, const Float& t)
{
	T P = (data3 - data2) - (data0 - data1);
	T Q = (data0 - data1) - P;
	T R = data2 - data0;
	T S = data1;
	return P * cubic(t) + Q * sqr(t) + R * t + S;
}
inline Float curve_t(const Float& t)//Mainly used for perlin noise
{
	return 6 * cubic(t) * sqr(t) - 15 * sqr(sqr(t)) + 10 * cubic(t);
}
#endif // __MATHUTIL__