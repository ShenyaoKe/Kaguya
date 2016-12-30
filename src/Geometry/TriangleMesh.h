#pragma once
#include "Geometry/PolyMesh.h"

namespace Kaguya
{

class TriangleMesh : public PolyMesh
{
public:
    TriangleMesh(std::vector<Point3f>  &vertexBuffer,
                 std::vector<uint32_t> &indexBuffer,
                 std::vector<uint32_t> &faceSizeBuffer,
                 size_t                 totalPrimCount,
                 TextureAttribute*      texAttri,
                 NormalAttribute*       normAttri,
                 bool                   isTessellated = true);
    ~TriangleMesh();

    void bounding() override;

    void printInfo(const std::string &msg) const override;

    bool intersect(const Ray &inRay,
                   DifferentialGeometry* dg,
                   Float* tHit, Float* rayEpsilon) const override;
    void postIntersect(const Ray &inRay,
                       DifferentialGeometry* dg) const override;

    void getTessellated(TessBuffer &trait) const override;

    void getBufferObject(BufferTrait* vertTraits,
                         BufferTrait* vidTraits) const;

    PolyMeshType polyMeshType() const override
    {
        return PolyMeshType::TRIANGLE;
    }

private:
    void tessellate(std::vector<uint32_t> &indexBuffer,
                    std::vector<uint32_t> &faceSizeBuffer,
                    size_t            tessellatedCount) override;

    const static uint32_t sTriFaceSize = 3;
};

namespace TriangleUtils
{
bool intersect(const Point3f &p0,
               const Point3f &p1,
               const Point3f &p2,
               Ray &inRay,
               Float* tHit,
               Float* rayEpsilon);

bool intersectWatertight(const Point3f &p0,
                         const Point3f &p1,
                         const Point3f &p2,
                         Ray &inRay, Float* tHit, Float* rayEpsilon);

void postIntersect(const Point3f &p0,
                   const Point3f &p1,
                   const Point3f &p2,
                   const Ray &inRay, DifferentialGeometry* dg);


}

}
