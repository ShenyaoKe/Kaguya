#include "Math/MonteCarlo.h"

namespace Kaguya
{

MonteCarlo::MonteCarlo()
{
}


MonteCarlo::~MonteCarlo()
{
}

Point3f SampleHemisphere(Float u, Float v)
{
	Float z(u);
	Float r = sqrt(std::max(0.0, 1.0 - sqr(z)));
	Float phi = 2 * M_PI * v;
	Float x = r * cos(phi);
	Float y = r * sin(phi);
	return Point3f(x, y, z);
}
Point3f SampleSphere(Float u, Float v)
{
	Float z = 1.0 - 2.0 * u;
	Float r = sqrt(std::max(0.0, 1.0 - sqr(z)));
	Float phi = 2.f * M_PI * v;
	Float x = r * cos(phi);
	Float y = r * sin(phi);
	return Point3f(x, y, z);
}

void ConcentricSampleDisk(Float u, Float v, Float &dx, Float &dy)
{
	Float r, theta;

	Float sx = u * 2 - 1;
	Float sy = v * 2 - 1;

	if (sx >= -sy)
	{
		if (sx > sy)
		{
			// Handle first region of disk
			r = sx;
			if (sy > 0.0) theta = sy / r;
			else          theta = 8.0f + sy / r;
		}
		else
		{
			// Handle second region of disk
			r = sy;
			theta = 2.0f - sx / r;
		}
	}
	else
	{
		if (sx <= sy)
		{
			// Handle third region of disk
			r = -sx;
			theta = 4.0f - sy / r;
		}
		else
		{
			// Handle fourth region of disk
			r = -sy;
			theta = 6.0f + sx / r;
		}
	}
	theta *= M_PI * 0.25;

	dx = r * cos(theta);
	dy = r * sin(theta);
}

}
