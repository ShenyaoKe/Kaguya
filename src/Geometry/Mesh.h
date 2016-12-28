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

class Mesh : public Primitive
{
public:
    Mesh();
    virtual ~Mesh() = 0;
private:
};

struct PolyIndex
{
    PolyIndex(size_t sz = 3) : size(sz)
    {
        v.reserve(sz);
        uv.reserve(sz);
        n.reserve(sz);
    }
    void push_back(uint32_t* ids)
    {
        v.push_back(ids[0]);
        if (ids[1] > 0) uv.push_back(ids[1]);
        if (ids[2] > 0) n.push_back(ids[2]);
    }
    void printInfo(const std::string &msg = "") const
    {
        if (!msg.empty())
        {
            std::cout << msg << std::endl;
        }
        for (int i = 0; i < size; i++)
        {
            std::cout << v[i] << "/" << uv[i] << "/" << n[i] << "\t";
        }
        std::cout << std::endl;
    }

    size_t size;
    ui32s_t v, uv, n;
};

enum class MeshType
{
    UNKNOWN,
    POLYGONAL_MESH,
    SUBDIVISION_MESH,
};

Mesh* createMesh(const std::string &filename,
                 MeshType meshType = MeshType::UNKNOWN);

namespace objFileParser
{
bool parse(const char*          filename,
           vector<Point3f>     &verts,
           vector<Point2f>     &uvs,
           vector<Normal3f>    &norms,
           vector<PolyIndex>   &polys);
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
