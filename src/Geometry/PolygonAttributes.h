#pragma once
#include "Geometry/Mesh.h"

namespace Kaguya
{

enum class AttributeRate : uint8_t
{
    CONSTANT,// all vertex the same
    UNIFORM,// each face has the same 
    VERTEX_VARYING,// varies in each vertex
    FACE_VARYING,// varies in each face
};

template <typename T>
class PolygonAttribute
{
public:
    // Constant & Uniform
    PolygonAttribute(AttributeRate attriType = AttributeRate::UNIFORM)
        : mType(attriType) {}
    // Vertex Varying
    PolygonAttribute(std::vector<T> attriVals)
        : mValueBuffer(std::move(attriVals))
        , mType(AttributeRate::VERTEX_VARYING) {}
    // Face Varying
    PolygonAttribute(std::vector<T>        attriVals,
                     std::vector<uint32_t> attriIndices)
        : mValueBuffer(std::move(attriVals))
        , mIndexBuffer(std::move(attriIndices))
        , mType(AttributeRate::FACE_VARYING) {}
    ~PolygonAttribute() {}

    bool isConstant() const
    {
        return mType == AttributeRate::CONSTANT;
    }
    bool isUniform() const
    {
        return mType == AttributeRate::UNIFORM;
    }
    bool isVertexVarying() const
    {
        return mType == AttributeRate::VERTEX_VARYING;
    }
    bool isFaceVarying() const
    {
        return mType == AttributeRate::FACE_VARYING;
    }

    void getVertexVarying(uint32_t primID, size_t primSize,
                          uint32_t* vIDs, T* targ) const
    {
        for (uint32_t i = 0; i < primSize; i++)
        {
            targ[i] = mValueBuffer[vIDs[i]];
        }
    }
    void getFaceVarying(uint32_t primID, size_t primSize,
                        T* targ) const
    {
        uint32_t id = primID * primSize;
        for (uint32_t i = 0; i < primSize; i++)
        {
            targ[i] = mValueBuffer[mIndexBuffer[id + i]];
        }
    }

    std::vector<T>        mValueBuffer;
    std::vector<uint32_t> mIndexBuffer;
    AttributeRate         mType;
};

using TextureAttribute = PolygonAttribute<Point2f>;
using NormalAttribute = PolygonAttribute<Normal3f>;

}
