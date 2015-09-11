#pragma once
#ifndef __IMAGEPYRAMID__
#define __IMAGEPYRAMID__

#include "Image/ImageData.h"
#include "Image/Filter.h"
class ImagePyramid
{
	int levels;
	vector<ImageData*> layers;
public:
	ImagePyramid(const ImageData &src, int lv = 3);
	~ImagePyramid();

	void getPixels(int &wdt, int &hgt, unsigned char* &pixMap) const;
};


#endif // __IMAGEPYRAMID__