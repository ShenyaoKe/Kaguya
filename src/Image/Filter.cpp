#include "Image/Filter.h"

/*
ImageData* filter::curveAdj(const ImageData* cvImg, const ImageData* src)
{
	unsigned char histCv[255] = {};
    
	// Posterize input image
	const int lv = 6;
	int step = 255 / (lv - 1);
	ImageData* postImg(cvImg.width, cvImg.height);
	Vector2D cvP[lv];
	cvImg.posterize(postImg, lv);
	postImg.updateHist();
    
	for (int i = 0; i < lv; i++) {
		cvP[i].x = step * i;
	}
    
	for (int j = 0; j < postImg.height; j++) {
		for (int i = 0; i < postImg.width; i++) {
			int index = postImg.pixels[i][j].r / step;
			cvP[index].y += postImg.pixels[i][j].b / (Float)postImg.histRGB[2][index * step];
		}
	}
	for (int i = 0; i < lv; i++) {
		cvP[i].getInfo();
	}
	histCv[0] = static_cast<unsigned char>(cvP[0].y);
	// End of posterization and get histogram adjust point from RGB.
    
	// Catmull-Rom curve
	// Create CR curve using points created by posterized image.
	for (int i = 0; i < lv - 1; i++) {
		Float p[4];
		p[0] = (i == 0) ? cvP[i].y : cvP[i - 1].y;
		p[1] = cvP[i].y;
		p[2] = cvP[i+1].y;
		p[3] = (i == lv - 2) ? cvP[i + 1].y : cvP[i + 2].y;
		Float t3 = 0 - p[0] + 3 * p[1] - 3 * p[2] + p[3];
		Float t2 = 2 * p[0] - 5 * p[1] + 4 * p[2] - 1 * p[3];
		Float t1 = 0 - p[0] + p[2];
		Float t0 = 2 * p[1];
		Float tmp;
        
		for (int j = 1; j <= step; j++) {
			Float t = j / static_cast<Float>(step);
			tmp = round((t3 * t * t * t + t2 * t * t + t1 * t + t0) * 0.5);
			histCv[i * step + j] = tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp);
		}
	}
	// End of curve generation.
    
	// Adjust histogram
	for (int j = 0; j < src->getHeight(); j++) {
		for (int i = 0; i < src->getWidth(); i++) {
			ret->pixels[i][j].r = histCv[static_cast<int>(src->pixels[i][j].r)];
			ret->pixels[i][j].g = histCv[static_cast<int>(src->pixels[i][j].g)];
			ret->pixels[i][j].b = histCv[static_cast<int>(src->pixels[i][j].b)];
		}
	}
    
}*/
ImageData* filter::reverse(const ImageData* src)
{
	int wdt = src->getWidth(), hgt = src->getHeight();
	ImageData* ret = new ImageData(wdt, hgt);
	for (int i = 0; i < wdt; i++)
	{
		for (int j = 0; j < hgt; j++)
		{
			ret->setRGBA(i, j, COLOR_WHITE - src->getRGBA(i, j));
		}
	}
	return ret;
}
ImageData* filter::offset(const ImageData*  src, int xOffset, int yOffset)
{
	int wdt = src->getWidth(), hgt = src->getHeight();
	ImageData* ret = new ImageData(wdt, hgt);
	while (xOffset > 0)
	{
		xOffset -= wdt;
	}
	while (yOffset > 0)
	{
		yOffset -= hgt;
	}
	for (int i = 0; i < wdt; i++)
	{
		for (int j = 0; j < hgt; j++)
		{
			int x = (i - xOffset) % wdt;
			int y = (j - yOffset) % hgt;
			ret->setRGBA(i, j, src->getRGBA(x, y));
		}
	}
	return ret;
}
ImageData* filter::smartOffset(const ImageData* src, int xOffset, int yOffset, int bWid)
{
	int wdt = src->getWidth(), hgt = src->getHeight();
	ImageData* ret = new ImageData(wdt, hgt);
	while (xOffset > 0)
	{
		xOffset -= wdt;
	}
	while (yOffset > 0)
	{
		yOffset -= hgt;
	}
	for (int i = 0; i < wdt; i++)
	{
		for (int j = 0; j < hgt; j++)
		{
			int x = (i + static_cast<int>(xOffset * sin(j / bWid * M_PI * 2 * bWid / src->getHeight())) + src->getWidth()) % src->getWidth();
			int y = (j + static_cast<int>(yOffset * sin(i / bWid * M_PI * 2 * bWid / src->getWidth())) + src->getHeight()) % src->getHeight();
			ret->setRGBA(i, j, src->getRGBA(x, y));
		}
	}
	return ret;
}
ImageData* filter::hsvOffset(const ImageData* src, Float hue, Float saturation, Float value)
{
	int wdt = src->getWidth(), hgt = src->getHeight();
	ImageData* ret = new ImageData(wdt, hgt);
	if (hue > 180 || hue < -180) {
		cout << "Error: Hue offset is beyond control!" << endl;
	}
	if (saturation > 1.0 || saturation < -1.0) {
		cout << "Error: Saturation offset is beyond control!" << endl;
	}
	if (value > 1.0 || value < -1.0) {
		cout << "Error: Value offset is beyond control!" << endl;
	}
    
	for (int i = 0; i < wdt; i++)
	{
		for (int j = 0; j < hgt; j++)
		{
			//cout << i << ", " << j << endl;
			//cout << "HSV:" << src->pixels[i][j].hgt << ", " << src->pixels[i][j].s << ", " << src->pixels[i][j].v << ", " << endl;
			ColorRGBA curRGBA = src->getRGBA(i, j);
			ColorHSV curHSV = curRGBA.conv2hsv();
			curHSV.h += hue;
			curHSV.s += saturation;
			curHSV.v += value;
			curHSV.clamp();
			ret->setRGBA(i, j, ColorRGBA(curHSV.conv2rgb(), curRGBA.a));
		}
	}
	return ret;
}
ImageData* filter::hueSwitch(const ImageData* src, Float oriHue, Float range, Float newHue)
{
	int wdt = src->getWidth(), hgt = src->getHeight();
	ImageData* ret = new ImageData(wdt, hgt);
	if (oriHue >= 360 || oriHue < 0) {
		cout << "Error: Hue offset is beyond control!" << endl;
	}
	if (range > 180.0 || range < -180.0) {
		cout << "Error: Switch range is beyond control!" << endl;
	}
	Float hueOffset = newHue - oriHue;

	for (int i = 0; i < wdt; i++)
	{
		for (int j = 0; j < hgt; j++)
		{
			Float tmpH;
			ColorRGBA curRGBA = src->getRGBA(i, j);
			ColorHSV curHSV = curRGBA.conv2hsv();
			if (inRange(curHSV.h, oriHue - range, oriHue + range))
			{
				curHSV.h += hueOffset;
			}
			curHSV.clamp();
			ret->setRGBA(i, j, ColorRGBA(curHSV.conv2rgb(), curRGBA.a));
		}
	}
	return ret;
}


