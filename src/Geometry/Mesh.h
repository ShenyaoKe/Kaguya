#pragma once
#include "Geometry/Primitive.h"

namespace Kaguya
{

struct BufferTrait
{
    const void* data = nullptr;
    uint32_t    count = 0;
    uint32_t    size = 0;
    uint32_t    offset = 0;
    uint32_t    stride = 0;
};

enum class MeshType
{
    UNKNOWN,
    POLYGONAL_MESH,
    SUBDIVISION_MESH,
};

class Mesh : public Primitive
{
public:
    Mesh();
    virtual ~Mesh() = 0;

private:
};

Mesh* createMesh(const std::string &filename,
                 MeshType meshType = MeshType::POLYGONAL_MESH);

namespace objFileParser
{
bool parse(const char*       filename,
           vector<Point3f>  &verts,
           vector<Point2f>  &uvs,
           vector<Normal3f> &norms,
           vector<uint32_t> &faceId,
           vector<uint32_t> &texcoordId,
           vector<uint32_t> &normId,
           vector<uint32_t> &faceCount);

enum index_t : uint8_t
{
    V = 1 << 0,
    UV = 1 << 1,
    NORM = 1 << 2,
    VT = V | UV,
    VN = V | NORM,
    VTN = V | UV | NORM
};

inline index_t facetype(const char* str, uint32_t* val)
{
    int argv = sscanf(str, "%d/%d/%d", val, val + 1, val + 2);
    switch (argv)
    {
    case 3:// V/T/N
        return VTN;//111
    case 2:// V/T
        return VT;//011
    case 1:
        argv = sscanf(str, "%d//%d", val, val + 2);
        if (argv == 2)// V//N
        {
            return VN;//101
        }
        else// V
        {
            return V;//001
        }
    default:
        return V;
    }
}
};
namespace plyFileParser
{

}

}
