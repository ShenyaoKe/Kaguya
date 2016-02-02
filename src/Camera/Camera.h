//
//  Camera.h
//
//  Created by Shenyao Ke on 1/29/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __Camera__
#define __Camera__


#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

#include "Core/Kaguya.h"
//#include "Math/CGVector.h"
#include "Math/Matrix4D.h"
#include "Math/Transform.h"
#include "Core/Sampler.h"
#include "Tracer/Ray.h"
#include "Camera/Film.h"
#include "Tracer/renderBuffer.h"

class baseCamera
{
public:
	baseCamera();
	baseCamera(const Vector3D& eyePos, const Vector3D& targetPos, const Vector3D& upVec);
	virtual~baseCamera(){};

	virtual void setResolution(int resX, int resY);
	virtual void setSample(int aaSample);
	virtual void setFocLen(Float fl);
	virtual void setFilmType(FILM_TYPE filmType);
	virtual void updateMatrices();
	//virtual void lookAt(const Vector3D& targPos);
	//virtual void lookAt(const Vector3D& camPos, const Vector3D& targPos, const Vector3D& upDir);
	virtual void setBuffer(int x, int y, const bufferData tmpBuff);
	virtual Ray shootRay(Float imgX, Float imgY) const = 0;
	virtual Float generateRay(const cameraSampler &sample, Ray* ray) const = 0;

	virtual bufferData getBufferData(int x, int y) const;
	virtual Vector3D getTarget() const;

	virtual void updateProjection(const Matrix4D &perspMat);
	virtual void updateCamToWorld(const Matrix4D &cam2wMat);

	// Camera Roaming Operation
	virtual void zoom(Float x_val = 0, Float y_val = 0, Float z_val = 0);
	virtual void rotate(Float x_rot = 0, Float y_rot = 0, Float z_rot = 0);
	virtual void rotatePYR(Float pitch = 0, Float yaw = 0, Float roll = 0);
	virtual void resizeViewport(Float aspr = 1.0);
	virtual void exportVBO(float *view, float *proj, float *raster) const;

	Transform CameraToWorld;
	Transform CameraToScreen, RasterToCamera, RasterToScreen;

protected:
	Point3D pos, target;
	Vector3D nx, ny, nz;

	Film film;//contains image size, film size(horApec, verApec)
	renderBuffer buffer;
	Float focLen = 0.035;//focal length
	int sample = 1;
private:
};

#endif