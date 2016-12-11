#include "OrthographicCamera.h"

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
