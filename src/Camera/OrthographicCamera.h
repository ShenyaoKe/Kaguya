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
    //Vector3D getPos() { return pos; }
    //void setResolution(int resX, int resY);
    //void setSample(int aaSample);

    //Ray generateRay(Float imgX, Float imgY) const;

    //void zoom(Float x_val = 0, Float y_val = 0, Float z_val = 0);
    //void rotate(Float x_rot = 0, Float y_rot = 0, Float z_rot = 0);
    //void resizeViewport(Float aspr = 1.0);

    //void setUpVec(Vector3D &upVec);
protected:
    Float nearPlane = -1.0, farPlane = 1.0;
};

}
