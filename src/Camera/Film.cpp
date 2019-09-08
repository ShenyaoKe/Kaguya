#include "Camera/Film.h"

namespace Kaguya
{

//////////////////////////////////////////////////////////////////////////
Film::Film(FILM_TYPE filmType,
		   int32_t resX, int32_t resY, FIT_RESOLUTION_GATE fitTyep)
	: width(resX), height(resY)
	, resFT(fitTyep)
	//: ImageData(resX, resY)
{
	setFilmType(filmType);
	setFitType(fitTyep);
}

Film::~Film()
{
}

void Film::setResolution(uint32_t resX, uint32_t resY)
{
	width = resX;
	height = resY;
}

void Film::setFilmType(FILM_TYPE filmType)
{
	switch (filmType)
	{
	case FT_SQUARE:
		horiApert = 10;
		vertApert = 10;
		break;
	case FT_16MM_THEATRICAL:
		horiApert = 10.262;
		vertApert = 7.493;
		break;
	case FT_35MM_ACADEMY:
		horiApert = 21.946;
		vertApert = 16.002;
		break;
	case FT_IMAX:
		horiApert = 70.409;
		vertApert = 52.629;
		break;
	default:
		horiApert = 36;
		vertApert = 24;
		break;
	}
}

void Film::setFitType(FIT_RESOLUTION_GATE fitType)
{
	Float filmRatio = horiApert / vertApert;
	Float imgRatio = static_cast<Float>(width) / static_cast<Float>(height);
	switch (fitType)
	{
	case FRG_HORIZONTAL_FIT:
	case FRG_VERTICAL_FIT:
		resFT = fitType;
		break;
	case FRG_FILL_FIT:
		if (imgRatio >= filmRatio)
		{
			resFT = FRG_HORIZONTAL_FIT;
		}
		else
		{
			resFT = FRG_VERTICAL_FIT;
		}
		break;
	case FRG_OVERSCAN_FIT:
		if (imgRatio < filmRatio)
		{
			resFT = FRG_HORIZONTAL_FIT;
		}
		else
		{
			resFT = FRG_VERTICAL_FIT;
		}
		break;
	default:
		break;
	}
}

Point2f Film::getFilmPos(Float imgX, Float imgY) const
{
	//Float filmRatio, imgRatio;
	switch (resFT)
	{
	case FRG_HORIZONTAL_FIT:
		//return Vector2D(imgX / width * horiApert, imgY / height + 0.5 * (vertApert - horiApert * height / width));
		return Point2f((imgX / width - 0.5) * horiApert, (imgY - 0.5 * height) * horiApert / width);
	case FRG_VERTICAL_FIT:
		return Point2f(imgX / width + 0.5 * (horiApert - vertApert * width / height), imgY / height * vertApert);
		/*
		case FRG_FILL_FIT:
		filmRatio = horiApert / vertApert;
		imgRatio = static_cast<Float>(width) / static_cast<Float>(height);
		if (imgRatio >= filmRatio)
		{
		return Vector2D((imgX / width - 0.5) * horiApert, (imgY - 0.5 * height) * horiApert / width);
		}
		else
		{
		return Vector2D(imgX / width + 0.5 * (horiApert - vertApert * width / height), imgY / height * vertApert);
		}
		case FRG_OVERSCAN_FIT:
		filmRatio = horiApert / vertApert;
		imgRatio = static_cast<Float>(width) / static_cast<Float>(height);
		if (imgRatio < filmRatio)
		{
		return Vector2D((imgX / width - 0.5) * horiApert, (imgY - 0.5 * height) * horiApert / width);
		}
		else
		{
		return Vector2D(imgX / width + 0.5 * (horiApert - vertApert * width / height), imgY / height * vertApert);
		}
		*/
	default:
		return Point2f();
	}
}

Point2f Film::getImgPos(Float filmX, Float filmY) const//need to change later
{
	Float tmpIx(0), tmpIy(0);
	switch (resFT)
	{
	case FRG_HORIZONTAL_FIT:
		tmpIx = filmX * width / horiApert;
		tmpIy = (filmY + 0.5f * (horiApert * height / width - vertApert)) * height;
		break;
	case FRG_VERTICAL_FIT:
		tmpIx = (filmX + 0.5f * (vertApert * width / height - horiApert)) * width;
		tmpIy = filmY * height / vertApert;
		break;
	default:
		break;
	}
	if (tmpIx < 0 || tmpIx >= width || tmpIy < 0 || tmpIy >= height)
	{
		std::cout << "Pixel out of image boundary" << std::endl;
		return Point2f(0, 0);
	}
	else
	{
		return Point2f(tmpIx, tmpIy);
	}
}

Point2f Film::getFilmUV(Float imgX, Float imgY) const
{
	//Float filmRatio, imgRatio;
	switch (resFT)
	{
	case FRG_HORIZONTAL_FIT:
		//return Vector2D(imgX / width * horiApert, imgY / height + 0.5 * (vertApert - horiApert * height / width));
		return Point2f(imgX / width, imgY / width);
	case FRG_VERTICAL_FIT:
		return Point2f(imgX / height, imgY / height);
	default:
		return Point2f();
	}
}

Matrix4x4 Film::rasterToFilm() const
{
	switch (resFT)
	{
	case FRG_HORIZONTAL_FIT:
	{
		Float m00 = horiApert / static_cast<Float>(width);
		return Matrix4x4(m00, 0, 0, 0,
						 0, m00, 0, 0,
						 0, 0, 1, 0,
						 -0.5 * horiApert, -0.5 * m00 * height, 0, 1);
	}
	case FRG_VERTICAL_FIT:
	{
		Float m00 = vertApert / static_cast<Float>(height);
		return Matrix4x4(m00, 0, 0, 0,
						 0, m00, 0, 0,
						 0, 0, 1, 0,
						 -0.5 * m00 * width, -0.5 * vertApert, 0, 1);
	}
	/*
	case FRG_FILL_FIT:
	filmRatio = horiApert / vertApert;
	imgRatio = static_cast<Float>(width) / static_cast<Float>(height);
	if (imgRatio >= filmRatio)
	{
	return Vector2D((imgX / width - 0.5) * horiApert, (imgY - 0.5 * height) * horiApert / width);
	}
	else
	{
	return Vector2D(imgX / width + 0.5 * (horiApert - vertApert * width / height), imgY / height * vertApert);
	}
	case FRG_OVERSCAN_FIT:
	filmRatio = horiApert / vertApert;
	imgRatio = static_cast<Float>(width) / static_cast<Float>(height);
	if (imgRatio < filmRatio)
	{
	return Vector2D((imgX / width - 0.5) * horiApert, (imgY - 0.5 * height) * horiApert / width);
	}
	else
	{
	return Vector2D(imgX / width + 0.5 * (horiApert - vertApert * width / height), imgY / height * vertApert);
	}
	*/
	default:
		break;
	}
	return Matrix4x4();
}
void Film::setAperture(Float hori, Float vert)
{
	horiApert = hori;
	vertApert = vert;
}
}
