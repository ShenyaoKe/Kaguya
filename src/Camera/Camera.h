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

	virtual Float generateRay(const CameraSample &sample, Ray* ray) const = 0;

	void setFilm(const Film &film);
	Film& getFilm() { return mFilm; }
	const Film& getFilm() const { return mFilm; }

	virtual void updateRasterToCam() = 0;
	virtual void updateCamToScreen() = 0;
	virtual void updateRasterToScreen() = 0;
	void updateTransform();

protected:
	Film mFilm;//contains image size, film size(horApec, verApec)

};

}