ImageData* filter::equalize(const ImageData* src)
{
	int wdt = src->getWidth(), hgt = src->getHeight();
	ImageData* ret = new ImageData(wdt, hgt);
	int bitlen = 1 << src->getBPP();
	Float avgStep = hgt * wdt / bitlen;
	Float eqArry[256];// euqalized src->data
	Float sumArray[256];// original src->data
	unsigned char *offsets[3];// original src->data shift to what color
    
	/*for (int i = 1; i < 4; i++)
	{
		eqArry[0] = avgStep;
		sumArray[0] = static_cast<Float>(histRGB[i][0]);
		offsets[i - 1] = new unsigned char[256];
        
		for (int j = 1; j < 256; j++)
		{
			eqArry[j] = eqArry[j - 1] + avgStep;
			sumArray[j] = sumArray[j - 1] + static_cast<Float>(histRGB[i][j]);
		}
        
		unsigned char offsetData = 0x00;
		for (int j = 0; j < 256; j++)
		{
            
			for (int k = offsetData; k < 255; k++, offsetData++)
			{
				if (abs(sumArray[j] - eqArry[k]) <= abs(sumArray[j] - eqArry[k + 1])) {
                    
					break;
				}
			}
			offsets[i-1][j] = offsetData;
		}
	}
    
	for (int j = 0; j < hgt; j++)
	{
		for (int i = 0; i < wdt; i++)
		{
			ret->pixels[i][j].r = offsets[0][static_cast<unsigned>(src->pixels[i][j].r * 255)];
			ret->pixels[i][j].g = offsets[1][static_cast<unsigned>(src->pixels[i][j].g * 255)];
			ret->pixels[i][j].b = offsets[2][static_cast<unsigned>(src->pixels[i][j].b * 255)];
		}
	}
	for (int i = 0; i < 3; i++) {
		delete [] offsets[i];
		offsets[i] = NULL;
	}*/
	return ret;
}
/*
ImageData* filter::equalizeLuma(const ImageData* src)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	Float avgStep = src->getHeight() * src->getWidth() / 255.0;
	Float eqArry[256];// euqalized src->data
	Float sumArray[256];// original src->data
	unsigned char offset[256];// original src->data shift to what color
    
	eqArry[0] = avgStep;
	sumArray[0] = static_cast<Float>(histRGB[0][0]);
    
	for (int j = 1; j < 256; j++) {
		eqArry[j] = eqArry[j - 1] + avgStep;
		sumArray[j] = sumArray[j - 1] + static_cast<Float>(histRGB[0][j]);
	}
    
	//////////////
	int cdfMin, index = 0;
	do {
		cdfMin = sumArray[index];
		index++;
	} while (cdfMin==0);
	//////////////
    
	for (int j = 0; j < 256; j++) {
		if (sumArray[j]) {
			offset[j] = static_cast<int>(round((sumArray[j] - cdfMin) * 255.0 / (width * src->getHeight() - cdfMin)));
		}
	}
    
	for (int j = 0; j < src->getHeight(); j++) {
		for (int i = 0; i < src->getWidth(); i++) {
			ret->pixels[i][j].r = offsets[0][static_cast<int>(src->pixels[i][j].r * 255)];
			ret->pixels[i][j].g = offsets[1][static_cast<int>(src->pixels[i][j].g * 255)];
			ret->pixels[i][j].b = offsets[2][static_cast<int>(src->pixels[i][j].b * 255)];
		}
	}
	return ret;
}*/
ImageData* filter::posterize(const ImageData* src, int level)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	if (level == 1)
	{
		cout << "Error: Level 1 is not allowed!\nAutomaticly switch to Level 2!" << endl;
		level++;
	}
	Float color_div = 1.0 / static_cast<Float>(level - 1.0);
	Float* post_val = new Float[level];
	*post_val = 0;
	for (int i = 1; i < level; i++)
	{
		post_val[i] = post_val[i - 1] + color_div;
	}

	//int rRange, gRange, bRange;
	for (int i = 0; i < src->getWidth(); i++)
	{
		for (int j = 0; j < src->getHeight(); j++)
		{
			ColorRGBA colorRange = src->getRGBA(i, j) * (level - 1);
			int rIdx = static_cast<int>(colorRange.r + 0.5);
			int gIdx = static_cast<int>(colorRange.g + 0.5);
			int bIdx = static_cast<int>(colorRange.b + 0.5);
            
			ret->setRGBA(i, j, ColorRGBA(post_val[rIdx], post_val[gIdx], post_val[bIdx]));
		}
	}

	delete [] post_val;

	return ret;
}

ImageData* filter::threshold(const ImageData* src, Float th)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	for (int i = 0; i < src->getWidth(); i++)
	{
		for (int j = 0; j < src->getHeight(); j++)
		{
			ColorRGBA pix_val = src->getRGBA(i, j);
			pix_val.r = pix_val.r > th ? 1 : 0;
			pix_val.g = pix_val.g > th ? 1 : 0;
			pix_val.b = pix_val.b > th ? 1 : 0;
			ret->setRGBA(i, j, pix_val);
		}
	}
	return ret;
}

