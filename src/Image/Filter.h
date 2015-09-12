//
//  Filter.cpp
//
//  Created by AKIKA on 10/11/14.
//  Copyright (c) 2014 AKIKA. All rights reserved.
//
#pragma once
#ifndef __FILTER__
#define __FILTER__

#include "Image/ImageData.h"
#include "Math/CGVector.h"
namespace filter
{

	typedef enum EdgeOperator
	{
		SOBEL,
		PREWITT,
		ROBERT
	};

	ImageData* curveAdj(const ImageData* cvImg, const ImageData* src);
	ImageData* reverse(const ImageData* src);
	ImageData* offset(const ImageData* src, int xOffset = 0, int yOffset = 0);
	ImageData* smartOffset(const ImageData* src, int xOffset = 0, int yOffset = 0, int bWid = 0);
	ImageData* hsvOffset(const ImageData* src, Float hue = 0.0, Float saturation = 0.0, Float value = 0.0);
	ImageData* hueSwitch(const ImageData* src, Float oriHue = 0.0, Float range = 0.0, Float newHue = 0.0);
	ImageData* equalize(const ImageData* src);
	ImageData* equalizeLuma(const ImageData* src);
	ImageData* posterize(const ImageData* src, int level = 8);
	ImageData* threshold(const ImageData* src, Float th = 0.5);
	ImageData* edgeDetect(const ImageData* src, Float **lumaImg = nullptr, EdgeOperator opType = EdgeOperator::SOBEL);

	ImageData* boxBlur(const ImageData* src, int radius = 3);
	ImageData* gaussianBlur(const ImageData* src, int radius = 3);
	ImageData* smartBlur(const ImageData* src, int radius, int th);
	ImageData* motionBlur(const ImageData* src, int radius);
	ImageData* motionBlur(const ImageData* src, const ImageData* mvImg, int radius);
	ImageData* bilateral(const ImageData* src, int radius);

	ImageData* emboss(const ImageData* src, Float theta, int radius);
	ImageData* dilation(const ImageData* src, int radius);
	ImageData* dilationRGB(const ImageData* src, int radius);
	ImageData* erosion(const ImageData* src, int radius);
	ImageData* mosaic(const ImageData* src, int size);
	ImageData* triangulate(const ImageData* src, int size);
	ImageData* circlepix(const ImageData* src, int radius, Float para);

	// Transform
	ImageData* translate(const ImageData* src, const Vector2D& translation, int& winWid, int& winHgt);
	ImageData* rotate(const ImageData* src, Float theta, int& winWid, int& winHgt);
	ImageData* scale(const ImageData* src, const Vector2D& sVec, int& winWid, int& winHgt);
	ImageData* shear(const ImageData* src, const Vector2D& shVec, int& winWid, int& winHgt);
	ImageData* reflect(const ImageData* src, const Vector2D& rflVec, int& winWid, int& winHgt);
	ImageData* perspective(const ImageData* src, const Vector2D& vPnt1, int& winWid, int& winHgt);
	ImageData* bilnear(const ImageData* src, const Vector2D* pos, int& winWid, int& winHgt);// 4Corner points
	ImageData* inflateScale(const ImageData* src, Float radius, int& winWid, int& winHgt);
	ImageData* textureSynth(const ImageData* sample, int radius);
}

#endif // __FILTER__