#include "RasterizedVolume.h"

RasterizedVolume::RasterizedVolume(const Mesh* src, double div)
	: mesh(src), division(div)
{
	auto fix = [](int y){ return y++; };
}

RasterizedVolume::~RasterizedVolume()
{
}
