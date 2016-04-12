#pragma once
#ifndef __TEXTURESYNTHESIS__
#define __TEXTURESYNTHESIS__

#include "Image/ImageData.h"

const Vector3D &ColorToVector(const ColorRGBA &clr)
{
	Vector3D ret(clr.r, clr.g, clr.b);
	return ret;
}
ImageData* synth(const ImageData * src, int wdt = 640, int hgt = 480, int nr = 2);


#endif // __TEXTURESYNTHESIS__