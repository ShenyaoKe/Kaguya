#include "EmbreeUtils.h"

namespace Kaguya
{
namespace EmbreeUtils
{

static RTCDevice sEmbreeDevice = nullptr;

RTCDevice createDevice(const char* cfg)
{
	sEmbreeDevice = rtcNewDevice(cfg);
	return sEmbreeDevice;
}

void deleteDevice()
{
	rtcReleaseDevice(sEmbreeDevice);
	sEmbreeDevice = nullptr;
}

RTCDevice getDevice()
{
	if (sEmbreeDevice == nullptr)
	{
		return createDevice();
	}

	return sEmbreeDevice;
}

}
}
