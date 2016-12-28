#pragma once
#include "Geometry/Mesh.h"
#include "Geometry/PolygonAttributes.h"

namespace Kaguya
{

class PolyMesh : public Mesh
{
public:
    PolyMesh() {}
    PolyMesh(vector<Point3f>   &vertexBuffer,
             vector<uint32_t>  &indexBuffer,
             size_t             vertexCount,
             size_t             faceCount,
             TextureAttribute*  texAttri,
             NormalAttribute*   normAttri);
    ~PolyMesh();

    /*void bounding() override;
    bool intersect(const Ray &inRay,
                   DifferentialGeometry* dg,
                   Float* tHit,
                   Float* rayEpsilon) const override;
    void postIntersect(const Ray &inRay,
                       DifferentialGeometry* dg) const override;*/

    static PolyMesh* createPolyMesh(vector<Point3f>   &vertexBuffer,
                                    vector<uint32_t>  &indexBuffer,
                                    vector<uint32_t>  &faceSizeBuffer,
                                    TextureAttribute*  texAttri,
                                    NormalAttribute*   normAttri);
protected:
    virtual void tessellate(vector<uint32_t> &indexBuffer,
                            vector<uint32_t> &faceSizeBuffer,
                            size_t            tessellatedCount) = 0;

    vector<Point3f>                   mVertexBuffer;
    vector<uint32_t>                  mIndexBuffer;
    size_t                            mVertexCount;
    size_t                            mFaceCount;

    std::unique_ptr<TextureAttribute> mTextureAttribute;
    std::unique_ptr<NormalAttribute>  mNormalAttibute;

    //std::unique_ptr<Mesh> mImpl;
};

}
