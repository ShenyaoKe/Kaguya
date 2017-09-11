#pragma once

#include "Math/MathUtil.h"
#include "Math/Vector.h"
#include "Math/Matrix3x3.h"

namespace Kaguya
{

class MonteCarlo
{
public:
	MonteCarlo();
	~MonteCarlo();
};
Point3f SampleHemisphere(Float u, Float v);
Point3f SampleSphere(Float u, Float v);
void ConcentricSampleDisk(Float u, Float v, Float &dx, Float &dy);

}
