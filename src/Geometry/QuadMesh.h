#pragma once

#include "Geometry/PolyMesh.h"

namespace Kaguya
{

class QuadMesh : public PolyMesh
{
public:
    QuadMesh(vector<Point3f>  &vertexBuffer,
             vector<uint32_t> &indexBuffer,
             vector<uint32_t> &faceSizeBuffer,
             size_t            totalPrimCount,
             bool              isTessellated = true);
    ~QuadMesh();

    void bounding();

    void refine(vector<Primitive*> &refined);
    void printInfo(const std::string &msg = "") const;

    bool intersect(const Ray &inRay,
                   DifferentialGeometry* dg,
                   Float* tHit, Float* rayEpsilon) const override;
    void postIntersect(const Ray &inRay,
                       DifferentialGeometry* dg) const override;

    void getBufferObject(BufferTrait* vertTraits,
                         BufferTrait* vidTraits) const;
    void exportVBO(vector<float>* vtx_array = nullptr,
                   vector<float>* uv_array = nullptr,
                   vector<float>* norm_array = nullptr) const;

    void exportIndexedVBO(vector<float>*    vtx_array = nullptr,
                          vector<float>*    uv_array = nullptr,
                          vector<float>*    norm_array = nullptr,
                          vector<uint32_t>* idx_array = nullptr) const;

    vector<Point3f>  vertices() const { return mVertexBuffer; }
    vector<uint32_t> faceids() const { return mIndexBuffer; }

private:
    void tessellate(vector<uint32_t> &indexBuffer,
                    vector<uint32_t> &faceSizeBuffer,
                    size_t            tessellatedCount) override;

    const static uint32_t sQuadFaceSize = 4;

};

}
