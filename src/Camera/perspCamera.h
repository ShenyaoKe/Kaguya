#pragma once
#include "Camera/Camera.h"

class perspCamera : public Camera
{
public:
	perspCamera(const Vector3D &eye = Vector3D(1, 1, 1),
		const Vector3D &targ = Vector3D(0, 0, 0),
		const Vector3D &up = Vector3D(0, 1, 0),
		Float asp = 1.0, Float lr = 0, Float fd = INFINITY,
		const Film &fm = Film());
	perspCamera(const Transform& cam2wo, const Transform& projection);
	~perspCamera();
	//Vector3D getPos() { return pos; }
	//void setResolution(int resX, int resY);
	//void setSample(int aaSample);
	void updateCamToScreen();
	void setDoF(Float lr, Float fd);
	Float generateRay(const cameraSampler &sample, Ray* ray) const;
	void renderImg(int x, int y, ColorRGBA& pixColor);
	void saveResult(const char* filename);
	void resizeViewport(Float aspr = 1.0);

	//void setUpVec(Vector3D &upVec);
protected:
	//Float fov;
private:
};