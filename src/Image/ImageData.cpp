#include "Image/ImageData.h"

ImageData::ImageData(int wd, int ht)
{
	width = wd;
	height = ht;
	bpp = 24;

	Aligned_2DArray(pixels, width, height, ROW_STORAGE);
}
ImageData::ImageData(int wd, int ht, Float* &pixMap)
{
	// Declare width and height
	width = wd;
	height = ht;
	bpp = 24;

	Aligned_2DArray(pixels, width, height, ROW_STORAGE);

	for (int i = 0; i < width * height; ++i)
	{
		pixels[0][i] = ColorRGBA(pixMap[i * 3], pixMap[i * 3 + 1], pixMap[i * 3 + 2]);
	}
}
ImageData::ImageData(int wd, int ht, unsigned char* pixMap, int pixtype)
{
	// Declare width and height
	width = wd;
	height = ht;
	bpp = 24;

	Aligned_2DArray(pixels, width, height, ROW_STORAGE);
	
	for (int i = 0; i < width * height; ++i)
	{
		switch (pixtype)
		{
		case RGB:
			pixels[0][i] = ColorRGBA(
				static_cast<Float>(pixMap[i * 3]) / 255.0f,
				static_cast<Float>(pixMap[i * 3 + 1]) / 255.0f,
				static_cast<Float>(pixMap[i * 3 + 2]) / 255.0f);
		case RGBA:
			pixels[0][i] = ColorRGBA(
				static_cast<Float>(pixMap[i * 3]) / 255.0f,
				static_cast<Float>(pixMap[i * 3 + 1]) / 255.0f,
				static_cast<Float>(pixMap[i * 3 + 2]) / 255.0f,
				static_cast<Float>(pixMap[i * 3 + 3]) / 255.0);
		case BGRA:
			pixels[0][i] = ColorRGBA(
				static_cast<Float>(pixMap[i * 3 + 2]) / 255.0f,
				static_cast<Float>(pixMap[i * 3 + 1]) / 255.0f,
				static_cast<Float>(pixMap[i * 3]) / 255.0f,
				static_cast<Float>(pixMap[i * 3 + 3]) / 255.0f);
		default:
			break;
		}
		
	}
}
ImageData::ImageData(const string &filename)
{
	// Define image file format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	// Define a pointer to the image file
	FIBITMAP *dib(0);
	// Define a pointer to the pixel data
	BYTE* bits(0);
	// Define image data type, e.g. RGB16, UINT16, DOUBLE, etc.
	int imgType(0);

	// Check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename.c_str(), 0);
	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(filename.c_str());
	}
	if (fif == FIF_UNKNOWN)
	{
		cout << "ERROR: Unknown image format!" << endl;
		abort();
	}

	// Read image data from file
	if (FreeImage_FIFSupportsReading(fif))
	{
		dib = FreeImage_Load(fif, filename.c_str());
	}
	if (!dib)
	{
		cout << "ERROR: Unable to read file " << filename << endl;
		abort();
	}

	//get image data from dib
	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	imgType = FreeImage_GetImageType(dib);
	bpp = FreeImage_GetBPP(dib);

	//if color data exists, initialize image
	if (bits == 0 || width == 0 || height == 0)
	{
		cout << "ERROR: Image file contains no available image data!" << endl;
		abort();
		//return false;
	}
	else
	{
		Aligned_2DArray(pixels, width, height, ROW_STORAGE);
	}

	// color order is BGR
	switch (imgType)
	{
	case FIT_UNKNOWN:// unknown type
	{
		std::cout << "Cannot recognize image type!" << std::endl;
		break;
	}
	case FIT_BITMAP:// standard image			: 1-, 4-, 8-, 16-, 24-, 32-bit
	{
		BYTE* pixels = bits;
		switch (bpp)
		{
		case 1:
		{
			for (int j = 0; j < height; j++)
			{
				for (int i = 0; i < width; i++)
				{
					int idx = (i + width * j) * 3;

					//cout << idx << ":" << pixels[idx] + 0 << endl;
					this->setRGBA(i, j, ColorRGBA(pixels[idx + 2] / 255.0f,
						pixels[idx + 1] / 255.0f,
						pixels[idx] / 255.0f));
				}
			}
			break;
		}
		case 4:
		case 8:
		case 16:
		case 24:// 8bit RGB image
		{
			for (int j = 0; j < height; j++)
			{
				for (int i = 0; i < width; i++)
				{
					int idx = (i + width * j) * 3;

					//cout << idx << ":" << pixels[idx] + 0 << endl;
					this->setRGBA(i, j, ColorRGBA(pixels[idx + 2] / 255.0f,
						pixels[idx + 1] / 255.0f,
						pixels[idx] / 255.0f));
				}
			}
			break;
		}
		case 32:// 8bit RGBA image
		{
			for (int j = 0; j < height; j++)
			{
				for (int i = 0; i < width; i++)
				{
					int idx = (i + width * j) * 4;
					this->setRGBA(i, j, ColorRGBA(pixels[idx + 2] / 255.0,
						pixels[idx + 1] / 255.0,
						pixels[idx] / 255.0,
						pixels[idx + 3] / 255.0));
				}
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	case FIT_UINT16:// array of unsigned short	: unsigned 16-bit
	{
		break;
	}
	case FIT_INT16:// array of short			: signed 16-bit
	{
		break;
	}
	case FIT_UINT32:// array of unsigned long	: unsigned 32-bit
	{
		break;
	}
	case FIT_INT32:// array of long				: signed 32-bit
	{
		break;
	}
	case FIT_FLOAT:// array of Float			: 32-bit IEEE doubleing point
	{
		break;
	}
	case FIT_DOUBLE:// array of Float			: 64-bit IEEE doubleing point
	{
		break;
	}
	case FIT_COMPLEX:// array of FICOMPLEX		: 2 x 64-bit IEEE doubleing point
	{
		break;
	}
	case FIT_RGB16:// 48-bit RGB image			: 3 x 16-bit
	{
		WORD* pixels = (WORD*)bits;

		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				int idx = (i + width * j) * 3;
				setRGBA(i, j, ColorRGBA(pixels[idx + 2] / 65535.0f,
					pixels[idx + 1] / 65535.0f,
					pixels[idx] / 65535.0f));
			}
			cout << endl;
		}
		break;
	}
	case FIT_RGBA16:// 64-bit RGBA image		: 4 x 16-bit
	{
		WORD* pixels = (WORD*)bits;
		//cout << "num: " << depth / sizeof(pixels[0]) / 8 << endl;
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				int idx = (i + width * j) * 3;
				setRGBA(i, j, ColorRGBA(pixels[idx + 2] / 65535.0,
					pixels[idx + 1] / 65535.0,
					pixels[idx] / 65535.0,
					pixels[idx + 3] / 65535.0));
			}
		}
		break;
	}
	default:
		break;
	}

	if (dib)
	{
		FreeImage_Unload(dib);
	}
}

