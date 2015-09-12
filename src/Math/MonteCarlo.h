#pragma once
#ifndef __MONTECARLO__
#define __MONTECARLO__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

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
Vector3D SampleHemisphere(Float u, Float v);

#endif // __MONTECARLO__