ImageData* filter::edgeDetect(const ImageData* src, Float **lumaImg, EdgeOperator opType)
{
	int width = src->getWidth();
	int height = src->getHeight();

	// Edge image is stored in red channel
	// Gradiance X and Y are stored in green and blue channel
	ImageData* ret = new ImageData(width, height);

	// horizontal and vertical derivative kernels
	int **gX, **gY;
	Aligned_2DArray(gX, 3, 3);
	Aligned_2DArray(gY, 3, 3);
	Float gMax(0), gMin(0);
	/*Float** res;
	Aligned_2DArray(res, width, height);*/

	// initialize kernel
	switch (opType)
	{
	case filter::SOBEL:
	{
		gX[0][0] = -1; gX[0][1] = 0; gX[0][2] = 1;
		gX[1][0] = -2; gX[1][1] = 0; gX[1][2] = 2;
		gX[2][0] = -1; gX[2][1] = 0; gX[2][2] = 1;

		gY[0][0] = 1; gY[0][1] = 2; gY[0][2] = 1;
		gY[1][0] = 0; gY[1][1] = 0; gY[1][2] = 0;
		gY[2][0] = -1; gY[2][1] = -2; gY[2][2] = -1;
		break;
	}
	case filter::PREWITT:
	{
		gX[0][0] = -1; gX[0][1] = 0; gX[0][2] = 1;
		gX[1][0] = -1; gX[1][1] = 0; gX[1][2] = 1;
		gX[2][0] = -1; gX[2][1] = 0; gX[2][2] = 1;

		gY[0][0] = 1; gY[0][1] = 1; gY[0][2] = 1;
		gY[1][0] = 0; gY[1][1] = 0; gY[1][2] = 0;
		gY[2][0] = -1; gY[2][1] = -1; gY[2][2] = -1;
		break;
	}
	case filter::ROBERT:
		break;
	default:
		break;
	}
    
	// if Luma image is not offered, generate it
	if (lumaImg == nullptr)
	{
		lumaImg = src->getLuma();
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Float sumX = 0, sumY = 0;
			//unsigned char newColor;
            
			for (int m = -1; m < 2; m++)
			{
				for (int n = -1; n < 2; n++)
				{
					int iIndex = clamp(i - m, 0, width - 1);
					int jIndex = clamp(j - n, 0, height - 1);
                    
					sumX += lumaImg[iIndex][jIndex] * gX[m + 1][n + 1];
					sumY += lumaImg[iIndex][jIndex] * gY[m + 1][n + 1];
				}
			}

			Float gs_val = sqrt(sqr(sumX) + sqr(sumY));
			//ret->setRGBA(i, j, ColorRGBA(gs_val, gs_val, gs_val));
			ret->setRGBA(i, j, ColorRGBA(gs_val, sumX, sumY));
		}
	}
    
	delete_2DArray(gX, 3, 3);
	delete_2DArray(gY, 3, 3);

	return ret;
}

ImageData* filter::boxBlur(const ImageData* src, int radius)
{
	int width = src->getWidth();
	int height = src->getHeight();
	ImageData* ret = new ImageData(width, height);
	int size = (radius << 1) + 1;
	Float kernel_val = 1.0 / static_cast<Float>(size);

	clock_t startT, endT;
	startT = clock();
    
	ColorRGBA **tmpImgX, **tmpImgY;
	Aligned_2DArray(tmpImgX, width, height);
	Aligned_2DArray(tmpImgY, width, height);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = -radius; k <= radius; k++)
			{
				int iIndex = clamp(i + k, 0, width - 1);
				tmpImgX[i][j] += src->getRGBA(iIndex, j) * kernel_val;
			}
		}
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = -radius; k <= radius; k++)
			{
				int jIndex = clamp(j + k, 0, height - 1);
				tmpImgY[i][j] += tmpImgX[jIndex][i] * kernel_val;
			}
			ret->setRGBA(i, j, tmpImgY[i][j]);
		}
	}

	endT = clock();
	std::cout << "Box blur (radius of " << size << ")runtime :" << (endT - startT) / CLOCKS_PER_SEC << " sec" << std::endl;

	delete_2DArray(tmpImgX, width, height);
	delete_2DArray(tmpImgY, width, height);
	
	return ret;
}

ImageData* filter::gaussianBlur(const ImageData* src, int radius)
{
	int width = src->getWidth();
	int height = src->getHeight();
	ImageData* ret = new ImageData(width, height);

	int size = (radius << 1) + 1;
	Float* kernel = new Float[size];
	Float sigma = radius / 3.0;
	Float sqSigma = sigma * sigma;
	Float sqrt2pi = sqrt(2 * M_PI);
	Float kSum = 0.0;
    
	// Initialize Kernel
	for (int i = -radius; i <= radius; i++)
	{
		kernel[i + radius] = exp( - sqr(i) / (2 * sqSigma)) / (sigma * sqrt2pi);
		kSum += kernel[i + radius];
	}
	for (int i = 0; i < size; i++)
	{
		kernel[i] /= kSum;
		std::cout << i << ": " << kernel[i] << endl;
	}

	// Start Blur
	clock_t startT, endT;
	startT = clock();
    
	ColorRGBA **tmpImgX, **tmpImgY;
	Aligned_2DArray(tmpImgX, width, height);
	Aligned_2DArray(tmpImgY, width, height);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = -radius; k <= radius; k++)
			{
				int iIndex = clamp(i + k, 0, width - 1);
				tmpImgX[i][j] += src->getRGBA(iIndex, j) * kernel[k + radius];
			}
		}
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = -radius; k <= radius; k++)
			{
				int jIndex = clamp(j + k, 0, height - 1);
				tmpImgY[i][j] += tmpImgX[jIndex][i] * kernel[k + radius];
			}
			ret->setRGBA(i, j, tmpImgY[i][j]);
		}
	}
    
	endT = clock();
	std::cout << "Gaussian blur (radius of " << size << ")runtime :" << (endT - startT) / CLOCKS_PER_SEC << " sec" << endl;
    
	delete [] kernel;
	delete_2DArray(tmpImgX, width, height);
	delete_2DArray(tmpImgY, width, height);
	
	return ret;
}

