#pragma once
#ifndef __RASTERIZED_VOLUME__
#define __RASTERIZED_VOLUME__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

#include "Geometry/TriangleMesh.h"
#include "Tracer/Ray.h"
#include "Accel/KdTreeAccel.h"

class RasterizedVolume
{
public:
	RasterizedVolume(const TriangleMesh* src, const KdTreeAccel* tree, double div);
	~RasterizedVolume();

	Point3f center() const;
	vector<Point3f> getGrids() const;
	void exportVBO(vector<float>* vtx_array = nullptr) const;

private:
	void rasterize();
private:
	const KdTreeAccel* kdtree;
	const TriangleMesh* mesh;
	vector<Point3f> grids;
	double division;
};


#endif // __RASTERIZED_VOLUME__