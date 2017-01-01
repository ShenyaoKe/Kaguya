#pragma once
#include "Core/Kaguya.h"
#include "Math/Geometry.h"
#include "Geometry/PrimitiveAttribute.h"

namespace Kaguya
{

class ObjLoader
{

    ObjLoader(const std::string &filename);
    ~ObjLoader();

    enum index_t : uint8_t
    {
        V = 1 << 0,
        UV = 1 << 1,
        NORM = 1 << 2,
        VT = V | UV,
        VN = V | NORM,
        VTN = V | UV | NORM
    };

    index_t facetype(const char* str, int32_t* val);

    void parse(std::FILE* fp);
    void parseFace(std::FILE* fp);

    void swapBuffers();
    void clearMeshBuffers();

    Primitive* finalizeMesh();
public:
    static Primitive* load(const std::string &filename);

private:
    std::FILE*                              fp;
    std::vector<Point3f>                    vertexBuffer;
    std::vector<Point3f>                    vertexBufferFinal;
    std::vector<Point2f>                    uvBuffer;
    std::vector<Point2f>                    uvBufferFinal;
    std::vector<Normal3f>                   normBuffer;
    std::vector<Normal3f>                   normBufferFinal;
    // Index Buffers
    std::vector<uint32_t>                   faceIndexBuffer;
    std::vector<uint32_t>                   uvIndexBuffer;
    std::vector<uint32_t>                   normIndexBuffer;
    // Face Sides
    std::vector<uint32_t>                   faceSizeBuffer;
    // Size & Offset
    size_t                                  vertCount;
    size_t                                  vertOffest;
    size_t                                  uvCount;
    size_t                                  uvOffest;
    size_t                                  normCount;
    size_t                                  normOffest;

    // Attribute
    std::shared_ptr<TextureAttribute>       texAttr;
    std::shared_ptr<NormalAttribute>        normAttr;

    bool                                    hasObject;
    bool                                    hasGroup;
    bool                                    hasMaterial;
    
    std::vector<std::shared_ptr<Primitive>> mPrims;

    std::string                             mFilename;
};

}

