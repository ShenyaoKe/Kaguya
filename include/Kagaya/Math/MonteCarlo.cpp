#include "Math/MonteCarlo.h"


MonteCarlo::MonteCarlo()
{
}


MonteCarlo::~MonteCarlo()
{
}

Vector3D SampleHemisphere(double u, double v)
{
	double z(u);
	double r = sqrt(max(0.0, 1.0 - sqr(z)));
	double phi = 2 * M_PI * v;
	double x = r * cos(phi);
	double y = r * sin(phi);
	return Vector3D(x, y, z);
}

