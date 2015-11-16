#pragma once
#ifndef __RASTERIZED_VOLUME__
#define __RASTERIZED_VOLUME__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

#include "Geometry/Mesh.h"
#include "Tracer/Ray.h"
#include "Accel/KdTreeAccel.h"

class RasterizedVolume
{
public:
	RasterizedVolume(const Mesh* src, const KdTreeAccel* tree, double div);
	~RasterizedVolume();

	Point3D center() const;
	void exportVBO(vector<float>* vtx_array = nullptr) const;

private:
	void rasterize();
private:
	const KdTreeAccel* kdtree;
	const Mesh* mesh;
	vector<Point3D> grids;
	double division;
};


#endif // __RASTERIZED_VOLUME__