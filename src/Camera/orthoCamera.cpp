#include "orthoCamera.h"

orthoCamera::orthoCamera()
{
	CameraToWorld.setMat(Matrix4x4::lookAt());
	CameraToScreen = Transform(Matrix4x4::orthography());
}

orthoCamera::orthoCamera(const Transform &cam2wo, const Transform &projection)
{
	CameraToWorld = cam2wo;
	CameraToScreen = projection;
}

orthoCamera::~orthoCamera()
{

}