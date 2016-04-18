#pragma once
#include "Camera\Camera.h"
#include "Camera\perspCamera.h"

class abstractCamera : public perspCamera
{
public:
	abstractCamera();
	abstractCamera(const Point3f &eyePos, const Vector3f &viewDir, const Vector3f &upVec,
		Texture *posImg = nullptr, Texture *dirImg = nullptr, Float tp = 0, Float td = 0,
		Float lr = 0, Float fd = INFINITY);
	~abstractCamera();
	void setImage(Texture *posImg = nullptr, Texture *dirImg = nullptr);
	void setAbstraction(Float tp = 0, Float td = 0);
	//Ray generateRay(Float imgX, Float imgY) const;

protected:
	Texture *posTex, *dirTex;
	Float tpos, tdir;
};

