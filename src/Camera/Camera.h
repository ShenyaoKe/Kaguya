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
//#include "Tracer/renderBuffer.h"

class Camera
{
public:
	//Camera();
	Camera(
		const Point3f &eye = Point3f(1, 1, 1),
		const Point3f &targ = Point3f(0, 0, 0),
		const Vector3f &up = Vector3f(0, 1, 0),
		Float asp = 1, Float lr = 0, Float fd = NUM_INFINITY,
		const Film &fm = Film());
	virtual~Camera(){}

	virtual void setFilm(const Film &f);
	virtual void setResolution(int resX, int resY);
	virtual void setSample(int aaSample);
	virtual void setFocLen(Float fl);
	virtual void setFilmType(FILM_TYPE filmType);
	void updateMatrices();

	//virtual void setBuffer(int x, int y, const bufferData tmpBuff);
	virtual Float generateRay(const cameraSampler &sample, Ray* ray) const = 0;

	//virtual bufferData getBufferData(int x, int y) const;
	virtual Point3f getTarget() const;

	virtual void setProjection(const Matrix4x4 &perspMat);
	virtual void setCamToWorld(const Matrix4x4 &cam2wMat);
	virtual void updateRasterToCam();
	virtual void updateCamToScreen() = 0;
	virtual void updateRasterToScreen();

	// Camera Roaming Operation
	void zoom(Float x_val = 0, Float y_val = 0, Float z_val = 0);
	void rotate(Float x_rot = 0, Float y_rot = 0, Float z_rot = 0);
	void rotatePYR(Float pitch = 0, Float yaw = 0, Float roll = 0);
	virtual void resizeViewport(Float aspr = 1.0);

	const Float* cam_to_world() const { return CameraToWorld.m.data(); }
	const Float* world_to_cam() const { return CameraToWorld.mInv.data(); }
	const Float* cam_to_screen() const { return CameraToScreen.m.data(); }
	const Float* screen_to_cam() const { return CameraToScreen.mInv.data(); }
	const Float* raster_to_screen() const { return RasterToScreen.m.data(); }
	const Float* screen_to_raster() const { return RasterToScreen.mInv.data(); }
	
	Transform CameraToWorld;
	Transform CameraToScreen, RasterToCamera, RasterToScreen;

protected:
	Point3f viewTarget;
	
	Film film;//contains image size, film size(horApec, verApec)
	//renderBuffer buffer;

	Float viewportRatio;//width / height
	Float focLen;//focal length
	Float lensRadius, focalDistance;

	Float nearPlane, farPlane;
	int sample = 1;
private:
};
