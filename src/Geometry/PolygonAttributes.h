#pragma once
#include "Geometry/Mesh.h"

enum class AttriType : uint8_t
{
    CONSTANT,// all vertex the same
    UNIFORM,// each face has the same 
    VERTEX_VARYING,// varies in each vertex
    FACE_VARYING,// varies in each face
};

enum class AttriTraitType : uint8_t
{
    TEXTURE_COORDS = 0,
    SHADING_NORMAL = 1,
};

class AttributeTrait
{
public:
    AttributeTrait(AttriType attriType          = AttriType::CONSTANT,
                   vector<Float> attriVals      = vector<Float>(),
                   vector<int32_t> attriIndices = vector<int32_t>());
	~AttributeTrait();
protected:

    AttriType mType;
    vector<Float>   mAttriVals;
    vector<int32_t> mAttriIndices;
private:
};

class PolyAttributes
{
public:
    PolyAttributes() : traits{} {}
	~PolyAttributes() {}

    void addAttributeTrait(AttriTraitType traitType,
                           AttributeTrait* trait);
    /*void getConstTexCoord(Point2f &targ, int32_t primID);
    void getUniformAttribute(int32_t primID, void* targ);*/

    static const int sTraitsCount = 2;
    AttributeTrait* traits[sTraitsCount];
};


