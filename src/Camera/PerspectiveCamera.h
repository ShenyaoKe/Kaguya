#pragma once
#include "Camera/Camera.h"

namespace Kaguya
{

class PerspectiveCamera : public ProjectiveCamera
{
public:
	PerspectiveCamera(const Point3f &eye = Point3f(1, 1, 1),
					  const Point3f &targ = Point3f(),
					  const Vector3f &up = Vector3f(0, 1, 0),
					  Float aspectRatio = 1, Float lensRadius = 0,
					  Float focalDist = sNumInfinity,
					  const Film &film = Film());
	PerspectiveCamera(const Transform &cam2wo, const Transform &projection);
	~PerspectiveCamera();

	//Vector3D getPos() { return pos; }
	//void setResolution(int resX, int resY);
	//void setSample(int aaSample);
	void updateCamToScreen() override;
	Float generateRay(const cameraSampler &sample, Ray* ray) const override;

	void setDoF(Float lr, Float fd);
	void renderImg(int x, int y, ColorRGBA &pixColor);
	void saveResult(const char* filename);
	void resizeViewport(Float aspr = 1.0) override;

};

}