ImageData* filter::bilateral(const ImageData* src, int radius)
{
	int width = src->getWidth();
	int height = src->getHeight();
	ImageData* ret = new ImageData(width, height);
	//int radius = 3;
	int size = (radius << 1) + 1;
	int kSize = sqr(size);
	Float** gKernel;//Spatial Weight
	Float** sKernel;//Similarity Weight
	Float sigmaD = radius / 3.0;// Sigma for gaussian, Can be adjusted
	Float sqSigmaD = sigmaD * sigmaD;// SigmaD^2
	Float sigmaR = 0.1;// Can be adjusted
	Float sqSigmaR = sqr(sigmaR);// SigmaR^2
	Float kSum = 0.0;


	// Initialize box blur kernel
	Aligned_2DArray(gKernel, size, size);
	Aligned_2DArray(sKernel, size, size);
	//
	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
			gKernel[i + radius][j + radius] = exp(-(sqr(i) + sqr(j)) / (2 * sqSigmaD)) / (2 * M_PI * sqSigmaD);
			kSum += gKernel[i + radius][j + radius];
		}

	}
	for (int j = 0; j < size; j++)
	{
		for (int i = 0; i < size; i++)
		{
			gKernel[i][j] /= kSum;
		}
	}

	// Start Blur
	clock_t startT, endT;
	startT = clock();

	Float **lumaImg = src->getLuma();
	//cout << kSize << endl;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			//startT = clock();
			ColorRGBA tmpColor;
			//Float obSize = 0.0;
			Float tmpDist;//Similarity Distance
			kSum = 0;
			for (int m = -radius; m <= radius; m++)
			{
				for (int n = -radius; n <= radius; n++)
				{

					if (i + m >= width || i + m < 0 || j + n >= height || j + n < 0)
					{
						//obradius += gKernel[m + radius][n + radius];
						continue;
					}
					tmpDist = abs(lumaImg[j + n][i + m] - lumaImg[i][j]);
					sKernel[m + radius][n + radius] = exp(-tmpDist * tmpDist / sqSigmaR / 2.0) * gKernel[m + radius][n + radius];
					//cout << sKernel[m + radius][n + radius] << endl;
					kSum += sKernel[m + radius][n + radius];
					
					tmpColor += src->getRGBA(i + m, j + n) * sKernel[m + radius][n + radius];
					//obradius+=0.1;
				}
			}
			//cout << kSum << endl;
			//endT = clock();
			//cout << "kernel time:\t" << (endT - startT) << endl;
			ret->setRGBA(i, j, tmpColor / kSum);
		}
	}
	endT = clock();
	std::cout << "Bilateral blur (radius of " << size << ")runtime :" << (endT - startT) / CLOCKS_PER_SEC << " sec" << endl;

	delete_2DArray(sKernel, size, size);
	delete_2DArray(gKernel, size, size);

	return ret;
}

