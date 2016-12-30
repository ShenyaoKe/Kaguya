#pragma once
#include "Geometry/TriangleMesh.h"
#include "Tracer/Ray.h"
#include "Accel/KdTreeAccel.h"

namespace Kaguya
{

class RasterizedVolume
{
public:
    RasterizedVolume(const TriangleMesh* src,
                     const KdTreeAccel* tree, Float div);
    ~RasterizedVolume();

    Point3f center() const;
    std::vector<Point3f> getGrids() const;
    void exportVBO(std::vector<float>* vtx_array = nullptr) const;

private:
    void rasterize();
private:
    const KdTreeAccel*   kdtree;
    const TriangleMesh*  mesh;
    std::vector<Point3f> grids;
    Float                division;
};

}
