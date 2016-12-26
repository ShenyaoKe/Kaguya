#include "PolyMesh.h"

PolyMesh::PolyMesh(vector<Point3f>  &inVerts,
                   vector<uint32_t> &inIndices)
    : verts(std::move(inVerts))
    , vids(std::move(inIndices))
{
}

PolyMesh::~PolyMesh()
{
}