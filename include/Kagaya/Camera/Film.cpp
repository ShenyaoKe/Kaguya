#include "Camera/Film.h"

//////////////////////////////////////////////////////////////////////////
Film::Film(FILM_TYPE filmType, int resX, int resY) :ImageData(resX, resY)
{
	setFilmType(filmType);
}
Film::Film(FILM_TYPE filmType, int resX, int resY, FIT_RESOLUTION_GATE fitTyep) : ImageData(resX, resY)
{
	setFilmType(filmType);
	setFitType(fitTyep);
}
Film::~Film()
{

}
void Film::setFilmType(FILM_TYPE filmType)
{
	switch (filmType)
	{
	case FT_SQUARE:
		horiApect = 0.01;
		vertApect = 0.01;
		break;
	case FT_16MM_THEATRICAL:
		horiApect = 0.010262;
		vertApect = 0.007493;
		break;
	case FT_35MM_ACADEMY:
		horiApect = 0.021946;
		vertApect = 0.016002;
		break;
	case FT_IMAX:
		horiApect = 0.070409;
		vertApect = 0.052629;
		break;
	default:
		break;
	}
}
void Film::setFitType(FIT_RESOLUTION_GATE fitType)
{
	double filmRatio = horiApect / vertApect;
	double imgRatio = static_cast<double>(width) / static_cast<double>(height);
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

Point2D Film::getFilmPos(double imgX, double imgY) const
{
	double filmRatio, imgRatio;
	switch (resFT)
	{
	case FRG_HORIZONTAL_FIT:
		//return Vector2D(imgX / width * horiApect, imgY / height + 0.5 * (vertApect - horiApect * height / width));
		return Point2D((imgX / width - 0.5) * horiApect, (imgY - 0.5 * height) * horiApect / width);
	case FRG_VERTICAL_FIT:
		return Point2D(imgX / width + 0.5 * (horiApect - vertApect * width / height), imgY / height * vertApect);
		// 	case FRG_FILL_FIT:
		// 		filmRatio = horiApect / vertApect;
		// 		imgRatio = static_cast<double>(width) / static_cast<double>(height);
		// 		if (imgRatio >= filmRatio)
		// 		{
		// 			return Vector2D((imgX / width - 0.5) * horiApect, (imgY - 0.5 * height) * horiApect / width);
		// 		}
		// 		else
		// 		{
		// 			return Vector2D(imgX / width + 0.5 * (horiApect - vertApect * width / height), imgY / height * vertApect);
		// 		}
		// 	case FRG_OVERSCAN_FIT:
		// 		filmRatio = horiApect / vertApect;
		// 		imgRatio = static_cast<double>(width) / static_cast<double>(height);
		// 		if (imgRatio < filmRatio)
		// 		{
		// 			return Vector2D((imgX / width - 0.5) * horiApect, (imgY - 0.5 * height) * horiApect / width);
		// 		}
		// 		else
		// 		{
		// 			return Vector2D(imgX / width + 0.5 * (horiApect - vertApect * width / height), imgY / height * vertApect);
		// 		}
	default:
		break;
	}
}
Point2D Film::getImgPos(double filmX, double filmY) const//need to change later
{
	double tmpIx, tmpIy;
	switch (resFT)
	{
	case FRG_HORIZONTAL_FIT:
		tmpIx = filmX * width / horiApect;
		tmpIy = (filmY + 0.5 * (horiApect * height / width - vertApect)) * height;
	case FRG_VERTICAL_FIT:
		tmpIx = (filmX + 0.5 * (vertApect * width / height - horiApect)) * width;
		tmpIy = filmY * height / vertApect;
	default:
		break;
	}
	if (tmpIx < 0 || tmpIx >= width || tmpIy < 0 || tmpIy >= height)
	{
		cout << "Pixel out of image boundary" << endl;
		return Point2D(0, 0);
	}
	else
	{
		return Point2D(tmpIx, tmpIy);
	}
}

Point3D Film::getFilmUV(double imgX, double imgY) const
{
	double filmRatio, imgRatio;
	switch (resFT)
	{
	case FRG_HORIZONTAL_FIT:
		//return Vector2D(imgX / width * horiApect, imgY / height + 0.5 * (vertApect - horiApect * height / width));
		return Point3D(imgX / width, imgY / width, 0);
	case FRG_VERTICAL_FIT:
		return Point3D(imgX / height, imgY / height, 0);
	default:
		break;
	}
}
