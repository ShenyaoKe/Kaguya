#if defined(_MSC_VER)
#pragma once
#endif

#include "Core\Kaguya.h"

namespace Kaguya
{

struct cameraSampler
{
	int32_t imgX, imgY;
	Float lensU, lensV;
	Float time;
};

}
