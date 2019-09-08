#pragma once

#include "Core/Kaguya.h"

#include "Math/MathUtil.h"
#include "Math/Vector.h"

namespace Kaguya
{

struct Sampler
{
	virtual Float generate1D() const { return unitRandom(10); }
	virtual Point2f generate2D() const { return Point2f(unitRandom(10), unitRandom(10)); }

};

struct CameraSample
{
	Point2f mFilm;
	Point2f mLens;
	Float mTime;
};

}
