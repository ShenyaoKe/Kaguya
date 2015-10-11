#pragma once
#ifndef __SPECTRUM__
#define __SPECTRUM__

//#include "Core/rtdef.h"
#include "Image/ColorData.h"

/************************************************************************/
/* Spuctrum                                                             */
/************************************************************************/
class Spectrum
{
public:
	ColorRGBA color;
	Float intensity;

	Spectrum();
	Spectrum(Float its, ColorRGBA spc);
	Spectrum(ColorRGBA spc);
	~Spectrum();

	ColorRGBA operator*(const ColorRGBA& color2) const;
	friend ColorRGBA operator*(const ColorRGBA& color, const Spectrum& spec);
	void printInfo() const;
protected:

private:
};

#endif // __SPECTRUM__