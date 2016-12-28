#include "Geometry/PolyMesh.h"
#include "Tracer/Ray.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/Shader.h"
#include "Shading/TextureMapping.h"
#include "Shading/Texture.h"
#include "QuadMesh.h"

namespace Kaguya
{

QuadMesh::QuadMesh(vector<Point3f>  &vertexBuffer,
                   vector<uint32_t> &indexBuffer,
                   vector<uint32_t> &faceSizeBuffer,
                   size_t            totalPrimCount,
                   bool              isTessellated)
{
    if (!isTessellated)
    {
        tessellate(indexBuffer, faceSizeBuffer, totalPrimCount);
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
        ObjBound.Union(v);
    }
}

void QuadMesh::refine(vector<Primitive*> &refined)
{
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

void QuadMesh::exportVBO(vector<float>* vtx_array,
                         vector<float>* uv_array,
                         vector<float>* norm_array) const
{
}

void QuadMesh::exportIndexedVBO(vector<float>* vtx_array,
                                vector<float>* uv_array,
                                vector<float>* norm_array,
                                vector<uint32_t>* idx_array) const
{
    bool has_vert(false), has_texcoord(false), has_normal(false), has_uid(false);

    if (vtx_array != nullptr)
    {
        vtx_array->clear();
        vtx_array->reserve(mVertexBuffer.size() * 3);
        has_vert = true;
    }
}

void QuadMesh::tessellate(vector<uint32_t> &indexBuffer,
                          vector<uint32_t> &faceSizeBuffer,
                          size_t            tessellatedCount)
{
    indexBuffer.resize(tessellatedCount);
    // TODO
}

}
