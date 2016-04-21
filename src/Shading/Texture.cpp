#include "Shading/Texture.h"

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
FileTexture::FileTexture(const string& filename)
{
	img = new ImageData(filename);
}
FileTexture::FileTexture(ImageData& filename)
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
/* Planet Texture                                                       */
/************************************************************************/
PlannetTexture::PlannetTexture(const string& nightfile, const string& dayfile)
{
	nightImg = new ImageData(nightfile);
	dayImg = new ImageData(dayfile);
}
PlannetTexture::~PlannetTexture()
{
	delete nightImg;
	nightImg = nullptr;
	delete dayImg;
	dayImg = nullptr;
}
ColorRGBA PlannetTexture::getColor(const DifferentialGeometry* queryPoint) const
{
	Float ramp;// = queryPoint->getDiffuseTheta();
	ramp = (ramp + tolerance) / (2 * tolerance);
	clampFromZeroToOne(ramp);
	if (ramp == 0)
	{
		return nightImg->bilinearPixel(queryPoint->uv.x * nightImg->getWidth(), queryPoint->uv.y * nightImg->getHeight());
	}
	else if (ramp == 1)
	{
		return dayImg->bilinearPixel(queryPoint->uv.x * dayImg->getWidth(), queryPoint->uv.y * dayImg->getHeight());
	}
	else
	{
		return dayImg->bilinearPixel(queryPoint->uv.x * dayImg->getWidth(), queryPoint->uv.y * dayImg->getHeight()) * ramp
			+ nightImg->bilinearPixel(queryPoint->uv.x * nightImg->getWidth(), queryPoint->uv.y * nightImg->getHeight()) * (1 - ramp);
	}
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
