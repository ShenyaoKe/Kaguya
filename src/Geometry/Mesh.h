#pragma once
#include "Geometry/Primitive.h"

namespace Kaguya
{

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
};

Mesh* createMesh(const std::string &filename,
                 MeshType meshType = MeshType::POLYGONAL_MESH);

namespace objFileParser
{

enum index_t : uint8_t
{
    V = 1 << 0,
    UV = 1 << 1,
    NORM = 1 << 2,
    VT = V | UV,
    VN = V | NORM,
    VTN = V | UV | NORM
};

bool parse(const char*            filename,
           std::vector<Point3f>  &verts,
           std::vector<Point2f>  &uvs,
           std::vector<Normal3f> &norms,
           std::vector<uint32_t> &faceId,
           std::vector<uint32_t> &texcoordId,
           std::vector<uint32_t> &normId,
           std::vector<uint32_t> &faceCount);

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
    default:// V
        return V;;//001
    }
}
};
namespace plyFileParser
{

}

}