ImageData::ImageData(const ImageData& src)
{
	// Declare width and height
	width = src.getWidth();
	height = src.getHeight();

	Aligned_2DArray(pixels, width, height, ROW_STORAGE);

	memcpy(pixels[0], src.pixels[0], width * height * sizeof(ColorRGBA));
}

ImageData::~ImageData()
{
	// Free data
	delete_2DArray(pixels);
}
void ImageData::setRGBA(int x, int y, const ColorRGBA &color)
{
	pixels[y][x] = color;
}
const ColorRGBA& ImageData::getRGBA(int x, int y) const
{
	return pixels[y][x];
}

const ColorRGBA& ImageData::getRGBA(int idx) const
{
	return pixels[0][idx];
}
// RGB
void ImageData::getPixels(unsigned char* &pixMap) const
{
	delete[] pixMap;
	
	pixMap = new unsigned char[width * height * 3];

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;//(i % height * width + (i / height)) * 3;
		pixMap[index++] = static_cast<unsigned char>(pixels[0][i].r * 255);
		pixMap[index++] = static_cast<unsigned char>(pixels[0][i].g * 255);
		pixMap[index] = static_cast<unsigned char>(pixels[0][i].b * 255);
	}
}

void ImageData::getPixelsRGBA(unsigned char* &pixMap) const
{
	delete[] pixMap;

	pixMap = new unsigned char[width * height * 4];

	for (int i = 0; i < width * height; ++i)
	{
		int index = i << 2;// (i % height * width + (i / height)) * 4;
		pixMap[index++] = static_cast<unsigned char>(pixels[0][i].r * 255);
		pixMap[index++] = static_cast<unsigned char>(pixels[0][i].g * 255);
		pixMap[index++] = static_cast<unsigned char>(pixels[0][i].b * 255);
		pixMap[index] = static_cast<unsigned char>(pixels[0][i].a * 255);
	}
}

