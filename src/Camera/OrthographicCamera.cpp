#include "OrthographicCamera.h"

namespace Kaguya
{

OrthographicCamera::OrthographicCamera()
{
	CameraToWorld.setMat(Matrix4x4::lookAt());
	CameraToScreen = Transform(Matrix4x4::orthography());
}

OrthographicCamera::OrthographicCamera(const Transform &cam2wo,
									   const Transform &projection)
{
	CameraToWorld = cam2wo;
	CameraToScreen = projection;
}

OrthographicCamera::~OrthographicCamera()
{
}

void OrthographicCamera::updateCamToScreen()
{
	Float horisize, vertsize;

	if (mViewportRatio < mFilm.getApertureRatio())
	{
		horisize = mFilm.horiApert;
		vertsize = horisize / mViewportRatio;
	}
	else
	{
		vertsize = mFilm.vertApert;
		horisize = vertsize * mViewportRatio;
	}

	CameraToScreen.setMat(Matrix4x4::orthographyFromFilm(
		horisize, vertsize, mScalor, mNearPlane, mFarPlane));
}

Float OrthographicCamera::generateRay(const cameraSampler &sample, Ray* ray) const
{
	// TODO: Add orthographic ray generation
	return 0;
}

void OrthographicCamera::resizeViewport(Float aspr)
{
	mViewportRatio = aspr;
	updateCamToScreen();
	updateRasterToScreen();
}

}
