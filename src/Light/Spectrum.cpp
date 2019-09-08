#include "Light/Spectrum.h"

namespace Kaguya
{

#ifdef USE_LEGACY_SPECTRUM
LegacySpectrum::LegacySpectrum()
{
	mColor = COLOR_WHITE;
	mIntensity = 1;
}

LegacySpectrum::LegacySpectrum(Float its)
	: mColor(COLOR_WHITE)
	, mIntensity(its)
{
}

LegacySpectrum::LegacySpectrum(Float its, ColorRGBA spc)
{
	mIntensity = its;
	mColor = spc;
}

LegacySpectrum::LegacySpectrum(ColorRGBA spc)
{
	mColor = spc;
	mIntensity = 1;
}

LegacySpectrum::~LegacySpectrum()
{
}

ColorRGBA LegacySpectrum::operator*(const ColorRGBA &color2) const
{
	return mColor * color2 * mIntensity;
}

LegacySpectrum LegacySpectrum::operator*(Float t) const
{
	return LegacySpectrum(mIntensity * t, mColor);
}

LegacySpectrum LegacySpectrum::operator/(Float t) const
{
	return LegacySpectrum(mIntensity / t, mColor);
}

ColorRGBA operator*(const ColorRGBA &color, const LegacySpectrum &spec)
{
	return spec * color;
}

LegacySpectrum operator*(Float t, const LegacySpectrum & spec)
{
	return spec * t;
}

void LegacySpectrum::printInfo() const
{
	std::cout << "Intensity:\t" << mIntensity << std::endl;
	std::cout << "Color:";
	mColor.printInfo();
}
#endif

}
