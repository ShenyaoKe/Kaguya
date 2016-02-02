#pragma once
#include "Camera\Camera.h"

class perspCamera : public baseCamera
{
public:
	perspCamera();
	perspCamera(const Point3D& eyePos, const Vector3D& target,
		const Vector3D& upVec, Float lr = 0.0, Float fd = INFINITY);
	perspCamera(const Transform& cam2wo, const Transform& projection);
	~perspCamera();
	//Vector3D getPos() { return pos; }
	//void setResolution(int resX, int resY);
	//void setSample(int aaSample);

	void setDoF(Float lr, Float fd);
	Ray shootRay(Float imgX, Float imgY) const;
	Float generateRay(const cameraSampler &sample, Ray* ray) const;
	void renderImg(int x, int y, ColorRGBA& pixColor);
	void saveResult(const char* filename);

	//void setUpVec(Vector3D& upVec);
protected:
	Float fov;
	Float lensRadius, focalDistance;
private:
};