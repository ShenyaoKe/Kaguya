#include "EmbreeUtils.h"

namespace Kaguya
{
namespace EmbreeUtils
{

static RTCDevice sEmbreeDevice = nullptr;

void createDevice(const char* cfg)
{
    sEmbreeDevice = rtcNewDevice(cfg);
}

void deleteDevice()
{
    rtcDeleteDevice(sEmbreeDevice);
    sEmbreeDevice = nullptr;
}

RTCDevice getDevice()
{
    return sEmbreeDevice;
}

}
}
