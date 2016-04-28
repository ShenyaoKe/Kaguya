//
//  film.h
//
//  Created by Shenyao Ke on 1/29/15.
//  Copyright (c) 2014 AKIKA. All rights reserved.
//
#pragma once
#ifndef __FILM__
#define __FILM__

#include "Math/Transform.h"

typedef unsigned char RES_FIT_TYPE;

enum FILM_TYPE
{
	FT_DEFAULT,
	FT_SQUARE,
	FT_16MM_THEATRICAL,
	FT_35MM_ACADEMY,
	FT_IMAX
};
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
	Film(FILM_TYPE filmType = FT_DEFAULT,
		int resX = default_resX, int resY = default_resY,
		FIT_RESOLUTION_GATE fitType = FRG_HORIZONTAL_FIT);
	~Film();

	void setFilmType(FILM_TYPE filmType);
	void setFitType(FIT_RESOLUTION_GATE fitType);
	
	Float ApectureRatio() const{ return horiApect / vertApect; }
	Point2f getFilmPos(Float imgX, Float imgY) const;// from -1 * len to +1 * len
	Point2f getImgPos(Float filmX, Float filmY) const;
	Point3f getFilmUV(Float imgX, Float imgY) const;
	Matrix4x4 rasterToFilm() const;

public:
	Float horiApect, vertApect;//mm
	int width, height;// width, height from image class
	Transform RasterToFilm, FilmToScreen;

	FIT_RESOLUTION_GATE resFT = FRG_HORIZONTAL_FIT;
};

#endif // __FILM__