#include "PolygonAttributes.h"

void PolyAttributes::addAttributeTrait(AttriTraitType traitType,
                                       AttributeTrait* trait)
{
    traits[(uint8_t)traitType] = trait;
}

AttributeTrait::AttributeTrait(AttriType attriType,
                               vector<Float> attriVals,
                               vector<int32_t> attriIndices)
    : mType(attriType)
    , mAttriVals(std::move(attriVals))
    , mAttriIndices(std::move(attriIndices))
{

}

AttributeTrait::~AttributeTrait()
{

}
