#pragma once
#include "Geometry/Shape.h"

class Curve : public Shape
{
public:
	Curve();
	virtual ~Curve();

protected:
    vector<Point3f> verts;
    uint8_t         degree;
};