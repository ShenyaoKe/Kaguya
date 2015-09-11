#pragma once
#ifndef __MONTECARLO__
#define __MONTECARLO__

#include "Math/MathUtil.h"
#include "Core/rtdef.h"
#include "Math/CGVector.h"
#include "Math/Matrix3D.h"

class MonteCarlo
{
public:
	MonteCarlo();
	~MonteCarlo();
};
Vector3D SampleHemisphere(double u, double v);

#endif // __MONTECARLO__