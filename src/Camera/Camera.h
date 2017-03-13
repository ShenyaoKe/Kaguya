//
//  Camera.h
//
//  Created by Shenyao Ke on 1/29/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once

#include "Core/Kaguya.h"
#include "Math/Matrix4x4.h"
#include "Math/Transform.h"
#include "Core/Sampler.h"
#include "Tracer/Ray.h"
#include "Camera/Film.h"

namespace Kaguya
{

class Camera
{
public:
    Camera(const Film &film = Film());
    ~Camera();

    void setFilm(const Film &film);
    Film& getFilm() { return mFilm; }
    const Film& getFilm() const { return mFilm; }

    virtual void updateRasterToCam() = 0;
    virtual void updateCamToScreen() = 0;
    virtual void updateRasterToScreen() = 0;
    void updateTransform();

protected:
    Film mFilm;//contains image size, film size(horApec, verApec)

private:

};

class ProjectiveCamera : public Camera
{
public:
    ProjectiveCamera(const Point3f &eye = Point3f(1, 1, 1),
                     const Point3f &targ = Point3f(0, 0, 0),
                     const Vector3f &up = Vector3f(0, 1, 0),
                     Float aspectRatio = 1, Float lensRadius = 0,
                     Float focalDist = sNumInfinity,
                     const Film &film = Film());
    virtual ~ProjectiveCamera() {}

    

    /*void setResolution(int resX, int resY);
    void setSample(int aaSample);
    void setFocLen(Float fl);
    void setFilmType(FILM_TYPE filmType);*/

    //virtual void setBuffer(int x, int y, const bufferData tmpBuff);
    virtual Float generateRay(const cameraSampler &sample, Ray* ray) const = 0;

    //virtual bufferData getBufferData(int x, int y) const;
    Point3f setTarget(const Point3f &targ) { mViewTarget = targ; }
    Point3f getTarget() const { return mViewTarget; }

    void lookAt(const Point3f &eye = Point3f(1, 1, 1),
                const Point3f &targ = Point3f(0, 0, 0),
                const Vector3f &up = Vector3f(0, 1, 0));
    void setProjection(const Matrix4x4 &perspMat);
    void setCamToWorld(const Matrix4x4 &cam2wMat);

    void updateRasterToCam() override;
    void updateCamToScreen() override = 0;
    void updateRasterToScreen() override;

    // Camera Roaming Operation
    void zoom(Float x_val = 0, Float y_val = 0, Float z_val = 0);
    void rotate(Float x_rot = 0, Float y_rot = 0, Float z_rot = 0);
    void rotatePYR(Float pitch = 0, Float yaw = 0, Float roll = 0);
    virtual void resizeViewport(Float aspr = 1.0);

    const Float* cam_to_world() const { return CameraToWorld.getMat().data(); }
    const Float* world_to_cam() const { return CameraToWorld.getInvMat().data(); }
    const Float* cam_to_screen() const { return CameraToScreen.getMat().data(); }
    const Float* screen_to_cam() const { return CameraToScreen.getInvMat().data(); }
    const Float* raster_to_screen() const { return RasterToScreen.getMat().data(); }
    const Float* screen_to_raster() const { return RasterToScreen.getInvMat().data(); }

    Transform CameraToWorld;
    Transform CameraToScreen, RasterToCamera, RasterToScreen;

protected:
    Point3f mViewTarget;

    //renderBuffer buffer;

    Float mViewportRatio;//width / height
    Float mFocalLength;//focal length
    Float mLensRadius;
    Float mFocalDistance;

    Float mNearPlane;
    Float mFarPlane;
    uint8_t mSample = 1;
private:
};

}
