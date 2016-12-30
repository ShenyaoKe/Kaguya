#include "Geometry/PolyMesh.h"
#include "Tracer/Ray.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/Shader.h"
#include "Shading/TextureMapping.h"
#include "Shading/Texture.h"
#include "QuadMesh.h"

namespace Kaguya
{

QuadMesh::QuadMesh(std::vector<Point3f>  &vertexBuffer,
                   std::vector<uint32_t> &indexBuffer,
                   std::vector<uint32_t> &faceSizeBuffer,
                   size_t                 totalPrimCount,
                   TextureAttribute*      texAttri,
                   NormalAttribute*       normAttri,
                   bool                   isTessellated)
    : PolyMesh(vertexBuffer, indexBuffer,
               vertexBuffer.size(), totalPrimCount,
               texAttri, normAttri)
{
    if (!isTessellated)
    {
        tessellate(mIndexBuffer, faceSizeBuffer, totalPrimCount);
        if (mTextureAttribute->isFaceVarying())
        {
            tessellate(mTextureAttribute->mIndexBuffer,
                       faceSizeBuffer,
                       totalPrimCount);
        }
        if (mNormalAttibute->isFaceVarying())
        {
            tessellate(mNormalAttibute->mIndexBuffer,
                       faceSizeBuffer,
                       totalPrimCount);
        }
    }
}

QuadMesh::~QuadMesh()
{
}

void QuadMesh::bounding()
{
    for (auto &v : mVertexBuffer)
    {
        mObjBound.Union(v);
    }
}

void QuadMesh::printInfo(const std::string &msg) const
{
    if (!msg.empty())
    {
        std::cout << msg << std::endl;
    }
    for (int i = 0; i < mVertexBuffer.size(); i++)
    {
        std::cout << "Vertex:\t" << mVertexBuffer[i] << std::endl;
    }
    for (int i = 0; i < mIndexBuffer.size(); i++)
    {
        /*std::cout << "Faces:\t";
        vids[i].printInfo();*/
    }
}

bool QuadMesh::intersect(const Ray &inRay, DifferentialGeometry* dg,
                         Float* tHit, Float* rayEpsilon) const
{
    return false;
}

void QuadMesh::postIntersect(const Ray &inRay, DifferentialGeometry* dg) const
{
    // TODO: Implement post-intersection method
}

void QuadMesh::getTessellated(TessBuffer &trait) const
{
    // TODO: Implementation check required
    // Setup time step
    size_t timestep = 1;
    trait.nTimeStep = timestep;

    // Setup first vertex buffer
    trait.nVertices = mVertexBuffer.size();
    trait.vertTraits.resize(timestep);
    trait.vertTraits[0].byteOffset = 0;
    trait.vertTraits[0].byteStride = sizeof(Point3f);
    trait.vertTraits[0].data = (void*)(mVertexBuffer.data());

    // Setup buffers for Motion Blur
    for (size_t i = 1; i < timestep; i++)
    {
        trait.vertTraits[i].byteOffset = 0;
        trait.vertTraits[i].byteStride = sizeof(Point3f);
        trait.vertTraits[i].data = (void*)(mVertexBuffer.data());
    }

    // Setup index buffer
    trait.nPrimtives = mIndexBuffer.size() / sQuadFaceSize;
    trait.indexTrait.byteOffset = 0;
    trait.indexTrait.byteStride = sizeof(uint32_t) * sQuadFaceSize;
    trait.indexTrait.data = (void*)(mIndexBuffer.data());
}

void QuadMesh::getBufferObject(BufferTrait* vertTraits,
                               BufferTrait* vidTraits) const
{
    if (vertTraits)
    {
        vertTraits->data = (void*)(mVertexBuffer.data());
        vertTraits->count = mVertexBuffer.size();
        vertTraits->size = sizeof(Point3f) * vertTraits->count;
        vertTraits->offset = 0;
        vertTraits->stride = sizeof(Point3f);
    }
    if (vidTraits)
    {
        vidTraits->data = (void*)(mIndexBuffer.data());
        vidTraits->count = mIndexBuffer.size();
        vidTraits->size = sizeof(uint32_t) * vidTraits->count;
        vidTraits->offset = 0;
        vidTraits->stride = sizeof(uint32_t);
    }
}

void QuadMesh::tessellate(std::vector<uint32_t> &indexBuffer,
                          std::vector<uint32_t> &faceSizeBuffer,
                          size_t                 tessellatedCount)
{
    size_t nCurrentPosition = indexBuffer.size();
    size_t nLastPosition = tessellatedCount * sQuadFaceSize;
    indexBuffer.resize(nLastPosition);

    // TODO: take care of Embree Quad indexing order
    for (int i = faceSizeBuffer.size() - 1; i >= 0; i--)
    {
        uint32_t curFaceSize = faceSizeBuffer[i];
        uint32_t lastIndex = indexBuffer[nCurrentPosition - 1];
        nCurrentPosition -= curFaceSize;
        if (curFaceSize == 3)
        {
            indexBuffer[--nLastPosition] = lastIndex;
            indexBuffer[--nLastPosition] = indexBuffer[nCurrentPosition + 2];
            indexBuffer[--nLastPosition] = indexBuffer[nCurrentPosition + 1];
            indexBuffer[--nLastPosition] = indexBuffer[nCurrentPosition];
        } 
        else
        {
            for (int j = curFaceSize; j > 0; j-=2)
            {
                indexBuffer[--nLastPosition] = indexBuffer[nCurrentPosition + j - 1];
                indexBuffer[--nLastPosition] = indexBuffer[nCurrentPosition + j - 2];
                indexBuffer[--nLastPosition] = indexBuffer[nCurrentPosition + j - 3];
                indexBuffer[--nLastPosition] = lastIndex;
            }
        }
        
    }
}

}
