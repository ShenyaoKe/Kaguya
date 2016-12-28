#pragma once
#include "Geometry/TriangleMesh.h"
#include "Tracer/Ray.h"
#include "Accel/KdTreeAccel.h"

namespace Kaguya
{

class RasterizedVolume
{
public:
    RasterizedVolume(const TriangleMesh* src, const KdTreeAccel* tree, Float div);
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

}
