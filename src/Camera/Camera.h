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
#include "Tracer/Ray.h"
#include "Camera/Film.h"
#include "Tracer/renderBuffer.h"

class baseCamera
{
public:
	baseCamera();
	baseCamera(Vector3D& eyePos, Vector3D& target, Vector3D& upVec);
	virtual~baseCamera(){};

	virtual void setResolution(int resX, int resY);
	virtual void setSample(int aaSample);
	virtual void setFocLen(Float fl);
	virtual void setFilmType(FILM_TYPE filmType);
	//virtual void lookAt(const Vector3D& targPos);
	//virtual void lookAt(const Vector3D& camPos, const Vector3D& targPos, const Vector3D& upDir);
	virtual void setBuffer(int x, int y, const bufferData tmpBuff);
	virtual Ray shootRay(Float imgX, Float imgY) const = 0;

	virtual bufferData getBufferData(int x, int y) const;

	virtual void updateProjection(const Matrix4D &perspMat);
	virtual void updateCamToWorld(const Matrix4D &cam2wMat);

	virtual void exportVBO(float *view, float *proj, float *raster) const;

	Transform CameraToWorld;
	Transform CameraToScreen, RasterToScreen;
protected:
	Point3D pos;
	Vector3D nx, ny, nz;

	Film film;//contains image size, film size(horApec, verApec)
	renderBuffer buffer;
	Float focLen = 0.035;//focal length
	int sample = 1;
private:
};


class perspCamera : public baseCamera
{
public:
	perspCamera();
	perspCamera(const Point3D& eyePos, const Vector3D& target, const Vector3D& upVec,
		Float lr = 0, Float fd = INFINITY);
	perspCamera(const Transform& cam2wo, const Transform& projection);
	~perspCamera();
	//Vector3D getPos() { return pos; }
	//void setResolution(int resX, int resY);
	//void setSample(int aaSample);

	void setDoF(Float lr, Float fd);
	Ray shootRay(Float imgX, Float imgY) const;
	void renderImg(int x, int y, ColorRGBA& pixColor);
	void saveResult(const char* filename);

	void zoom(Float x_val = 0, Float y_val = 0, Float z_val = 0);
	void rotate(Float x_rot = 0, Float y_rot = 0, Float z_rot = 0);
	void resizeViewport(Float aspr = 1.0);

	//void setUpVec(Vector3D& upVec);
protected:
	Float fov;
	Float lensRadius, focalDistance;
private:
};

/*
class abstractCamera : public perspCamera
{
public:
	abstractCamera();
	abstractCamera(const Point3D& eyePos, const Vector3D& viewDir, const Vector3D& upVec,
		Texture *posImg = nullptr, Texture *dirImg = nullptr, Float tp = 0, Float td = 0,
		Float lr = 0, Float fd = INFINITY);
	~abstractCamera();
	void setImage(Texture *posImg = nullptr, Texture *dirImg = nullptr);
	void setAbstraction(Float tp = 0, Float td = 0);
	Ray shootRay(Float imgX, Float imgY) const;

protected:
	Texture *posTex, *dirTex;
	Float tpos, tdir;
};*/
#endif