void ImageData::printRGBA(int x, int y) const
{
	cout << pixels[y][x] << endl;
}
void ImageData::resize(int x, int y)
{
	delete pixels;
	width = x;
	height = y;

	Aligned_2DArray(pixels, width, height);
	//pixels = new ColorRGBA[width * height];
}
ColorRGBA ImageData::bilinearPixel(Float x, Float y) const
{
	while (x < 0)
	{
		x += width;
	}
	while (x >= width)
	{
		x -= width;
	}
	while (y < 0)
	{
		y += height;
	}
	while (y >= height)
	{
		y -= height;
	}
	ColorRGBA pixC[4];
	//cout << "x:" << x << "\ty:" << y << endl;
	int floorX = Floor2Int(x), floorY = Floor2Int(y);
	Float tX = x - floorX, tY = y - floorY;
	//int ceilX = (tX == 0 || floorX == width - 1) ? floorX : (floorX + 1);
	//int ceilY = (tY == 0 || floorY == height - 1) ? floorY : (floorY + 1);
	int ceilX = (floorX == width - 1) ? 0 : (floorX + 1);
	int ceilY = (floorY == height - 1) ? 0 : (floorY + 1);
	pixC[0] = getRGBA(floorX, floorY);
	pixC[2] = getRGBA(floorX, ceilY);
	pixC[1] = getRGBA(ceilX, floorY);
	pixC[3] = getRGBA(ceilX, ceilY);

	return lerp(lerp(pixC[0], pixC[1], tX), lerp(pixC[2], pixC[3], tX), tY);
}

bool ImageData::writeFile(const string &filename) const
{
	int BBP = 24;
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, BBP);
	RGBQUAD color;

	if (!bitmap)
	{
		cout << "Failed to allocate FreeImage data!" << endl;
		return false;
	}


	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			//Set color from image data.
			//......
			ColorRGBA curColor = getRGBA(i, j);
			color.rgbRed = static_cast<BYTE>(curColor.r * 255);
			color.rgbGreen = static_cast<BYTE>(curColor.g * 255);
			color.rgbBlue = static_cast<BYTE>(curColor.b * 255);
			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}
	//save image file.
	if (FreeImage_Save(FIF_PNG, bitmap, filename.c_str()))
	{
		cout << "Image successfully saved!" << endl;
		return true;
	}
	return false;
}

int** ImageData::genHist() const
{
	/*int** histRGB = new int*[4];
	int bitlen = 1 << bpp;
	for (int i = 0; i < 4; i++)
	{
		histRGB[i] = new int[bitlen];
	}
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			histRGB[0][static_cast<int>(pixels[i][j]->r)]++;
			histRGB[1][static_cast<int>(pixels[i][j]->r)]++;
			histRGB[2][static_cast<int>(pixels[i][j]->r)]++;
			histRGB[3][static_cast<int>(pixels[i][j]->r)]++;
		}
	}*/
	return nullptr;
}

Float** ImageData::getLuma() const
{
	Float** ret;
	Aligned_2DArray(ret, this->getWidth(), this->getHeight(), COL_STORAGE);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			ColorRGBA curColor = this->getRGBA(i, j);
			ret[i][j] = curColor.r * 0.2126 + curColor.g * 0.7152 + curColor.b * 0.0722;
		}
	}

	return ret;
}
