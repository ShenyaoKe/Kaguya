/*!
 * \class PolyMesh
 *
 * \brief 
 *
 * \author Shenyao Ke
 * \date December 2016
 */
#pragma once
#include "Geometry/Mesh.h"
#include "Geometry/PolygonAttributes.h"

namespace Kaguya
{

enum class PolyMeshType
{
    TRIANGLE,
    QUAD
};
struct TessTrait
{
    //size_t      num = 0;
    size_t      byteOffset = 0;
    size_t      byteStride = 0;
    const void* data = nullptr;
};
struct TessBuffer
{
    std::vector<TessTrait> vertTraits;
    TessTrait              indexTrait;
    size_t                 nPrimtives;
    size_t                 nVertices;
    size_t                 nTimeStep;
};

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

    PrimitiveType primitiveType() const override
    {
        return PrimitiveType::POLYGONAL_MESH;
    }

    virtual PolyMeshType polyMeshType() const = 0;

    virtual void getTessellated(TessBuffer &trait) const = 0;

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
};

}
