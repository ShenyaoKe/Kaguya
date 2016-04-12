#pragma once
#ifndef __TEXTURE__
#define __TEXTURE__

//#include "Core/rtdef.h"
#include "Image/ImageData.h"
#include "Geometry/Shape.h"
#include "Shading/Noise.h"
#include "Geometry/DifferentialGeometry.h"

class Texture
{
public:

	Texture();
	virtual ~Texture();

	virtual ColorRGBA getColor(const Vector3D &uv) const;
	virtual ColorRGBA getColor(const DifferentialGeometry* queryPoint) const;
};

/************************************************************************/
/* File Texture                                                         */
/************************************************************************/
class FileTexture :public Texture
{
	ImageData* img = nullptr;
public:
	FileTexture();
	FileTexture(const string& filename);
	FileTexture(ImageData& image);
	~FileTexture();

	void assignImage(ImageData* &image);
	ColorRGBA getColor(const Vector3D &uv) const;
protected:

private:
};
/************************************************************************/
/* Planet Texture                                                       */
/************************************************************************/
class PlannetTexture :public Texture
{
	ImageData* nightImg = nullptr;
	ImageData* dayImg = nullptr;
	Float tolerance = 0.1;
public:
	PlannetTexture();
	PlannetTexture(const string& nightfile, const string& dayfile);
	~PlannetTexture();

	ColorRGBA getColor(const DifferentialGeometry* queryPoint) const;
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
	ColorRGBA getColor(const Vector3D &uv) const;
protected:

private:
};
/************************************************************************/
/* PerlinNoise 3D                                                       */
/************************************************************************/
class PerlinNoiseTexture3D :public Texture
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

	virtual void setColor(const ColorRGBA& bc, const ColorRGBA& dc);
	virtual void setRange(const Float& max_value, const Float& min_value);
	ColorRGBA getColor(const Vector3D &uv) const;
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

	ColorRGBA getColor(const Vector3D &uv) const;
protected:

private:
};
/************************************************************************/
/* Solid File Texture                                                   */
/************************************************************************/
class SolidFileTexture :public Texture
{
	ImageData* img = nullptr;
public:
	SolidFileTexture();
	SolidFileTexture(const string& filename);
	SolidFileTexture(ImageData& image);
	~SolidFileTexture();

	void assignImage(ImageData* &image);
	ColorRGBA getColor(const Vector3D &uv) const;
protected:

private:
};
#endif // __TEXTURE__