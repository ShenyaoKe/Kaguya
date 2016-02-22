//
//  film.h
//
//  Created by Shenyao Ke on 1/29/15.
//  Copyright (c) 2014 AKIKA. All rights reserved.
//
#pragma once
#ifndef __FILM__
#define __FILM__
//#include "Core/rtdef.h"
#include "Math/Transform.h"

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

class Film//:public ImageData
{
public:
	Film(FILM_TYPE filmType = FT_16MM_THEATRICAL,
		int resX = default_resX, int resY = default_resY,
		FIT_RESOLUTION_GATE fitType = FRG_HORIZONTAL_FIT);
	~Film();

	void setFilmType(FILM_TYPE filmType);
	void setFitType(FIT_RESOLUTION_GATE fitType);
	
	Float ApectureRatio() const{ return horiApect / vertApect; }
	Point2D getFilmPos(Float imgX, Float imgY) const;// from -1 * len to +1 * len
	Point2D getImgPos(Float filmX, Float filmY) const;
	Point3D getFilmUV(Float imgX, Float imgY) const;
	Matrix4D rasterToFilm() const;

public:
	Float horiApect = 36, vertApect = 24;//mm
	int width, height;// width, height from image class
	Transform RasterToFilm, FilmToScreen;

	FIT_RESOLUTION_GATE resFT = FRG_HORIZONTAL_FIT;
};

#endif // __FILM__