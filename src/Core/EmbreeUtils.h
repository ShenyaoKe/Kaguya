#pragma once
#include "Core/Kaguya.h"
#include "Tracer/Ray.h"
#include "Math/Vector.h"

#include <embree3/rtcore.h>
#include <embree3/rtcore_ray.h>

namespace Kaguya
{
namespace EmbreeUtils
{

RTCDevice createDevice(const char* cfg = nullptr);
void deleteDevice();
RTCDevice getDevice();

inline Ray safeConvert(const RTCRay &src)
{
	return Ray(Point3f(src.org_x, src.org_y, src.org_z),
			   Vector3f(src.dir_x, src.dir_y, src.dir_z), src.tnear, src.tfar);
}

inline RTCRay safeConvert(const Ray &src)
{
	RTCRay ret;
	ret.org_x = src.o.x;
	ret.org_y = src.o.y;
	ret.org_z = src.o.z;

	ret.dir_x = src.d.x;
	ret.dir_y = src.d.y;
	ret.dir_z = src.d.z;

	ret.tnear = src.tMin;
	ret.tfar = src.tMax;

	ret.id = RTC_INVALID_GEOMETRY_ID;
	//ret.primID = RTC_INVALID_GEOMETRY_ID;
	//ret.instID = RTC_INVALID_GEOMETRY_ID;

	return ret;

}

}
}
