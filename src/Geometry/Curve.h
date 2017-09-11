#pragma once
#include "Geometry/Geometry.h"

namespace Kaguya
{

class Curve : public Geometry
{
public:
	Curve();
	virtual ~Curve();

protected:
	std::vector<Point3f> verts;
	uint8_t              degree;
};

}
