#pragma once
#ifndef __TEXTURESYNTHESIS__
#define __TEXTURESYNTHESIS__

#include "Image/ImageData.h"
namespace TextureSynthesis
{
	inline Vector3f ColorToVector(const ColorRGBA &clr)
	{
		return Vector3f(clr.r, clr.g, clr.b);

	}
	ImageData* synth(const ImageData* src, int wdt = 640, int hgt = 480, int nr = 2);
}


#endif // __TEXTURESYNTHESIS__