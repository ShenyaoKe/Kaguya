#pragma once
#include "Geometry/Primitive.h"

namespace Kaguya
{

class Curve : public Primitive
{
public:
    Curve();
    virtual ~Curve();

protected:
    std::vector<Point3f> verts;
    uint8_t              degree;
};

}
