//
//  ImageData.h
//
//  Created by Shenyao Ke on 10/11/14.
//  Copyright (c) 2014 Shenyao. All rights reserved.
//
#pragma once

#include "Core/MemoryControl.h"
#include "Math/Vector.h"
#include "Image/ColorData.h"
//#include "ppmImage.h"
#include "Math/Matrix3x3.h"

//read image files
#include <FreeImage.h>

namespace Kaguya
{

// 
/************************************************************************/
// Image pixel values are stored in a row-major-storage matrix
// The pixel at (row, col) is stored in pixels[col][row]
/************************************************************************/
class ImageData
{
protected:
	uint32_t mWidth, mHeight, mBPP;
	AlignedArray2D<ColorRGBA> mPixels;
public:
	enum PixmapType : uint8_t
	{
		RGB,
		RGBA,
		ARGB,
		BGR,
		BGRA,
		ABGR
	};
	//ImageData();
	//ImageData(ppmImage &ppmData);
	ImageData(uint32_t wd = default_resX, uint32_t ht = default_resY);
	ImageData(uint32_t wd, uint32_t ht, Float* &pixMap);//pixMap stores rgb data
	explicit ImageData(uint32_t wd, uint32_t ht, unsigned char* pixMap, uint8_t pixtype = RGB);//pixMap stores rgb data
	ImageData(const std::string &filename);
	ImageData(const ImageData &src);
	virtual~ImageData();

	uint32_t getWidth() const { return mWidth; }
	uint32_t getHeight() const { return mHeight; }
	uint32_t getBPP() const { return mBPP; }
	const ColorRGBA &getRGBA(uint32_t x, uint32_t y) const;
	const ColorRGBA &getRGBA(uint32_t idx) const;
	void setRGBA(uint32_t x, uint32_t y, const ColorRGBA &color);
	void getPixels(unsigned char* &pixMap) const;
	void getPixelsRGBA(unsigned char* &pixMap) const;
	AlignedArray2D<uint32_t>* genHist() const;//Generate histogram
	AlignedArray2D<Float>* getLuma() const;// Generate Luma

	void printRGBA(uint32_t x, uint32_t y) const;
	void resize(uint32_t x, uint32_t y);
	ColorRGBA bilinearPixel(Float x, Float y) const;
	//virtual const ColorRGBA &bicubicPixel(Float x, Float y) const;
	bool writeFile(const std::string &filename) const;
	//ColorRGB bicubicPixel(Float x, Float y);
	//void convert2PPM(ppmImage &ppmData);

	//friend const ImageData &downsampling(const ImageData &src);
};

}
