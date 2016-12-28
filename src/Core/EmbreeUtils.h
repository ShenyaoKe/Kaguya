#pragma once
#include "Core/Kaguya.h"
#include "embree2/rtcore.h"

namespace Kaguya
{
namespace EmbreeUtils
{

void createDevice(const char* cfg = nullptr);
void deleteDevice();
RTCDevice getDevice();

}
}
