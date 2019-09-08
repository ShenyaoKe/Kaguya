#pragma once
#include "Camera/PerspectiveCamera.h"

namespace Kaguya {

class AklemanCamera : public PerspectiveCamera
{
public:
	AklemanCamera();
	AklemanCamera(const Point3f &eyePos, const Vector3f &viewDir, const Vector3f &upVec,
		Texture* posImg = nullptr, Texture* dirImg = nullptr, Float tp = 0, Float td = 0,
		Float lr = 0, Float fd = std::numeric_limits<Float>::infinity());
	~AklemanCamera();
	void setImage(Texture* posImg = nullptr, Texture* dirImg = nullptr);
	void setAbstraction(Float tp = 0, Float td = 0);
	//Ray generateRay(Float imgX, Float imgY) const;

protected:
    const Texture* posTex;
    const Texture* dirTex;
	Float tpos, tdir;
};

}
