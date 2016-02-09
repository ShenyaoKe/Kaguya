//
//  ImageData.h
//
//  Created by Shenyao Ke on 10/11/14.
//  Copyright (c) 2014 Shenyao. All rights reserved.
//

#ifndef __ImageData__
#define __ImageData__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
//#include <math.h>

#include "Core/MemoryControl.h"
#include "Math/CGVector.h"
#include "Image/ColorData.h"
//#include "ppmImage.h"
#include "Math/Matrix3D.h"

//read image files
#include "FreeImage.h"
//using namespace std;


// 
/************************************************************************/
// Image pixel values are stored in a row-major-storage matrix
// The pixel at (row, col) is stored in pixels[col][row]
/************************************************************************/
class ImageData
{
protected:
	int width, height, bpp;
	ColorRGBA **pixels;
public:
	enum PixmapType
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
	ImageData(int wd = default_resX, int ht = default_resY);
	ImageData(int wd, int ht, Float* &pixMap);//pixMap stores rgb data
	ImageData(int wd, int ht, const unsigned char* &pixMap, int pixtype = RGB);//pixMap stores rgb data
	ImageData(const string &filename);
	ImageData(const ImageData& src);
	virtual~ImageData();
    
	virtual const int getWidth() const { return width; }
	virtual const int getHeight() const { return height; }
	virtual const int getBPP() const { return bpp; }
	virtual const ColorRGBA& getRGBA(int x, int y) const;
	virtual const ColorRGBA& getRGBA(int idx) const;
	virtual void setRGBA(int x, int y, const ColorRGBA& color);
	virtual void getPixels(unsigned char* &pixMap) const;
	virtual void getPixelsRGBA(unsigned char* &pixMap) const;
	virtual int** genHist() const;//Generate histogram
	virtual Float** getLuma() const;// Generate Luma
	
	virtual void printRGBA(int x, int y) const;
	virtual void resize(int x, int y);
	virtual ColorRGBA bilinearPixel(Float x, Float y) const;
	//virtual const ColorRGBA& bicubicPixel(Float x, Float y) const;
	virtual bool writeFile(const string &filename) const;
	//ColorRGB bicubicPixel(Float x, Float y);
	//void convert2PPM(ppmImage &ppmData);

	//friend const ImageData& downsampling(const ImageData& src);
};
#endif /* defined(__ImageData__) */
