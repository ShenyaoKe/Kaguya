//
//  Camera.h
//
//  Created by Shenyao Ke on 1/29/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __Camera__
#define __Camera__
#include "Math/CGVector.h"
#include "Tracer/Ray.h"
#include "Camera/Film.h"
#include "Tracer/renderBuffer.h"

class baseCamera
{
public:
	baseCamera(){};
	baseCamera(Vector3D& eyePos, Vector3D& viewDir, Vector3D& upVec);
	virtual~baseCamera(){};

	virtual void setResolution(int resX, int resY);
	virtual void setSample(int aaSample);
	virtual void setFocLen(double fl);
	virtual void setFilmType(FILM_TYPE filmType);
	virtual void lookAt(const Vector3D& targPos);
	virtual void lookAt(const Vector3D& camPos, const Vector3D& targPos, const Vector3D& upDir);
	virtual void setBuffer(int x, int y, const bufferData tmpBuff);
	virtual Ray shootRay(double imgX, double imgY) const = 0;

	virtual bufferData getBufferData(int x, int y) const;
protected:
	Point3D pos;
	Vector3D nx, ny, nz;
	Film film;//contains image size, film size(horApec, verApec)
	renderBuffer buffer;
	double focLen = 0.035;//focal length
	int sample = 1;
private:
};

class perspCamera:public baseCamera
{
public:
	perspCamera();
	perspCamera(const Point3D& eyePos, const Vector3D& viewDir, const Vector3D& upVec,
		double lr = 0, double fd = INFINITY);
	~perspCamera();
	//Vector3D getPos() { return pos; }
	//void setResolution(int resX, int resY);
	//void setSample(int aaSample);
	void setDoF(double lr, double fd);
	Ray shootRay(double imgX, double imgY) const;
	void renderImg(int x, int y, ColorRGBA& pixColor);
	void saveResult(const char* filename);
	//void setUpVec(Vector3D& upVec);
protected:
	double lensRadius, focalDistance;
private:
};

class abstractCamera : public perspCamera
{
public:
	abstractCamera();
	abstractCamera(const Point3D& eyePos, const Vector3D& viewDir, const Vector3D& upVec,
		Texture *posImg = nullptr, Texture *dirImg = nullptr, double tp = 0, double td = 0,
		double lr = 0, double fd = INFINITY);
	~abstractCamera();
	void setImage(Texture *posImg = nullptr, Texture *dirImg = nullptr);
	void setAbstraction(double tp = 0, double td = 0);
	Ray shootRay(double imgX, double imgY) const;

protected:
	Texture *posTex, *dirTex;
	double tpos, tdir;
};
#endif