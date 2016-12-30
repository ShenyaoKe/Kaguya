#pragma once
#include "Core/Kaguya.h"
#include "Tracer/Ray.h"
#include "Math/Geometry.h"

#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

namespace Kaguya
{
namespace EmbreeUtils
{

RTCDevice createDevice(const char* cfg = nullptr);
void deleteDevice();
RTCDevice getDevice();

inline Ray safeConvert(const RTCRay &src)
{
    return Ray(Point3f(src.org), Vector3f(src.dir), src.tnear, src.tfar);
}

inline RTCRay safeConvert(const Ray &src)
{
    RTCRay ret;
    ret.org[0] = src.o.x;
    ret.org[1] = src.o.y;
    ret.org[2] = src.o.z;

    ret.dir[0] = src.d.x;
    ret.dir[1] = src.d.y;
    ret.dir[2] = src.d.z;

    ret.tnear  = src.tMin;
    ret.tfar   = src.tMax;

    ret.geomID = RTC_INVALID_GEOMETRY_ID;
    ret.primID = RTC_INVALID_GEOMETRY_ID;
    ret.instID = RTC_INVALID_GEOMETRY_ID;

    return ret;

}

}
}
