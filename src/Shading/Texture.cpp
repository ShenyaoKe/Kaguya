#include "Shading/Texture.h"

Texture::Texture()
{
}
Texture::~Texture()
{
}
ColorRGBA Texture::getColor(const Point3D& uv) const
{
	return ColorRGBA();
}
ColorRGBA Texture::getColor(const DifferentialGeometry *queryPoint) const
{
	ColorRGBA ret = getColor(queryPoint->UV);
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
	img = NULL;
}
ColorRGBA FileTexture::getColor(const Point3D& uv) const
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
	nightImg = NULL;
	delete dayImg;
	dayImg = NULL;
}
ColorRGBA PlannetTexture::getColor(const DifferentialGeometry *queryPoint) const
{
	Float ramp = queryPoint->getDiffuseTheta();
	ramp = (ramp + tolerance) / (2 * tolerance);
	clampFromZeroToOne(ramp);
	if (ramp == 0)
	{
		return nightImg->bilinearPixel(queryPoint->UV.x * nightImg->getWidth(), queryPoint->UV.y * nightImg->getHeight());
	}
	else if (ramp == 1)
	{
		return dayImg->bilinearPixel(queryPoint->UV.x * dayImg->getWidth(), queryPoint->UV.y * dayImg->getHeight());
	}
	else
	{
		return dayImg->bilinearPixel(queryPoint->UV.x * dayImg->getWidth(), queryPoint->UV.y * dayImg->getHeight()) * ramp
			+ nightImg->bilinearPixel(queryPoint->UV.x * nightImg->getWidth(), queryPoint->UV.y * nightImg->getHeight()) * (1 - ramp);
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
ColorRGBA PerlinNoiseTexture::getColor(const Point3D& uv) const
{
	Float pValue = clampFromZeroToOne((noise.getValue(uv) - vmin) / (vmax - vmin));
	return pValue * brightColor + (1 - pValue)*darkColor;
}
/************************************************************************/
/* PerlinNoise 3D                                                       */
/************************************************************************/
PerlinNoiseTexture3D::PerlinNoiseTexture3D()
{
}
PerlinNoiseTexture3D::PerlinNoiseTexture3D(Float persistence, int octaves,
	int wd, int ht, int dp)
{
	noise.setData(persistence, octaves, wd, ht, dp);
}
PerlinNoiseTexture3D::~PerlinNoiseTexture3D()
{
}
void PerlinNoiseTexture3D::setColor(const ColorRGBA& bc, const ColorRGBA& dc)
{
	brightColor = bc;
	darkColor = dc;
}
void PerlinNoiseTexture3D::setRange(const Float& max_value, const Float& min_value)
{
	vmin = min_value;
	vmax = max_value;
}
ColorRGBA PerlinNoiseTexture3D::getColor(const Point3D& uv) const
{
	Float pValue = clampFromZeroToOne((noise.getValue(uv) - vmin) / (vmax - vmin));
	//cout << noise.getValue(queryPoint->UV) << endl;
	return pValue * brightColor + (1 - pValue)*darkColor;
}
/************************************************************************/
/* Wood Texture from Perlin Noise 3D                                    */
/************************************************************************/
WoodTexture3D::WoodTexture3D()
{
}
WoodTexture3D::WoodTexture3D(Float persistence, int octaves, int size, int thr)
	:PerlinNoiseTexture3D(persistence, octaves, size, size, size), threshold(thr)
{
	detailNoise.setData(persistence*0.5, octaves, size * 40, size * 40, size * 40);
	setColor(COLOR_SADDLE_BROWN, COLOR_BLACK);
}
WoodTexture3D::~WoodTexture3D()
{
}
ColorRGBA WoodTexture3D::getColor(const Point3D& uv) const
{
	Float pValue = threshold * (noise.getValue(uv) - vmin) / (vmax - vmin);
	pValue -= static_cast<int>(pValue);
	pValue += detailNoise.getValue(uv) * 0.5;
	clampFromZeroToOne(pValue);
	//cout << noise.getValue(queryPoint->UV) << endl;
	return pValue * brightColor + (1 - pValue)*darkColor;
}
/************************************************************************/
/* Solid File Texture                                                   */
/************************************************************************/
SolidFileTexture::SolidFileTexture()
{
}
SolidFileTexture::SolidFileTexture(const string& filename)
{
	img = new ImageData(filename);
}
SolidFileTexture::SolidFileTexture(ImageData& filename)
{
	img = &filename;
}
SolidFileTexture::~SolidFileTexture()
{
	delete img;
	img = NULL;
}
ColorRGBA SolidFileTexture::getColor(const Point3D& uv) const
{
	return img->bilinearPixel(uv.x * img->getWidth(), uv.y * img->getHeight());
}