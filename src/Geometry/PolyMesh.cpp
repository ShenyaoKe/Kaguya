#include "PolyMesh.h"
#include "Geometry/TriangleMesh.h"
#include "Geometry/QuadMesh.h"

namespace Kaguya
{

PolyMesh::PolyMesh(std::vector<Point3f>   &vertexBuffer,
                   std::vector<uint32_t>  &indexBuffer,
                   size_t                  vertexCount,
                   size_t                  faceCount,
                   TextureAttribute*       texAttri,
                   NormalAttribute*        normAttri)
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

void PolyMesh::getRenderBuffer(RenderBufferTrait* trait) const
{
    switch (polyMeshType())
    {
    case PolyMeshType::TRIANGLE:
        trait->renderType = GPURenderType::TRIANGLE;
        break;
    case PolyMeshType::QUAD:
        trait->renderType = GPURenderType::QUAD;
        break;
    default:
        break;
    }
    // attach vertex buffer
    trait->vertex.data = (void*)(mVertexBuffer.data());
    trait->vertex.count = mVertexBuffer.size();
    trait->vertex.size = sizeof(Point3f) * trait->vertex.count;
    trait->vertex.offset = 0;
    trait->vertex.stride = sizeof(Point3f);

    // attach index buffer
    trait->index.data = (void*)(mIndexBuffer.data());
    trait->index.count = mIndexBuffer.size();
    trait->index.size = sizeof(uint32_t) * trait->index.count;
    trait->index.offset = 0;
    trait->index.stride = sizeof(uint32_t);

    // Extract TextureCoordinates to Texture Buffer
    switch (mTextureAttribute->mType)
    {
    case AttributeType::VERTEX_VARYING:
    {
        trait->texAttriType = AttributeType::VERTEX_VARYING;
        trait->texcoords.attriData = mTextureAttribute->getValuePtr();
        trait->texcoords.dataSize = mTextureAttribute->getValueByteSize();
        break;
    }
    case AttributeType::FACE_VARYING:
    {
        trait->texAttriType = AttributeType::FACE_VARYING;
        trait->texcoords.attriData = mTextureAttribute->getValuePtr();
        trait->texcoords.dataSize = mTextureAttribute->getValueByteSize();
        trait->texcoords.attriIndex = mTextureAttribute->getIndexPtr();
        trait->texcoords.indexSize = mTextureAttribute->getIndexByteSize();
        break;
    }
    default:
        break;
    }
    // Extract Shading Normal to Texture Buffer
    switch (mNormalAttibute->mType)
    {
    case AttributeType::VERTEX_VARYING:
    {
        trait->normAttriType = AttributeType::VERTEX_VARYING;
        trait->normal.attriData = mNormalAttibute->getValuePtr();
        trait->normal.dataSize = mNormalAttibute->getValueByteSize();
        break;
    }
    case AttributeType::FACE_VARYING:
    {
        trait->normAttriType = AttributeType::FACE_VARYING;
        trait->normal.attriData = mNormalAttibute->getValuePtr();
        trait->normal.dataSize = mNormalAttibute->getValueByteSize();
        trait->normal.attriIndex = mNormalAttibute->getIndexPtr();
        trait->normal.indexSize = mNormalAttibute->getIndexByteSize();
        break;
    }
    default:
        break;
    }
}

PolyMesh* PolyMesh::createPolyMesh(std::vector<Point3f>   &vertexBuffer,
                                   std::vector<uint32_t>  &indexBuffer,
                                   std::vector<uint32_t>  &faceSizeBuffer,
                                   TextureAttribute*       texAttri,
                                   NormalAttribute*        normAttri)
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
