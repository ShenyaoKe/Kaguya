//
//  film.h
//
//  Created by Shenyao Ke on 1/29/15.
//  Copyright (c) 2014 AKIKA. All rights reserved.
//
#pragma once
#ifndef __FILM__
#define __FILM__
#include "Core/rtdef.h"
#include "Image/ImageData.h"

typedef unsigned char RES_FIT_TYPE;

typedef enum 
{
	FT_SQUARE = 0,
	FT_16MM_THEATRICAL = 1,
	FT_35MM_ACADEMY = 2,
	FT_IMAX = 9
}FILM_TYPE;
typedef enum
{
	FRG_HORIZONTAL_FIT = 0,
	FRG_VERTICAL_FIT = 1,
	FRG_FILL_FIT = 2,
	FRG_OVERSCAN_FIT = 3
}FIT_RESOLUTION_GATE;



class Film:public ImageData
{
public:
	Film() : ImageData(default_resX, default_resY){};
	~Film();
	Film(FILM_TYPE filmType, int resX, int resY);
	Film(FILM_TYPE filmType, int resX, int resY, FIT_RESOLUTION_GATE fitType);

	void setFilmType(FILM_TYPE filmType);
	void setFitType(FIT_RESOLUTION_GATE fitType);

	double getHoriApect() const { return horiApect; }
	double getVertApect() const { return vertApect; }
	Point2D getFilmPos(double imgX, double imgY) const;// from -1 * len to +1 * len
	Point2D getImgPos(double filmX, double filmY) const;
	Point3D getFilmUV(double imgX, double imgY) const;
protected:
	
private:
	double horiApect = 0.036, vertApect = 0.024;
	FIT_RESOLUTION_GATE resFT = FRG_HORIZONTAL_FIT;
};

#endif // __FILM__