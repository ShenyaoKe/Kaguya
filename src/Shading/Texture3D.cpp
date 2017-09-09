#include "Texture3D.h"

namespace Kaguya
{

Texture3D::Texture3D()
{
}


Texture3D::~Texture3D()
{
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
void PerlinNoiseTexture3D::setColor(const ColorRGBA &bc, const ColorRGBA &dc)
{
	brightColor = bc;
	darkColor = dc;
}
void PerlinNoiseTexture3D::setRange(Float max_value, Float min_value)
{
	vmin = min_value;
	vmax = max_value;
}
ColorRGBA PerlinNoiseTexture3D::getColor(const Point3f &pos) const
{
	Float pValue = clampFromZeroToOne((noise.getValue(pos) - vmin) / (vmax - vmin));
	//std::cout << noise.getValue(isec->UV) << std::endl;
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
	detailNoise.setData(persistence * 0.5, octaves, size * 40, size * 40, size * 40);
	setColor(COLOR_SADDLE_BROWN, COLOR_BLACK);
}
WoodTexture3D::~WoodTexture3D()
{
}
ColorRGBA WoodTexture3D::getColor(const Point3f &pos) const
{
	Float pValue = threshold * (noise.getValue(pos) - vmin) / (vmax - vmin);
	pValue -= static_cast<int>(pValue);
	pValue += detailNoise.getValue(pos) * 0.5;
	clampFromZeroToOne(pValue);
	//std::cout << noise.getValue(isec->UV) << std::endl;
	return pValue * brightColor + (1 - pValue)*darkColor;
}

}
