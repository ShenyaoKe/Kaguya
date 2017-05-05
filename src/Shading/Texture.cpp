#include "Shading/Texture.h"

namespace Kaguya
{

Texture::Texture()
{
}
Texture::~Texture()
{
}
ColorRGBA Texture::getColor(const Point2f &uv) const
{
	return ColorRGBA();
}
ColorRGBA Texture::getColor(const DifferentialGeometry* queryPoint) const
{
	ColorRGBA ret;// = getColor(queryPoint->UV);
	return ret;
}
/************************************************************************/
/* File Texture                                                         */
/************************************************************************/
FileTexture::FileTexture()
{
}
FileTexture::FileTexture(const std::string &filename)
{
	img = new ImageData(filename);
}
FileTexture::FileTexture(ImageData &filename)
{
	img = &filename;
}
FileTexture::~FileTexture()
{
	img = nullptr;
}
ColorRGBA FileTexture::getColor(const Point2f &uv) const
{
	return img->bilinearPixel(uv.x * img->getWidth(), uv.y * img->getHeight());
}
/************************************************************************/
/* Perlin Noise                                                         */
/************************************************************************/
PerlinNoiseTexture::PerlinNoiseTexture()
{
}
PerlinNoiseTexture::PerlinNoiseTexture(Float persistence, int octaves, int wd, int ht)
{
	noise.setData(persistence, octaves, wd, ht);
}
PerlinNoiseTexture::~PerlinNoiseTexture()
{
}
ColorRGBA PerlinNoiseTexture::getColor(const Point2f &uv) const
{
	Float pValue = clampFromZeroToOne((noise.getValue(uv) - vmin) / (vmax - vmin));
	return pValue * brightColor + (1 - pValue)*darkColor;
}

}
