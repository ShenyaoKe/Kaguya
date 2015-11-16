#pragma once
#ifndef __RASTERIZED_VOLUME__
#define __RASTERIZED_VOLUME__

#include "Geometry/Mesh.h"
#include "Tracer/Ray.h"
#include "Accel/KdTreeAccel.h"

class RasterizedVolume
{
public:
	RasterizedVolume(const Mesh* src, double div);
	~RasterizedVolume();
private:
	KdTreeAccel* kdtree;
	const Mesh* mesh;
	vector<Point3D> pos;
	double division;
};


#endif // __RASTERIZED_VOLUME__