/*

ImageData* filter::motionBlur(const ImageData* src, const ImageData* mvImg, int radius)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	int size = 2 * radius + 1;
	Float** kernel = new Float*[size];
	Float sigmaX = radius / 3.0;
	Float sqSigmaX = sigmaX * sigmaX;
	Float sigmaY = radius / 12.0;
	Float sqSigmaY = sigmaY * sigmaY;
	Float sqrt2pi = sqrt(2 * M_PI);
	Float kSum = 0.0;
    
	// Define blur kernel
	for (int i = 0; i < 2 * radius + 1; i++) {
		kernel[i] = new Float[2 * radius + 1];
	}
    
	for (int j = 0; j < src->getHeight(); j++) {
		for (int i = 0; i < src->getWidth(); i++) {
			Vector2D dir(mvImg.pixels[i][j].r / 255.0 - 0.5, mvImg.pixels[i][j].g / 255.0 - 0.5);
			Vector2D normalDir(-mvImg.pixels[i][j].g / 255.0 + 0.5, mvImg.pixels[i][j].r / 255.0 - 0.5);
			dir.normalize();
			normalDir.normalize();
			// Initialize mb kernel
			for (int m = -radius; m <= radius; m++) {
				for (int n = -radius; n <= radius; n++) {
					Float kX = dir.dotMul(Vector2D(m, n));
					Float kY = normalDir.dotMul(Vector2D(m, n));
                    
					kernel[m + radius][n + radius] = exp( - (kX * kX) / (2 * sqSigmaX) - (kY * kY) / (2 * sqSigmaY));
					kSum += kernel[m + radius][n + radius];
				}
			}
			for (int m = 0; m < size; m++) {
				for (int n = 0; n < size; n++) {
					kernel[m][n] /= kSum;
				}
			}
			kSum = 0;
			// Finish kernel
			Float tmpRGB[3] = {0, 0, 0};
			// Initialize mb kernel
			for (int m = -radius; m <= radius; m++) {
				for (int n = -radius; n <= radius; n++) {
					int x = (i + m) < 0 ? i : ((i + m) >= src->getWidth() ? (width - 1) : (i + m));
					int y = (j + n) < 0 ? j : ((j + n) >= src->getHeight() ? (height - 1) : (j + n));
                    
					tmpRGB[0] += kernel[m + radius][n + radius] * src->pixels[x][y].r;
					tmpRGB[1] += kernel[m + radius][n + radius] * src->pixels[x][y].g;
					tmpRGB[2] += kernel[m + radius][n + radius] * src->pixels[x][y].b;
				}
			}
			ret->pixels[i][j] = ColorData(tmpRGB[0], tmpRGB[1], tmpRGB[2]);
		}
	}
}
ImageData* filter::emboss(const ImageData* src, Float theta, int radius)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	int size = radius * 2 + 1;
	Float** kernel = new Float*[size];
	//Float gMax = 0, gMin = 0;
	//Float* gsTmp = new Float[width * src->getHeight()];
	Float kMax = 0, kMin = 0;
	//Float* yTmp = new Float[width * src->getHeight()];
	Vector2D dir(cos(theta), sin(theta));
	dir.normalize();
    
	//int* kernel[5];
	for (int i = 0; i < size; i++) {
		kernel[i] = new Float[size];
	}
	for (int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			kernel[i][j] = dir.dotMul(Vector2D(i - 1, j - 1));
			//cout << kernel[i][j] << endl;
			kMax = (kMax > kernel[i][j]) ? kMax : kernel[i][j];
			kMin = (kMin < kernel[i][j]) ? kMin : kernel[i][j];
		}
	}
	for (int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			//kernel[i][j] /= kMax;
		}
	}
    
	for (int j = 0; j < src->getHeight(); j++) {
		for (int i = 0; i < src->getWidth(); i++) {
			ret->pixels[i][j] = ColorData(0xFF, 0xFF, 0xFF);
			//ret->pixels[i][j] = ColorData(0x00, 0x00, 0x00);
			Float sum = 0;
			//unsigned char newColor;
            
			for (int m = -radius; m < radius + 1; m++) {
				for (int n = -radius; n < radius + 1; n++) {
					int iIndex = i - m;
					int jIndex = j - n;
					iIndex = iIndex >= src->getWidth() ? (width - 1) : (iIndex < 0 ? 0 : iIndex);
					jIndex = jIndex >= src->getHeight() ? (height - 1) : (jIndex < 0 ? 0 : jIndex);
                    
					sum += src->pixels[iIndex][jIndex].luma * kernel[m + radius][n + radius];
					//cout << m << " " << n << " " << src->pixels[i - m][j - n].r+0 <<  endl;
				}
			}
			sum = (sum + kMax * 255.0) / (kMax - kMin);
			sum = sum > 255 ? 255 : (sum < 0 ? 0 : sum);
			ret->pixels[i][j] = ColorData(static_cast<unsigned char>(sum), static_cast<unsigned char>(sum), static_cast<unsigned char>(sum));
			ret->pixels[i][j].updateLuma();
		}
	}
    
	for (int i = 0; i < size; i++) {
		delete [] kernel[i];
		kernel[i] = NULL;
	}
}
ImageData* filter::dilation(const ImageData* src, int radius)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	int size = radius * 2 + 1;
	int** kernel = new int*[radius];

	for (int i = 0; i < size; i++) {
		kernel[i] = new int[size];
		for (int j = 0; j < size; j++) {
			if ((i - radius) * (i - radius) + (j - radius) * (j - radius) <= radius * radius) {
				kernel[i][j] = 1;
			} else {
				kernel[i][j] = 0;
			}
            
			cout << kernel[i][j] << " ";
		}
		cout << endl;
	}
	for (int j = 0; j < src->getHeight(); j++) {
		for (int i = 0; i < src->getWidth(); i++) {
			ColorData maxColor = src->pixels[i][j];
			for (int m = -radius; m <= radius; m++) {
				for (int n = -radius; n <= radius; n++) {
					int iData = i - m;
					int jData = j - n;
					if (iData >= src->getWidth() || iData < 0 || jData >= src->getHeight() || jData < 0) {
						continue;
					} else {
						if (maxColor.luma < (src->pixels[iData][jData].luma + kernel[m + radius][n + radius]) && kernel[m + radius][n + radius]) {
							int tmp = kernel[m + radius][n + radius];
							maxColor.r = (src->pixels[iData][jData].r + kernel[m + radius][n + radius]) > 255 ? 255 : (src->pixels[iData][jData].r + kernel[m + radius][n + radius]);
							maxColor.g = (src->pixels[iData][jData].g + kernel[m + radius][n + radius]) > 255 ? 255 : (src->pixels[iData][jData].g + kernel[m + radius][n + radius]);
							maxColor.b = (src->pixels[iData][jData].b + kernel[m + radius][n + radius]) > 255 ? 255 : (src->pixels[iData][jData].b + kernel[m + radius][n + radius]);
							maxColor.updateLuma();
							
						}
					}
				}
				ret->pixels[i][j] = maxColor;
			}
		}
	}
	for (int i = 0; i < size; i++) {
		delete [] kernel[i];
		kernel[i] = NULL;
	}
}
ImageData* filter::erosion(const ImageData* src, int radius)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	int size = 2 * radius + 1;
	int** kernel = new int*[size];

	for (int i = 0; i < size; i++) {
		kernel[i] = new int[size];
		for (int j = 0; j < size; j++) {
			if ((i - radius) * (i - radius) + (j - radius) * (j - radius) <= radius * radius) {
				kernel[i][j] = 1;
			}
			else {
				kernel[i][j] = 0;
			}

			cout << kernel[i][j] << " ";
		}
		cout << endl;
	}
	for (int j = 0; j < src->getHeight(); j++) {
		for (int i = 0; i < src->getWidth(); i++) {
			ColorData minColor = src->pixels[i][j];
			for (int m = -radius; m <= radius; m++) {
				for (int n = -radius; n <= radius; n++) {
					int iData = i - m;
					int jData = j - n;
					if (iData >= src->getWidth() || iData < 0 || jData >= src->getHeight() || jData < 0) {
						continue;
					} else {
						if (minColor.luma >(src->pixels[iData][jData].luma + kernel[m + radius][n + radius]) && kernel[m + radius][n + radius]) {
							minColor.r = (src->pixels[iData][jData].r - kernel[m + radius][n + radius]) < 0 ? 0 : (src->pixels[iData][jData].r - kernel[m + radius][n + radius]);
							minColor.g = (src->pixels[iData][jData].g - kernel[m + radius][n + radius]) < 0 ? 0 : (src->pixels[iData][jData].g - kernel[m + radius][n + radius]);
							minColor.b = (src->pixels[iData][jData].b - kernel[m + radius][n + radius]) < 0 ? 0 : (src->pixels[iData][jData].b - kernel[m + radius][n + radius]);
							minColor.updateLuma();
						}
						//maxColor = maxColor.luma > (src->pixels[iData][jData].luma * kernel[m+radius][n+radius]) ? maxColor : src->pixels[iData][jData];
					}
				}
				ret->pixels[i][j] = minColor;
			}
		}
	}
	for (int i = 0; i < size; i++) {
		delete[] kernel[i];
		kernel[i] = NULL;
	}
}
ImageData* filter::mosaic(const ImageData* src, int size)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	for (int j = 0; j < src->getHeight(); j+=size)
	{
		for (int i = 0; i < src->getWidth(); i+=size) {
			int sum[3] = {};
			int num = 0;
			for (int m = 0; m < size; m++) {
				for (int n = 0; n < size; n++) {
					int iData = i + m;
					int jData = j + n;
					if (iData >= src->getWidth() || jData >= src->getHeight()) {
						continue;
					}
					sum[0] += src->pixels[iData][jData].r;
					sum[1] += src->pixels[iData][jData].g;
					sum[2] += src->pixels[iData][jData].b;
					num++;
				}
			}
			for (int i = 0; i < 3; i++) {
				sum[i] /= num;
			}
			for (int m = 0; m < size; m++) {
				for (int n = 0; n < size; n++) {
					int iData = i + m;
					int jData = j + n;
					if (iData >= src->getWidth() || jData >= src->getHeight()) {
						continue;
					}
					ret->pixels[iData][jData].r = sum[0];
					ret->pixels[iData][jData].g = sum[1];
					ret->pixels[iData][jData].b = sum[2];
					ret->pixels[iData][jData].updateLuma();
				}
			}
		}
	}
}
ImageData* filter::triangulate(const ImageData* src, int size)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	int xStep = src->getWidth() / (size * 2);
	int yStep = src->getHeight() / size;

	for (int j = 0; j <= yStep; j++) {
		for (int i = 0; i <= xStep; i++) {
			//lower triangle
			int lowSum[3] = {};
			int lowNum = 0;
			
			for (int n = 0; n < size; n++) {
				for (int m = n; m < 2 * size - n; m++) {
					int iData = i * size * 2 + m;
					int jData = j * size + n;
					if (iData >= src->getWidth() || jData >= src->getHeight()) {
						continue;
					}
					lowSum[0] += src->pixels[iData][jData].r;
					lowSum[1] += src->pixels[iData][jData].g;
					lowSum[2] += src->pixels[iData][jData].b;
					lowNum++;
				}
			}
			for (int k = 0; k < 3; k++) {
				lowSum[k] /= lowNum;
			}

			for (int n = 0; n < size; n++) {
				for (int m = n; m < 2 * size - n; m++) {
					int iData = i * size * 2 + m;
					int jData = j * size + n;
					if (iData >= src->getWidth() || jData >= src->getHeight()) {
						continue;
					}
					ret->pixels[iData][jData].r = lowSum[0];
					ret->pixels[iData][jData].g = lowSum[1];
					ret->pixels[iData][jData].b = lowSum[2];
					ret->pixels[iData][jData].updateLuma();
				}
			}
			//upper triangle
			int upSum[3] = {};
			int upNum = 0;

			for (int n = 0; n < size; n++) {
				for (int m = -n; m < n; m++) {
					int iData = i * size * 2 + m;
					int jData = j * size + n;
					if (iData >= src->getWidth() || jData >= src->getHeight() || iData < 0 || jData < 0) {
						continue;
					}
					upSum[0] += src->pixels[iData][jData].r;
					upSum[1] += src->pixels[iData][jData].g;
					upSum[2] += src->pixels[iData][jData].b;
					upNum++;
				}
			}
			for (int k = 0; k < 3; k++) {
				upSum[k] /= upNum;
			}

			for (int n = 0; n < size; n++) {
				for (int m = -n; m < n; m++) {
					int iData = i * size * 2 + m;
					int jData = j * size + n;
					if (iData >= src->getWidth() || jData >= src->getHeight() || iData < 0 || jData < 0) {
						continue;
					}
					ret->pixels[iData][jData].r = upSum[0];
					ret->pixels[iData][jData].g = upSum[1];
					ret->pixels[iData][jData].b = upSum[2];
					ret->pixels[iData][jData].updateLuma();
				}
			}
		}
	}
}
ImageData* filter::circlepix(const ImageData* src, int radius, Float para)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	int threshold = 127;//help to define circle size
	int size = 2 * radius + 1;
	int** kernel = new int*[size];
	//int fNum = 0, bNum = 0;

	// Create kernel
	cout << "Circle kernel is:" << endl;
	for (int i = 0; i < size; i++) {
		kernel[i] = new int[size];
		for (int j = 0; j < size; j++) {
			if ((i - radius) * (i - radius) + (j - radius) * (j - radius) <= radius * radius / para) {
				kernel[i][j] = 1;
				//fNum++;
			} else {
				kernel[i][j] = 0;
				//bNum++;
			}
			
			cout << kernel[i][j] << " ";
		}
		cout << endl;
	}
	//cout << fNum << "\t" << bNum << endl;
	for (int j = 0; j < src->getHeight(); j += size) {
		for (int i = 0; i < src->getWidth(); i += size) {
			int fSum[3] = {}, bSum[3] = {};
			int fNum = 0, bNum = 0;
			for (int m = 0; m < size; m++) {
				for (int n = 0; n < size; n++) {
					int iData = i + m;
					int jData = j + n;
					if (iData >= src->getWidth() || jData >= src->getHeight()) {
						continue;
					}
					if (kernel[m][n])
					{
						fSum[0] += src->pixels[iData][jData].r;
						fSum[1] += src->pixels[iData][jData].g;
						fSum[2] += src->pixels[iData][jData].b;
						fNum++;
					} else {
						bSum[0] += src->pixels[iData][jData].r;
						bSum[1] += src->pixels[iData][jData].g;
						bSum[2] += src->pixels[iData][jData].b;
						bNum++;
					}
					
				}
			}
			for (int k = 0; k < 3; k++) {
				fSum[k] /= fNum;
				//bSum[k]=0;// /= bNum;
				bSum[k] /= bNum;
			}
			for (int m = 0; m < size; m++) {
				for (int n = 0; n < size; n++) {
					int iData = i + m;
					int jData = j + n;
					if (iData >= src->getWidth() || jData >= src->getHeight()) {
						continue;
					}
					ret->pixels[iData][jData].r = fSum[0] * kernel[m][n] + bSum[0] * (1 - kernel[m][n]);
					ret->pixels[iData][jData].g = fSum[1] * kernel[m][n] + bSum[1] * (1 - kernel[m][n]);
					ret->pixels[iData][jData].b = fSum[2] * kernel[m][n] + bSum[2] * (1 - kernel[m][n]);
					ret->pixels[iData][jData].updateLuma();
				}
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		delete [] kernel[i];
		kernel[i] = NULL;
	}
}
ImageData* filter::translate(const ImageData* src, Vector2D& tVec, int& winWid, int& winHgt)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	Matrix3D tMat, itMat;
	tMat.setTranslation(tVec);
	tMat.printInfo();
	Float dt = tMat.det;
	itMat = tMat.inverseMat();

	Vector2D pos[4] = {Vector2D(0, 0), Vector2D(src->getWidth(), 0), Vector2D(0, src->getHeight()), Vector2D(src->getWidth(), src->getHeight())};
	Vector2D tmp;
	for (int i = 0; i < 4; i++)
	{
		tmp = tMat * pos[i];
		tmp.getInfo();
		winWid = tmp.x > winWid ? tmp.x : winWid;
		winHgt = tmp.y > winHgt ? tmp.y : winHgt;
	}
	ret->resize(winWid, winHgt);

	for (int i = 0; i < (int)winWid; i++)
	{
		for (int j = 0; j < (int)winHgt; j++)
		{
			Vector3D oriPos = itMat * Vector3D(i, j, 1);
			ret->pixels[i][j] = src->bilinearPixel(oriPos.x, oriPos.y);
		}
	}
	
}
ImageData* filter::rotate(const ImageData* src, Float theta, int& winWid, int& winHgt)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	Matrix3D rMat, tMat1, tMat2, mat, iMat;
	rMat.setRotation(theta);
	tMat1.setTranslation(Vector2D(-width / 2.0, -height / 2.0));
	tMat2.setTranslation(Vector2D(width / 2.0, src->getHeight() / 2.0));
	mat =  tMat2 * rMat * tMat1;
	iMat = mat.inverseMat();

	Vector2D pos[4] = { Vector2D(0, 0), Vector2D(src->getWidth(), 0), Vector2D(0, src->getHeight()), Vector2D(src->getWidth(), src->getHeight()) };
	Vector2D tmp;
	for (int i = 0; i < 4; i++)
	{
		tmp = mat * pos[i];
		winWid = tmp.x > winWid ? tmp.x : winWid;
		winHgt = tmp.y > winHgt ? tmp.y : winHgt;
	}
	ret->resize(winWid, winHgt);

	for (int i = 0; i < (int)winWid; i++)
	{
		for (int j = 0; j < (int)winHgt; j++)
		{
			Vector3D oriPos = iMat * Vector3D(i, j, 1);
			ret->pixels[i][j] = src->bilinearPixel(oriPos.x, oriPos.y);
		}
	}
}
ImageData* filter::scale(const ImageData* src, Vector2D& sVec, int& winWid, int& winHgt)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	Matrix3D sMat, tMat1, tMat2, mat, iMat;
	sMat.setScale(sVec.x, sVec.y);
	tMat1.setTranslation(Vector2D(-width / 2.0, -height / 2.0));
	tMat2.setTranslation(Vector2D(width / 2.0, src->getHeight() / 2.0));
	mat = tMat2 * sMat * tMat1;
	iMat = mat.inverseMat();

	Vector2D pos[4] = { Vector2D(0, 0), Vector2D(src->getWidth(), 0), Vector2D(0, src->getHeight()), Vector2D(src->getWidth(), src->getHeight()) };
	Vector2D tmp;
	for (int i = 0; i < 4; i++)
	{
		tmp = mat * pos[i];
		winWid = tmp.x > winWid ? tmp.x : winWid;
		winHgt = tmp.y > winHgt ? tmp.y : winHgt;
	}
	ret->resize(winWid, winHgt);

	for (int i = 0; i < (int)winWid; i++)
	{
		for (int j = 0; j < (int)winHgt; j++)
		{
			Vector3D oriPos = iMat * Vector3D(i, j, 1);
			ret->pixels[i][j] = src->bilinearPixel(oriPos.x, oriPos.y);
		}
	}
}
ImageData* filter::shear(const ImageData* src, Vector2D& shVec, int& winWid, int& winHgt)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	Matrix3D shMat, iMat;
	shMat.setShear(shVec);
	iMat = shMat.inverseMat();

	Vector2D pos[4] = { Vector2D(0, 0), Vector2D(src->getWidth(), 0), Vector2D(0, src->getHeight()), Vector2D(src->getWidth(), src->getHeight()) };
	Vector2D tmp;
	for (int i = 0; i < 4; i++)
	{
		tmp = shMat * pos[i];
		winWid = tmp.x > winWid ? tmp.x : winWid;
		winHgt = tmp.y > winHgt ? tmp.y : winHgt;
	}
	ret->resize(winWid, winHgt);
	cout << "Window src->getWidth()" << winWid << "\t src->getHeight():" << winHgt << endl;
	for (int i = 0; i < (int)winWid; i++)
	{
		for (int j = 0; j < (int)winHgt; j++)
		{
			Vector3D oriPos = iMat * Vector3D(i, j, 1);
			ret->pixels[i][j] = src->bilinearPixel(oriPos.x, oriPos.y);
		}
	}
}
ImageData* filter::reflect(const ImageData* src, Vector2D& rflVec, int& winWid, int& winHgt)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	Matrix3D rflMat, tMat1, tMat2, mat, iMat;
	Vector2D rflVec_norm = rflVec.getnorm();
	rflMat.setReflection(rflVec_norm);
	tMat1.setTranslation(Vector2D(-width / 2.0, -height / 2.0));
	tMat2.setTranslation(Vector2D(width / 2.0, src->getHeight() / 2.0));
	mat = tMat2 * rflMat * tMat1;
	iMat = mat.inverseMat();

	Vector2D pos[4] = { Vector2D(0, 0), Vector2D(src->getWidth(), 0), Vector2D(0, src->getHeight()), Vector2D(src->getWidth(), src->getHeight()) };
	Vector2D tmp;
	for (int i = 0; i < 4; i++)
	{
		tmp = mat * pos[i];
		winWid = tmp.x > winWid ? tmp.x : winWid;
		winHgt = tmp.y > winHgt ? tmp.y : winHgt;
	}
	ret->resize(winWid, winHgt);
	//cout << "Window src->getWidth()" << winWid << "\t src->getHeight():" << winHgt << endl;
	for (int i = 0; i < (int)winWid; i++)
	{
		for (int j = 0; j < (int)winHgt; j++)
		{
			Vector3D oriPos = iMat * Vector3D(i, j, 1);
			ret->pixels[i][j] = src->bilinearPixel(oriPos.x, oriPos.y);
		}
	}
}
ImageData* filter::perspective(const ImageData* src, Vector2D& vPnt1, int& winWid, int& winHgt)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	Matrix3D mat, iMat;
	mat.setPerspective(vPnt1);
	iMat = mat.inverseMat();
	//mat.printInfo();
	Vector3D pos[4] = { Vector3D(0, 0, 1), Vector3D(src->getWidth(), 0, 1), Vector3D(0, src->getHeight(), 1), Vector3D(src->getWidth(), src->getHeight(), 1) };
	Vector3D tmp;
	for (int i = 0; i < 4; i++)
	{
		tmp = mat * pos[i];
		tmp.x /= tmp.z; tmp.y /= tmp.z; tmp.z = 1.0;
		tmp.getInfo();
		winWid = tmp.x > winWid ? tmp.x : winWid;
		winHgt = tmp.y > winHgt ? tmp.y : winHgt;
	}
	ret->resize(winWid, winHgt);
	cout << "Window src->getWidth()" << winWid << "\t src->getHeight():" << winHgt << endl;
	for (int i = 0; i < (int)winWid; i++)
	{
		for (int j = 0; j < (int)winHgt; j++)
		{
			Vector3D oriPos = iMat * Vector3D(i, j, 1);
			ret->pixels[i][j] = src->bilinearPixel(oriPos.x / oriPos.z, oriPos.y / oriPos.z);
		}
	}
}
ImageData* filter::bilnear(const ImageData* src, Vector2D* pos, int& winWid, int& winHgt)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	// 
	Float a0, a1, a2, a3, b0, b1, b2, b3, c0, c1, c2;
	Float u, v;
	// 1, u, v, u*v
	a0 = pos[0].x; a1 = pos[1].x - pos[0].x; a2 = pos[2].x - pos[0].x; a3 = pos[0].x - pos[1].x - pos[2].x + pos[3].x;
	b0 = pos[0].y; b1 = pos[1].y - pos[0].y; b2 = pos[2].y - pos[0].y; b3 = pos[0].y - pos[1].y - pos[2].y + pos[3].y;

	// get window src->getWidth() & src->getHeight()
	for (int i = 0; i < 4; i++)
	{
		winWid = pos[i].x > winWid ? pos[i].x : winWid;
		winHgt = pos[i].y > winHgt ? pos[i].y : winHgt;
	}
	ret->resize(winWid, winHgt);

	for (int i = 0; i < winWid; i++)
	{
		for (int j = 0; j < winHgt; j++)
		{
			Vector3D oriPos;
			c0 = a1 * (b0 - j) + b1 * (i - a0);
			c1 = a3 * (b0 - j) + b3 * (i - a0) + a1 * b2 - a2 * b1;
			c2 = a3 * b2 - a2 * b3;

			v = -c1 / (2 * c2) + sqrt(c1 * c1 - 4 * c2 * c0) / (2 * c2);
			u = (i - a0 - a2 * v) / (a1 + a3 * v);
			oriPos = Vector3D(u * src->getWidth(), v * src->getHeight(), 1);

			ret->pixels[i][j] = src->bilinearPixel(oriPos.x, oriPos.y);
		}
	}
}
ImageData* filter::inflateScale(const ImageData* src, Float radius, int& winWid, int& winHgt)
{
	ImageData* ret = new ImageData(src->getWidth(), src->getHeight());
	Matrix3D sMat, tMat1, tMat2, mat, iMat;
	tMat1.setTranslation(Vector2D(-width / 2.0, -height / 2.0));
	tMat2.setTranslation(Vector2D(width / 2.0, src->getHeight() / 2.0));

	Vector2D pos[4] = { Vector2D(0, 0), Vector2D(src->getWidth(), 0), Vector2D(0, src->getHeight()), Vector2D(src->getWidth(), src->getHeight()) };
	Vector2D tmp;
	for (int i = 0; i < 4; i++)
	{
		Vector2D lenV(pos[i].x - src->getWidth() / 2.0, pos[i].y - src->getHeight() / 2.0);
		Float len = lenV.getLength();
		if (len >= radius)
		{
			tmp = pos[i];
		}
		else
		{
			sMat.setScale(2 - lenV.getLength() / radius, 2 - lenV.getLength() / radius);
			mat = tMat2 * sMat * tMat1;
			tmp = mat * pos[i];
		}
		winWid = tmp.x > winWid ? tmp.x : winWid;
		winHgt = tmp.y > winHgt ? tmp.y : winHgt;
	}
	ret->resize(winWid, winHgt);

	for (int i = 0; i < winWid; i++)
	{
		for (int j = 0; j < winHgt; j++)
		{
			Vector3D oriPos;
			Vector2D lenV(i - winWid / 2.0, j - winHgt / 2.0);
			Float len = lenV.getLength();

			if (len >= radius)
			{
				oriPos = Vector3D(i, j, 1);
			}
			else
			{
				Float sc = (radius - sqrt((radius - len) * radius)) / len;
				//error 01
				//Float sc = (radius - sqrt((radius - len / (lenV.x / len + 2)) * radius) * (lenV.x / len + 2)) / len;
				//cout << sc << endl;
				//sMat.setScale(sin(sc * pi / 2)); //edge smooth inflate
				sMat.setScale(sc);
				mat = tMat2 * sMat * tMat1;
				oriPos = mat * Vector3D(i, j, 1);
			}
			
			ret->pixels[i][j] = src->bilinearPixel(oriPos.x, oriPos.y);
		}
	}
}

ImageData* filter::textureSynth(const ImageData* sample, int radius)
{
	ImageData* ret = new ImageData(sample->getWidth(), sample->getHeight());
	int neighSize = 2 * (radius * radius + radius);

	// Create vector map for sample image
	Vector3D* smpVec = new Vector3D[sample.width * sample.height];
	for (int v = 0; v < sample.height; v++)
	{
		for (int u = 0; u < sample.width; u++)
		{
			smpVec[u + v * sample.width] = Vector3D(sample.pixels[u][v].r, sample.pixels[u][v].g, sample.pixels[u][v].b);
			//smpVec[u + v * sample.width].getInfo();
		}
	}
	// End of vector map
	// Timer
	clock_t startT, endT;
	startT = clock();
	for (int j = radius; j < src->getHeight() - radius; j++)
	{
		clock_t lineS, lineE;
		lineS = clock();
		for (int i = radius; i < src->getWidth() - radius; i++)
		{
			// Store neighbor pixels
			Vector3D* neighPixs = new Vector3D[neighSize];
			for (int n = -radius; n <= 0; n++)
			{
				for (int m = -radius; m <= radius; m++)
				{
					if (n == 0 && m == 0)
					{
						break;
					}
					else
					{
						//cout << (radius + radius + 1) * n + m + neighSize << endl;
						neighPixs[(radius + radius + 1) * n + m + neighSize] = Vector3D(src->pixels[i + m][j + n].r, src->pixels[i + m][j + n].g, src->pixels[i + m][j + n].b);
					}
				}
			}
			// Find most similar pixel in sample image
			Float vecSum, minSum;
			Vector3D tmpVec, minVec;
			minSum = 442;//442 is sqrt(225*255*3)
			for (int v = 0; v < sample.height; v++)
			{
				for (int u = 0; u < sample.width; u++)
				{
					//cout << u << ", " << v << endl;
					vecSum = 0;
					
					for (int n = -radius; n <= 0; n++)
					{
						for (int m = -radius; m <= radius; m++)
						{
							if (n == 0 && m == 0)
							{
								break;
							}
							else
							{
								int uIndex = u + m < 0 ? 0 : (u + m >= src->getWidth() ? src->getWidth() : u + m);
								int vIndex = v + n < 0 ? 0 : (v + n >= src->getHeight() ? src->getHeight() : v + n);
								tmpVec = neighPixs[(radius * 2 + 1)*n + m + neighSize] - smpVec[vIndex * sample.width + uIndex];
								//cout << tmpVec.getLength() << endl;
								vecSum += tmpVec.getLength() / neighSize;
								//vecSum += (neighPixs[(radius * 2 + 1)*n + m + neighSize] ).getLength() / neighSize;
								//cout << "\t" << (radius * 2 + 1)*n + m + neighSize << endl;
								//cout << "\t" << m << "\t" << n << endl;
							}
						}
					}
					//cout << vecSum << endl;
					if (vecSum < minSum)
					{
						minSum = vecSum;
						minVec = smpVec[v * sample.width + u];
					}
				}
			}
			//cout << "\t" << minSum << endl;
			src->pixels[i][j] = ColorData(minVec.x, minVec.y, minVec.z);
			//minSum = 442;
			delete [] neighPixs;

		}
		lineE = clock();
		std::cout << "Line " << j << " done in " << (lineE - lineS) / CLOCKS_PER_SEC << " sec." << endl;
		//ret->pixels[radius][j].printData();
	}
	endT = clock();
	std::cout << "Timer synthesis runtime :" << (endT - startT) / CLOCKS_PER_SEC << " sec." << endl;
}
*/