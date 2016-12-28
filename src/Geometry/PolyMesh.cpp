#include "PolyMesh.h"
#include "Geometry/TriangleMesh.h"
#include "Geometry/QuadMesh.h"

namespace Kaguya
{

PolyMesh::PolyMesh(vector<Point3f>   &vertexBuffer,
                   vector<uint32_t>  &indexBuffer,
                   size_t             vertexCount,
                   size_t             faceCount,
                   TextureAttribute*  texAttri,
                   NormalAttribute*   normAttri)
    : mVertexBuffer(std::move(vertexBuffer))
    , mIndexBuffer(std::move(indexBuffer))
    , mVertexCount(vertexCount)
    , mFaceCount(faceCount)
    , mTextureAttribute(texAttri)
    , mNormalAttibute(normAttri)
{

}

PolyMesh::~PolyMesh()
{
}


/*
void PolyMesh::bounding()
{
    mImpl->bounding();
}

bool PolyMesh::intersect(const Ray &inRay,
                         DifferentialGeometry* dg,
                         Float* tHit,
                         Float* rayEpsilon) const
{
    return mImpl->intersect(inRay, dg, tHit, rayEpsilon);
}

void PolyMesh::postIntersect(const Ray &inRay,
                             DifferentialGeometry* dg) const
{
    mImpl->postIntersect(inRay, dg);
}*/

PolyMesh* PolyMesh::createPolyMesh(vector<Point3f>   &vertexBuffer,
                                   vector<uint32_t>  &indexBuffer,
                                   vector<uint32_t>  &faceSizeBuffer,
                                   TextureAttribute*  texAttri,
                                   NormalAttribute*   normAttri)
{
    size_t oriFaceCount = std::accumulate(
        faceSizeBuffer.begin(), faceSizeBuffer.end(), 0);
    size_t triCount = std::accumulate(
        faceSizeBuffer.begin(), faceSizeBuffer.end(), 0,
        [](size_t sum, uint32_t curSize) { return sum + curSize - 2; });
    size_t quadCount = std::accumulate(
        faceSizeBuffer.begin(), faceSizeBuffer.end(), 0,
        [](size_t sum, uint32_t curSize) { return sum + (curSize - 1 >> 1); });

    if (triCount < quadCount)
    {
        return new TriangleMesh(vertexBuffer,
                                indexBuffer,
                                faceSizeBuffer,
                                triCount,
                                oriFaceCount == triCount);
    }
    else
    {
        return new QuadMesh(vertexBuffer,
                            indexBuffer,
                            faceSizeBuffer,
                            triCount,
                            oriFaceCount == quadCount);
    }
}

}
