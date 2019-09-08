#pragma once

#include "Tracer/Ray.h"

namespace Kaguya
{

class RayDifferential : public Ray
{
public:

	Point3f mRxOrigin, mRyOrigin;
	Vector3f mRxDirection, mRyDirection;


};

}
