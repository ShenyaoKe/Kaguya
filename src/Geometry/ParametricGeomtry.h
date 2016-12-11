#pragma once
#include "Geometry/Shape.h"

class ParametricGeomtry : public Shape
{
public:
    ParametricGeomtry(const Transform* o2w = nullptr,
                      const Transform* w2o = nullptr)
        : Shape(o2w, w2o)
    {
    }
    virtual ~ParametricGeomtry() {}

};
