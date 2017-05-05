#pragma once
#include "Camera\Camera.h"

namespace Kaguya
{

class OrthographicCamera : public ProjectiveCamera
{
public:
	OrthographicCamera();
	OrthographicCamera(const Transform &cam2wo, const Transform &projection);
	~OrthographicCamera();

	void updateCamToScreen() override;
	Float generateRay(const cameraSampler &sample, Ray* ray) const override;

	void setScalor(Float scalor) { mScalor = scalor; }
	//void zoom(Float x_val = 0, Float y_val = 0, Float z_val = 0);
	//void rotate(Float x_rot = 0, Float y_rot = 0, Float z_rot = 0);
	void resizeViewport(Float aspr) override;

	//void setUpVec(Vector3D &upVec);
private:
	Float mScalor = 1;
};

}
