#include "Camera/Camera.h"

namespace Kaguya
{

Camera::Camera(const Film &film)
	: mFilm(film)
{
}

Camera::~Camera()
{
}

void Camera::setFilm(const Film &film)
{
	mFilm = film;
	updateTransform();
}

void Camera::updateTransform()
{
	// Raster to camera
	updateRasterToCam();

	// Camera to screen
	updateCamToScreen();

	// Update RasterToScreen
	updateRasterToScreen();
}



}
