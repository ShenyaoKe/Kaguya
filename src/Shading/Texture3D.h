#pragma once

//#include "Core/rtdef.h"
#include "Image/ImageData.h"
#include "Geometry/Primitive.h"
#include "Shading/Noise.h"
#include "Geometry/Intersection.h"

namespace Kaguya
{

class Texture3D
{
public:
	Texture3D();
	virtual ~Texture3D() = 0;
};

/************************************************************************/
/* PerlinNoise 3D                                                       */
/************************************************************************/
class PerlinNoiseTexture3D :public Texture3D
{
protected:
	PerlinNoise3D noise;
	ColorRGBA brightColor = COLOR_LIGHT_YELLOW;
	ColorRGBA darkColor = COLOR_SADDLE_BROWN;
	Float vmin = -1;
	Float vmax = 1;
public:
	PerlinNoiseTexture3D();
	PerlinNoiseTexture3D(Float persistence, int octaves, int wd, int ht, int dp);
	~PerlinNoiseTexture3D();

	virtual void setColor(const ColorRGBA &bc, const ColorRGBA &dc);
	virtual void setRange(Float max_value, Float min_value);
	ColorRGBA getColor(const Point3f &pos) const;
protected:

private:
};
/************************************************************************/
/* Wood Texture from Perlin Noise 3D                                    */
/************************************************************************/
class WoodTexture3D :public PerlinNoiseTexture3D
{
	int threshold = 20;
	PerlinNoise3D detailNoise;
public:
	WoodTexture3D();
	WoodTexture3D(Float persistence, int octaves, int size, int thr);
	~WoodTexture3D();

	ColorRGBA getColor(const Point3f &uv) const;
protected:

private:
};

}
