#pragma once
#ifndef __NOISE__
#define __NOISE__

#include "Math/MathUtil.h"
#include "Math/CGVector.h"

inline double Noise(int x)
{
	x = (x << 13) ^ x;
	return 1.0 - ((x * (sqr(x) * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}
inline double Noise(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}
inline double Noise(int x, int y, int z)
{
	int n = x + y * 57 + z * 940;
	n = (n << 13) ^ n;
	return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}
inline double SmoothNoise1D(int x)
{
	return Noise(x) / 2 + Noise(x - 1) / 4 + Noise(x + 1) / 4;
}
inline double SmoothNoise2D(int x, int y)
{
	double corner = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
	double sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
	double center = Noise(x, y) / 4;

	return corner + sides + center;
}
inline double SmoothNoise3D(int x, int y, int z)
{
	double corner = (Noise(x - 1, y - 1, z - 1) + Noise(x + 1, y - 1, z - 1) + Noise(x - 1, y + 1, z - 1) + Noise(x + 1, y + 1, z - 1)
		+ Noise(x - 1, y - 1, z + 1) + Noise(x + 1, y - 1, z + 1) + Noise(x - 1, y + 1, z + 1) + Noise(x + 1, y + 1, z + 1)) / 64;
	double sides = (Noise(x - 1, y, z - 1) + Noise(x + 1, y, z - 1) + Noise(x, y - 1, z - 1) + Noise(x, y + 1, z - 1)
		+ Noise(x - 1, y - 1, z) + Noise(x + 1, y - 1, z) + Noise(x - 1, y + 1, z) + Noise(x + 1, y + 1, z)
		+ Noise(x - 1, y, z + 1) + Noise(x + 1, y, z + 1) + Noise(x, y - 1, z + 1) + Noise(x, y + 1, z + 1)) / 32;
	double faces = (Noise(x, y, z - 1) + Noise(x, y, z + 1) 
		+ Noise(x - 1, y, z) + Noise(x + 1, y, z) + Noise(x, y - 1, z) + Noise(x, y + 1, z)) / 16;
	double center = Noise(x, y, z) / 8;

	return corner + sides + faces + center;
}
inline double interpolateNoise(double x)
{
	int floorX = floor(x);
	double t = curve_t(x - floorX);
	return lerp(SmoothNoise1D(x), SmoothNoise1D(x + 1), t);
}
inline double interpolateNoise(double x, double y)
{
	int floorX = floor(x);
	double tx = curve_t(x - floorX);
	int floorY = floor(y);
	double ty = curve_t(y - floorY);

	double v1 = SmoothNoise2D(floorX, floorY);
	double v2 = SmoothNoise2D(floorX + 1, floorY);
	double v3 = SmoothNoise2D(floorX, floorY + 1);
	double v4 = SmoothNoise2D(floorX + 1, floorY + 1);

	return lerp(lerp(v1, v2, tx), lerp(v3, v4, tx), ty);
}
inline double interpolateNoise(double x, double y, double z)
{
	int floorX = floor(x);
	double tx = curve_t(x - floorX);
	int floorY = floor(y);
	double ty = curve_t(y - floorY);
	int floorZ = floor(z);
	double tz = curve_t(z - floorZ);

	double v1 = SmoothNoise3D(floorX, floorY, floorZ);
	double v2 = SmoothNoise3D(floorX + 1, floorY, floorZ);
	double v3 = SmoothNoise3D(floorX, floorY + 1, floorZ);
	double v4 = SmoothNoise3D(floorX + 1, floorY + 1, floorZ);
	double v5 = SmoothNoise3D(floorX, floorY, floorZ + 1);
	double v6 = SmoothNoise3D(floorX + 1, floorY, floorZ + 1);
	double v7 = SmoothNoise3D(floorX, floorY + 1, floorZ + 1);
	double v8 = SmoothNoise3D(floorX + 1, floorY + 1, floorZ + 1);

	return lerp(lerp(lerp(v1, v2, tx), lerp(v3, v4, tx), ty),
		lerp(lerp(v5, v6, tx), lerp(v7, v8, tx), ty), tz);
}
inline double PerlinNoise1D(double x)
{
	double total = 0;
	double p = 1;//persistence
	int n = 4 - 1;//numOfOctaves - 1

	for (int i = 0; i < n; i++)
	{
		double freq = pow(2, i);
		double amp = pow(p, i);
		total += interpolateNoise(x*freq) * amp;
	}
	return total;
}
/************************************************************************/
/* Perlin Noise 1D                                                      */
/************************************************************************/
class PerlinNoise1D
{
	double p = 1;//persistence
	int oct = 1;//Octaves
	int len = 100;
public:
	PerlinNoise1D();
	PerlinNoise1D(int persistence, int octaves, int lt);
	virtual  ~PerlinNoise1D();
	virtual double getValue(double x);
private:

};

inline PerlinNoise1D::PerlinNoise1D()
{
}
inline PerlinNoise1D::PerlinNoise1D(int persistence, int octaves, int lt)
{
	p = persistence;
	oct = octaves;
	len = lt;
}
inline PerlinNoise1D::~PerlinNoise1D()
{
}
inline double PerlinNoise1D::getValue(double x)
{
	double total = 0;

	for (int i = 0; i < oct; i++)
	{
		double freq = pow(2, i);
		double amp = pow(p, i);
		total += interpolateNoise(x*freq) * amp;
	}
	return total;
}
/************************************************************************/
/* Perlin Noise 2D                                                      */
/************************************************************************/
class PerlinNoise2D
{
public:
	double p = 1;//persistence
	int oct = 1;//Octaves
	int width = 100;
	int height = 100;

	PerlinNoise2D();
	PerlinNoise2D(double persistence, int octaves, int wd, int ht);
	virtual ~PerlinNoise2D();
	virtual void setData(double persistence, int octaves, int wd, int ht);
	virtual double getValue(const Point3D& uv) const;
private:

};

inline PerlinNoise2D::PerlinNoise2D()
{
}
inline PerlinNoise2D::PerlinNoise2D(double persistence, int octaves, int wd, int ht)
{
	p = persistence;
	oct = octaves;
	width = wd;
	height = ht;
}
inline PerlinNoise2D::~PerlinNoise2D()
{
}
inline void PerlinNoise2D::setData(double persistence, int octaves, int wd, int ht)
{
	p = persistence;
	oct = octaves;
	width = wd;
	height = ht;
}
inline double PerlinNoise2D::getValue(const Point3D& uv) const
{
	double total = 0;

	for (int i = 0; i < oct; i++)
	{
		double freq = pow(2, i);
		double amp = pow(p, i);
		total += interpolateNoise(uv.x * width * freq, uv.y * height * freq) * amp;
		//total += interpolateNoise(uv.x * freq, uv.y * freq) * amp;
	}
	return total;
}
/************************************************************************/
/* Perlin Noise 3D                                                      */
/************************************************************************/
class PerlinNoise3D
{
public:
	double p = 1;//persistence
	int oct = 1;//Octaves
	int width = 100;
	int height = 100;
	int depth = 100;

	PerlinNoise3D();
	PerlinNoise3D(double persistence, int octaves, int wd, int ht, int dp);
	virtual ~PerlinNoise3D();
	virtual void setData(double persistence, int octaves, int wd, int ht, int dp);
	virtual double getValue(const Point3D& uv) const;
private:

};

inline PerlinNoise3D::PerlinNoise3D()
{
}
inline PerlinNoise3D::PerlinNoise3D(double persistence, int octaves, int wd, int ht, int dp)
{
	p = persistence;
	oct = octaves;
	width = wd;
	height = ht;
	depth = dp;
}
inline PerlinNoise3D::~PerlinNoise3D()
{
}
inline void PerlinNoise3D::setData(double persistence, int octaves, int wd, int ht, int dp)
{
	p = persistence;
	oct = octaves;
	width = wd;
	height = ht;
	depth = dp;
}
inline double PerlinNoise3D::getValue(const Point3D& uv) const
{
	double total = 0;

	for (int i = 0; i < oct; i++)
	{
		double freq = pow(2, i);
		double amp = pow(p, i);
		total += interpolateNoise(uv.x * width * freq, uv.y * height * freq, uv.z * depth * freq) * amp;
		//total += interpolateNoise(uv.x * freq, uv.y * freq) * amp;
	}
	return total;
}
/************************************************************************/
/* wood texture                                                         */
/************************************************************************/

#endif // __NOISE__