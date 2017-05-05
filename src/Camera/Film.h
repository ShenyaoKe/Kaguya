//
//  film.h
//
//  Created by Shenyao Ke on 1/29/15.
//  Copyright (c) 2014 AKIKA. All rights reserved.
//
#pragma once

#include "Math/Transform.h"

namespace Kaguya
{

using RES_FIT_TYPE = uint8_t;

enum FILM_TYPE
{
	FT_DEFAULT,
	FT_SQUARE,
	FT_16MM_THEATRICAL,
	FT_35MM_ACADEMY,
	FT_IMAX
};
enum FIT_RESOLUTION_GATE
{
	FRG_HORIZONTAL_FIT = 0,
	FRG_VERTICAL_FIT = 1,
	FRG_FILL_FIT = 2,
	FRG_OVERSCAN_FIT = 3
};

class Film//:public ImageData
{
public:
	Film(FILM_TYPE filmType = FT_DEFAULT,
		 int32_t resX = default_resX, int32_t resY = default_resY,
		 FIT_RESOLUTION_GATE fitType = FRG_HORIZONTAL_FIT);
	~Film();

	void setResolution(uint32_t resX, uint32_t resY);
	void setAperture(Float hori, Float vert);
	void setFilmType(FILM_TYPE filmType);
	void setFitType(FIT_RESOLUTION_GATE fitType);

	Float getApertureRatio() const { return horiApert / vertApert; }
	Point2f getFilmPos(Float imgX, Float imgY) const;// from -1 * len to +1 * len
	Point2f getImgPos(Float filmX, Float filmY) const;
	Point2f getFilmUV(Float imgX, Float imgY) const;
	Matrix4x4 rasterToFilm() const;

public:
	Float horiApert, vertApert;//mm
	int width, height;// width, height from image class
	Transform RasterToFilm, FilmToScreen;

	FIT_RESOLUTION_GATE resFT = FRG_HORIZONTAL_FIT;
};

}
