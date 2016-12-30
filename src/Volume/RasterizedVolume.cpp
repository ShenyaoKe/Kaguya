#include "RasterizedVolume.h"

namespace Kaguya
{

RasterizedVolume::RasterizedVolume(
    const TriangleMesh* src, const KdTreeAccel* tree, Float div)
    : mesh(src), kdtree(tree)
    , division(div)
{
    rasterize();
}

RasterizedVolume::~RasterizedVolume()
{
}

Point3f RasterizedVolume::center() const
{
    Point3f centroid;
    for (auto grid : grids)
    {
        centroid += grid;
    }
    centroid /= static_cast<Float>(grids.size());
    return centroid;
}

std::vector<Point3f> RasterizedVolume::getGrids() const
{
    return grids;
}

void RasterizedVolume::exportVBO(std::vector<float>* vtx_array) const
{
    if (vtx_array == nullptr)
    {
        return;
    }
    vtx_array->reserve(grids.size() * 3);
    for (auto grid : grids)
    {
        vtx_array->push_back(static_cast<float>(grid.x));
        vtx_array->push_back(static_cast<float>(grid.y));
        vtx_array->push_back(static_cast<float>(grid.z));
    }
}

void RasterizedVolume::rasterize()
{
    const Bounds3f &bound = kdtree->treeBound;
    Vector3f diag = bound.diagnal();
    int zaxis = bound.maxExtent();
    int xaxis = (zaxis + 1) % 3;
    int yaxis = (zaxis + 2) % 3;

    Point3f rayPos = bound.pMin;
    Float initZ = rayPos[zaxis] - division;
    Float initY = rayPos[yaxis] + 0.01;
    rayPos[zaxis] = initZ;
    rayPos[yaxis] = initY;
    rayPos[xaxis] += division * 0.01;
    Vector3f rayDir;
    rayDir[zaxis] = 1.0;


    DifferentialGeometry queryPoint;
    Float tHit = INFINITY, rayEp;
    //vector<Point3f> grid;
    Ray rasterRay;
    while (rayPos[xaxis] < bound.pMax[xaxis])
    {
        while (rayPos[yaxis] < bound.pMax[yaxis])
        {
            std::vector<Float> hitLen;
            rasterRay = Ray(rayPos, rayDir);
            while (kdtree->intersect(rasterRay, &queryPoint, &tHit, &rayEp))
            {
                hitLen.push_back(tHit);
                rasterRay.o[zaxis] += tHit + rayEp;
                tHit = INFINITY;
            };
            if (hitLen.size() > 1)
            {
                Point3f curPos = rayPos;
                //curPos[zaxis] += hitLen[0];
                for (int i = 0; i < hitLen.size() - 1; i += 2)
                {
                    curPos[zaxis] += hitLen[i];
                    Float newDP = hitLen[i + 1] + curPos[zaxis];
                    while (curPos[zaxis] < newDP)
                    {
                        grids.push_back(curPos);
                        curPos[zaxis] += division;
                    }
                }
            }
            rayPos[yaxis] += division;
            rayPos[zaxis] = initZ;
        }
        rayPos[xaxis] += division;
        rayPos[yaxis] = initY;
        rayPos[zaxis] = initZ;
    }
    std::cout << "Grid count: " << grids.size() << std::endl;
}

}
