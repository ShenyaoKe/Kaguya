#pragma once

//#include "Core/rtdef.h"
#include "Image/ImageData.h"
#include "Geometry/Geometry.h"
#include "Shading/Noise.h"
#include "Geometry/Intersection.h"

namespace Kaguya
{

class Texture
{
public:

	Texture();
	virtual ~Texture();

	virtual ColorRGBA getColor(const Point2f &uv) const;
	virtual ColorRGBA getColor(const Intersection* isec) const;
};

/************************************************************************/
/* File Texture                                                         */
/************************************************************************/
class FileTexture :public Texture
{
	ImageData* img = nullptr;
public:
	FileTexture();
	FileTexture(const std::string &filename);
	FileTexture(ImageData &image);
	~FileTexture();

	void assignImage(ImageData* &image);
	ColorRGBA getColor(const Point2f &uv) const;
protected:

private:
};
/************************************************************************/
/* Perlin Noise                                                         */
/************************************************************************/
class PerlinNoiseTexture :public Texture
{
	PerlinNoise2D noise;
	ColorRGBA brightColor = COLOR_LIGHT_YELLOW;
	ColorRGBA darkColor = COLOR_SADDLE_BROWN;
	Float vmin = -1;
	Float vmax = 1;
public:
	PerlinNoiseTexture();
	PerlinNoiseTexture(Float persistence, int octaves, int wd, int ht);
	~PerlinNoiseTexture();
	ColorRGBA getColor(const Point2f &uv) const;
protected:

private:
};

}
