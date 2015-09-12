#include "Math/MonteCarlo.h"


MonteCarlo::MonteCarlo()
{
}


MonteCarlo::~MonteCarlo()
{
}

Vector3D SampleHemisphere(Float u, Float v)
{
	Float z(u);
	Float r = sqrt(max(0.0, 1.0 - sqr(z)));
	Float phi = 2 * M_PI * v;
	Float x = r * cos(phi);
	Float y = r * sin(phi);
	return Vector3D(x, y, z);
}

