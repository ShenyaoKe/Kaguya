#pragma once
#include "Geometry/PolyMesh.h"

namespace Kaguya
{

class TriangleMesh : public PolyMesh
{
public:
    friend class Triangle;
    TriangleMesh(vector<Point3f>  &vertexBuffer,
                 vector<uint32_t> &indexBuffer,
                 vector<uint32_t> &faceSizeBuffer,
                 size_t            totalPrimCount,
                 bool              isTessellated = true);
    ~TriangleMesh();

    void bounding();

    void refine(vector<Primitive*> &refined);

    void printInfo(const std::string &msg = "") const;

    bool intersect(const Ray &inRay,
                   DifferentialGeometry* dg,
                   Float* tHit, Float* rayEpsilon) const;
    void postIntersect(const Ray &inRay, DifferentialGeometry* dg) const;

    void getBufferObject(BufferTrait* vertTraits,
                         BufferTrait* vidTraits) const;
    void exportVBO(vector<float>* vtx_array = nullptr,
                   vector<float>* uv_array = nullptr,
                   vector<float>* norm_array = nullptr) const;

    void exportIndexedVBO(vector<float>*    vtx_array = nullptr,
                          vector<float>*    uv_array = nullptr,
                          vector<float>*    norm_array = nullptr,
                          vector<uint32_t>* idx_array = nullptr) const;

    /*vector<Point3f>     vertices() const { return mVertexBuffer; }
    vector<Point2f>     uvcoords() const { return uvs; }
    vector<Normal3f>    normals() const { return norms; }
    vector<PolyIndex>   faceids() const { return fids; }*/
private:
    void tessellate(vector<uint32_t> &indexBuffer,
                    vector<uint32_t> &faceSizeBuffer,
                    size_t            tessellatedCount) override;

    const static uint32_t sTriFaceSize = 3;
protected:
    //vector<Point2f>     uvs;
    //vector<Normal3f>    norms;
    //vector<PolyIndex>   fids;
};

class Triangle : public Primitive
{
public:
    Triangle();
    Triangle(TriangleMesh* inMesh, size_t n);

    void bounding();
    void attachMesh(const TriangleMesh* inMesh);
    void setPoint(Point3f* p0, Point3f* p1, Point3f* p2);
    void setUV(Point2f* uv0, Point2f* uv1, Point2f* uv2);
    void setNormal(Normal3f* n0, Normal3f* v1, Normal3f* v2);

    bool intersect(const Ray &inRay,
                   DifferentialGeometry* dg,
                   Float* tHit, Float* rayEpsilon) const;
    bool intersectWatertight(const Ray &inRay,
                             DifferentialGeometry* dg,
                             Float* tHit, Float* rayEpsilon) const;
    void postIntersect(const Ray &inRay,
                       DifferentialGeometry* dg) const;
    void getNormal(DifferentialGeometry* queryPoint) const;

protected:
    const TriangleMesh* mesh;
    std::array<Point3f*, 3> p;
    std::array<Point2f*, 3> uv;
    std::array<Normal3f*, 3> n;
    friend class TriangleMesh;
};

}
