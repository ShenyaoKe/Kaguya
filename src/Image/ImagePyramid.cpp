#include "ImagePyramid.h"
//#include "Image/Filter.cpp"

namespace Kaguya
{

ImagePyramid::ImagePyramid(const ImageData &src, int lv) : levels(lv)
{
	layers.resize(levels);
	layers[0] = new ImageData(src);
	for (int i = 1; i < levels; i++)
	{
		ImageData* blurImg = filter::gaussianBlur(layers[i - 1], 2);
		int wdt = layers[i - 1]->getWidth() >> 1;
		int hgt = layers[i - 1]->getHeight() >> 1;
		ImageData* downImg = new ImageData(wdt, hgt);
		for (int x = 0; x < wdt; x++)
		{
			for (int y = 0; y < hgt; y++)
			{
				downImg->setRGBA(x, y, blurImg->getRGBA(x << 1, y << 1));
			}
		}
		layers[i] = downImg;
	}
}

ImagePyramid::~ImagePyramid()
{
}

void ImagePyramid::getPixels(int &wdt, int &hgt, unsigned char* &pixMap) const
{
	wdt = layers[0]->getWidth() * 2;
	hgt = layers[0]->getHeight();
	delete pixMap;
	pixMap = new unsigned char[wdt * hgt * 3];

	ImageData txImg(wdt, hgt);
	int offx(0), offy(0);
	for (int lyIdx = 0; lyIdx < levels; lyIdx++)
	{
		int lyW = layers[lyIdx]->getWidth();
		int lyH = layers[lyIdx]->getHeight();
		int pIdx = offy * hgt + offx;
		for (int x = 0; x < wdt; x++)
		{
			for (int y = 0; y < hgt; y++)
			{
				ColorRGBA tmpC = layers[lyIdx]->getRGBA(x, y);
				pixMap[pIdx++] = static_cast<unsigned char>(tmpC.r * 255);
				pixMap[pIdx++] = static_cast<unsigned char>(tmpC.g * 255);
				pixMap[pIdx++] = static_cast<unsigned char>(tmpC.b * 255);
			}
			pIdx += (wdt - lyW) * 3;
		}
		offx += lyW;
		offy += lyH;
	}
}

}
