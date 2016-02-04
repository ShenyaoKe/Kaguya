#include "orthoCamera.h"

orthoCamera::orthoCamera()
{
	CameraToWorld.setMat(Matrix4D::LookAt());
	CameraToScreen = Transform(Matrix4D::Orthography());
}

orthoCamera::orthoCamera(const Transform& cam2wo, const Transform& projection)
{
	CameraToWorld = cam2wo;
	CameraToScreen = projection;
}

orthoCamera::~orthoCamera()
{

}

Ray orthoCamera::generateRay(Float imgX, Float imgY) const
{
	return Ray();
}
