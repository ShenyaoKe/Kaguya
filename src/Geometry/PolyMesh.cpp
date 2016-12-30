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

PolyMesh* PolyMesh::createPolyMesh(vector<Point3f>   &vertexBuffer,
                                   vector<uint32_t>  &indexBuffer,
                                   vector<uint32_t>  &faceSizeBuffer,
                                   TextureAttribute*  texAttri,
                                   NormalAttribute*   normAttri)
{
    uint32_t triCount = std::accumulate(
        faceSizeBuffer.begin(), faceSizeBuffer.end(), 0,
        [](uint32_t sum, uint32_t curSize) { return sum + curSize - 2; });
    uint32_t quadCount = std::accumulate(
        faceSizeBuffer.begin(), faceSizeBuffer.end(), 0,
        [](uint32_t sum, uint32_t curSize) { return sum + ((curSize - 1) >> 1); });

    if (triCount * 3 < quadCount * 4)
    {
        return new TriangleMesh(vertexBuffer,
                                indexBuffer,
                                faceSizeBuffer,
                                triCount,
                                texAttri,
                                normAttri,
                                triCount == faceSizeBuffer.size());
    }
    else
    {
        return new QuadMesh(vertexBuffer,
                            indexBuffer,
                            faceSizeBuffer,
                            quadCount,
                            texAttri,
                            normAttri,
                            quadCount == faceSizeBuffer.size());
    }
}

}
