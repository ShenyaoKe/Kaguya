#include "orthoCamera.h"

orthoCamera::orthoCamera()
{
	CameraToWorld = lookAt();
	CameraToScreen = Transform(setOrthographic());
}

orthoCamera::orthoCamera(const Transform& cam2wo, const Transform& projection)
{
	CameraToWorld = cam2wo;
	CameraToScreen = projection;
}

orthoCamera::~orthoCamera()
{

}

Ray orthoCamera::shootRay(Float imgX, Float imgY) const
{
	return Ray();
}
