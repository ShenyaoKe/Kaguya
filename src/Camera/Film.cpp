#include "Camera/Film.h"

//////////////////////////////////////////////////////////////////////////
Film::Film(FILM_TYPE filmType,
	int resX, int resY, FIT_RESOLUTION_GATE fitTyep)
	//: ImageData(resX, resY)
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
	Float filmRatio = horiApect / vertApect;
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

Point2D Film::getFilmPos(Float imgX, Float imgY) const
{
	Float filmRatio, imgRatio;
	switch (resFT)
	{
	case FRG_HORIZONTAL_FIT:
		//return Vector2D(imgX / width * horiApect, imgY / height + 0.5 * (vertApect - horiApect * height / width));
		return Point2D((imgX / width - 0.5) * horiApect, (imgY - 0.5 * height) * horiApect / width);
	case FRG_VERTICAL_FIT:
		return Point2D(imgX / width + 0.5 * (horiApect - vertApect * width / height), imgY / height * vertApect);
	/*
	case FRG_FILL_FIT:
		filmRatio = horiApect / vertApect;
		imgRatio = static_cast<Float>(width) / static_cast<Float>(height);
		if (imgRatio >= filmRatio)
		{
			return Vector2D((imgX / width - 0.5) * horiApect, (imgY - 0.5 * height) * horiApect / width);
		}
		else
		{
			return Vector2D(imgX / width + 0.5 * (horiApect - vertApect * width / height), imgY / height * vertApect);
		}
	case FRG_OVERSCAN_FIT:
		filmRatio = horiApect / vertApect;
		imgRatio = static_cast<Float>(width) / static_cast<Float>(height);
		if (imgRatio < filmRatio)
		{
			return Vector2D((imgX / width - 0.5) * horiApect, (imgY - 0.5 * height) * horiApect / width);
		}
		else
		{
			return Vector2D(imgX / width + 0.5 * (horiApect - vertApect * width / height), imgY / height * vertApect);
		}
	*/
	default:
		return Point2D();
	}
}
Point2D Film::getImgPos(Float filmX, Float filmY) const//need to change later
{
	Float tmpIx, tmpIy;
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

Point3D Film::getFilmUV(Float imgX, Float imgY) const
{
	Float filmRatio, imgRatio;
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
Matrix4D Film::rasterToFilm() const
{
	//Matrix4D ret;
	switch (resFT)
	{
	case FRG_HORIZONTAL_FIT:
	{
		Float m00 = horiApect / static_cast<Float>(width);
		return Matrix4D(
			m00, 0, 0, 0,
			0, m00, 0, 0,
			0, 0, 1, 0,
			-0.5 * horiApect, -0.5 * m00 * height, 0, 1
			);
	}
	case FRG_VERTICAL_FIT:
	{
		Float m00 = vertApect / static_cast<Float>(height);
		return Matrix4D(
			m00, 0, 0, 0,
			0, m00, 0, 0,
			0, 0, 1, 0,
			-0.5 * m00 * width, -0.5 * vertApect, 0, 1
			);
	}
		/*
		case FRG_FILL_FIT:
		filmRatio = horiApect / vertApect;
		imgRatio = static_cast<Float>(width) / static_cast<Float>(height);
		if (imgRatio >= filmRatio)
		{
		return Vector2D((imgX / width - 0.5) * horiApect, (imgY - 0.5 * height) * horiApect / width);
		}
		else
		{
		return Vector2D(imgX / width + 0.5 * (horiApect - vertApect * width / height), imgY / height * vertApect);
		}
		case FRG_OVERSCAN_FIT:
		filmRatio = horiApect / vertApect;
		imgRatio = static_cast<Float>(width) / static_cast<Float>(height);
		if (imgRatio < filmRatio)
		{
		return Vector2D((imgX / width - 0.5) * horiApect, (imgY - 0.5 * height) * horiApect / width);
		}
		else
		{
		return Vector2D(imgX / width + 0.5 * (horiApect - vertApect * width / height), imgY / height * vertApect);
		}
		*/
	default:
		break;
	}
	return Matrix4D();
}
