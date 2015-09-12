//
//  ColorData.h
//
//  Created by AKIKA on 10/11/14.
//  Copyright (c) 2014 Shenyao Ke. All rights reserved.
//

#ifndef __ColorData__
#define __ColorData__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT


#include "Core/Kaguya.h"
#include "Math/MathUtil.h"

/*
#ifndef Float
typedef float Float
#endif // !Float*/

using namespace std;

class ColorWebRGB
{
public:
	unsigned char r, g, b;
	ColorWebRGB();
	ColorWebRGB(unsigned char red, unsigned char green, unsigned char blue);
	~ColorWebRGB();

private:

};

inline ColorWebRGB::ColorWebRGB()
{
	r = 0;
	g = 0;
	b = 0;
}
inline ColorWebRGB::ColorWebRGB(unsigned char red, unsigned char green, unsigned char blue)
{
	r = red; g = green; b = blue;
}
inline ColorWebRGB::~ColorWebRGB()
{
}
// RGB color space
class ColorRGB
{
private:
protected:
public:
    Float r, g, b; // RGB 0-1.0
    
	explicit ColorRGB() : r(0), g(0), b(0){}
	explicit ColorRGB(unsigned char red, unsigned char green, unsigned char blue)
	{
		r = static_cast<Float>(red) / 255.0;
		g = static_cast<Float>(green) / 255.0;
		b = static_cast<Float>(blue) / 255.0;
	}
	explicit ColorRGB(int red, int green, int blue)
	{
		r = static_cast<Float>(red) / 255.0;
		g = static_cast<Float>(green) / 255.0;
		b = static_cast<Float>(blue) / 255.0;
	}
	explicit ColorRGB(Float red, Float green, Float blue) : r(red), g(green), b(blue){}
    
	virtual ~ColorRGB(){};
    //ColorRGB(Float hue, Float saturation, Float value);
	ColorRGB operator+(const ColorRGB& color2) const { return ColorRGB(r + color2.r, g + color2.g, b + color2.b); }
	ColorRGB operator-(const ColorRGB& color2) const { return ColorRGB(r - color2.r, g - color2.g, b - color2.b); }
	ColorRGB operator*(const ColorRGB& color2) const { return ColorRGB(r * color2.r, g * color2.g, b * color2.b); }
	ColorRGB operator/(const ColorRGB& color2) const { return ColorRGB(r / color2.r, g / color2.g, b / color2.b); }
	ColorRGB operator*(Float n) const { return ColorRGB(r * n, g * n, b * n); }
	ColorRGB operator/(Float n) const { return ColorRGB(r / n, g / n, b / n); }
	
	ColorRGB& operator+=(const ColorRGB& color2) { *this = *this + color2; return *this; }
	ColorRGB& operator-=(const ColorRGB& color2) { *this = *this - color2; return *this; }
	ColorRGB& operator*=(Float n) { *this = *this * n; return *this; }
	ColorRGB& operator/=(Float n) { *this = *this / n; return *this; }

	friend ColorRGB operator*(Float n, const ColorRGB& color){ return color * n; }
	friend ostream& operator<<(ostream &os, const ColorRGB &color);

	virtual void setRGB(Float red, Float green, Float blue){ r = red; g = green; b = blue; };
	virtual void setWebColor(unsigned char red, unsigned char green, unsigned char blue){ r = red / 255.0; g = green / 255.0; b = blue / 255.0; };
	//virtual void setRGB(unsigned short red, unsigned short green, unsigned short blue){ r = red / 255.0; g = green / 255.0; b = blue / 255.0; };
	virtual Float getLuma(){ return 0.2126 * r + 0.7152 * g + 0.0722 * b; }
	virtual void printInfo() const{ cout << "Color data\n\tRed:\t" << this->r << "\n\tGreen:\t" << this->g << "\n\tBlue:\t" << this->b << endl; }
    
	const ColorHSV& conv2hsv() const;
    //inline ColorRGB hsv2rgb();
    //inline void setHSV(Float hue, Float saturation, Float value);
    //inline void updateLuma();
	void clamp();
	ColorRGB returnClamp();

};
class ColorRGBA :public ColorRGB
{
private:
public:
	Float a;//alpha channel
	ColorRGBA() : ColorRGB(), a(1){}
	ColorRGBA(int red, int green, int blue) : ColorRGB(red, green, blue), a(1){}
	ColorRGBA(Float red, Float green, Float blue) : ColorRGB(red, green, blue), a(1){}
	ColorRGBA(Float red, Float green, Float blue, Float alpha) : ColorRGB(red, green, blue), a(alpha){}
	ColorRGBA(ColorRGB rgb) : ColorRGB(rgb.r, rgb.g, rgb.b), a(1){}
	ColorRGBA(ColorRGB rgb, Float alpha) : ColorRGB(rgb.r, rgb.g, rgb.b), a(alpha){}
	~ColorRGBA(){}

	ColorRGBA operator+(const ColorRGBA& color2) const { return ColorRGBA(r + color2.r, g + color2.g, b + color2.b, a + color2.a); }
	ColorRGBA operator-(const ColorRGBA& color2) const { return ColorRGBA(r - color2.r, g - color2.g, b - color2.b, a - color2.a); }
	ColorRGBA operator*(const ColorRGBA& color2) const { return ColorRGBA(r * color2.r, g * color2.g, b * color2.b, a * color2.a); }
	ColorRGBA operator/(const ColorRGBA& color2) const { return ColorRGBA(r / color2.r, g / color2.g, b / color2.b, a / color2.a); }
	ColorRGBA operator*(Float n) const { return ColorRGBA(r * n, g * n, b * n, a * n); }
	ColorRGBA operator/(Float n) const { return ColorRGBA(r / n, g / n, b / n, a / n); }

	ColorRGBA& operator+=(const ColorRGBA& color2) { *this = *this + color2; return *this; }
	ColorRGBA& operator-=(const ColorRGBA& color2) { *this = *this - color2; return *this; }
	ColorRGBA& operator*=(Float n) { *this = *this * n; return *this; }
	ColorRGBA& operator/=(Float n) { *this = *this / n; return *this; }
	ColorRGBA& operator=(const ColorRGB& color2) { this->r = color2.r; this->g = color2.g; this->b = color2.b; return *this; }
	friend ColorRGBA operator*(Float n, const ColorRGBA& color1){ return color1 * n; }
	friend ostream& operator<<(ostream &os, const ColorRGBA &color);

	ColorRGBA compAdd(const ColorRGBA& color2) const{ return *this * this->a + color2 * (1 - this->a); }
	friend ColorRGBA compAdd(const ColorRGBA& color1, const ColorRGBA& color2);

	ColorRGBA multiplyRGB(Float n) const{ return ColorRGBA(r * n, g * n, b * n, a); }

	//void setRGBA(Float red, Float green, Float blue){ setRGB(red, green, blue); };
	void setRGBA(Float red, Float green, Float blue, Float alpha){ setRGB(red, green, blue); a = alpha; };
	//void setRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha){ setRGB(red, green, blue); a = alpha / 255.0; };
	//void setRGBA(unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha){ setRGB(red, green, blue); a = alpha / 255.0; };
	void printInfo() const{ cout << "Color data\n\tRed:\t" << this->r << "\n\tGreen:\t" << this->g << "\n\tBlue:\t" << this->b << "\n\tAlpha:\t" << this->a << endl; };

	const ColorHSVA& conv2hsva() const;

	void clamp();
	ColorRGBA returnClamp();
};
inline ColorRGBA compAdd(const ColorRGBA& color1, const ColorRGBA& color2)
{
	ColorRGBA tmp = color1 * color1.a + color2 * (1 - color1.a);
	//
	return tmp;
}
// HSV color space
class ColorHSV
{
public:
	Float h, s, v;//H is from 0-360
	ColorHSV();
	ColorHSV(Float hue, Float saturation, Float value){ h = hue; s = saturation; v = value; }

	const ColorRGB& conv2rgb() const;

	void clamp();
};


inline const ColorRGB& ColorHSV::conv2rgb() const
{
	Float r, g, b;
	Float h0 = h / 60.0;

	Float chroma = s * v;
	Float x = chroma * (1.0 - abs(fmod(h0, 2.0) - 1.0));

	Float min = v - chroma;

	if (h0 == 0)
	{
		r = min;
		g = min;
		b = min;
	}
	else if (h0 < 1.0)
	{
		r = (min + chroma);
		g = (min + x);
		b = min;
	}
	else if (h0 < 2.0)
	{
		r = (min + x);
		g = (min + chroma);
		b = min;
	}
	else if (h0 < 3.0)
	{
		r = min;
		g = (min + chroma);
		b = (min + x);
	}
	else if (h0 < 4.0)
	{
		r = min;
		g = (min + x);
		b = (min + chroma);
	}
	else if (h0 < 5.0)
	{
		r = (min + x);
		g = min;
		b = (min + chroma);
	}
	else if (h0 < 6.0)
	{
		r = (min + chroma);
		g = min;
		b = (min + x);
	}
	return ColorRGB(r, g, b);
	//cout << "New RGB:" << r << ", " << g << ", " << b << ", " << endl;
	//cout << "HSV:" << h << ", " << s << ", " << v << ", " << endl;
}

inline void ColorHSV::clamp()
{
	while (h < 0.0)
	{
		h += 360.0;
	}
	while (h >= 360.0)
	{
		h -= 360.0;
	}
	clampFromZeroToOne(s);
	clampFromZeroToOne(v);
}

inline void ColorRGB::clamp()
{
	r = r > 1 ? 1 : (r < 0 ? 0 : r);
	g = g > 1 ? 1 : (g < 0 ? 0 : g);
	b = b > 1 ? 1 : (b < 0 ? 0 : b);
}
inline ColorRGB ColorRGB::returnClamp()
{
	Float retR = r > 1 ? 1 : (r < 0 ? 0 : r);
	Float retG = g > 1 ? 1 : (g < 0 ? 0 : g);
	Float retB = b > 1 ? 1 : (b < 0 ? 0 : b);
	return ColorRGB(retR, retG, retB);
}
inline void ColorRGBA::clamp()
{
	ColorRGB::clamp();
	a = a > 1 ? 1 : (a < 0 ? 0 : a);
}

inline ColorRGBA ColorRGBA::returnClamp()
{
	Float retR = r > 1 ? 1 : (r < 0 ? 0 : r);
	Float retG = g > 1 ? 1 : (g < 0 ? 0 : g);
	Float retB = b > 1 ? 1 : (b < 0 ? 0 : b);
	Float retA = a > 1 ? 1 : (a < 0 ? 0 : a);
	ColorRGBA ret(retR, retG, retB, retA);
	return ret;// ColorRGBA(retR, retG, retB, retA);
}

/*
const ColorHSVA& ColorRGBA::conv2hsva() const
{
	return;
}*/

//Convert RGB color data to HSV
inline const ColorHSV& ColorRGB::conv2hsv() const
{
	Float h, s, v;
	Float max = (r > g ? (r > b ? r : b) : (g > b ? g : b));
	Float min = (r < g ? (r < b ? r : b) : (g < b ? g : b));

	v = max;
	Float chroma = max - min;
	if (max == 0)
	{
		h = 0.0;
		s = 0.0;
	}
	else
	{
		s = chroma / max;

		if (chroma == 0)
		{
			h = 0.0;
		} 
		else if (max == r)
		{
			h = 60 * fmod(static_cast<Float>(g - b) / chroma, 6);
		}
		else if (max == g)
		{
			h = 60 * ((b - r) / chroma + 2);
		}
		else if (max == b)
		{
			h = 60 * ((r - g) / chroma + 4);
		}
		
		h = (h >= 0) ? h : (h + 360.0); //
	}
	return ColorHSV(h, s, v);
//cout << "RGB:" << r << ", " << g << ", " << b << ", " << endl;
//cout << "HSV:" << h << ", " << s << ", " << v << ", " << endl;
}

inline ostream& operator<<(ostream &os, const ColorRGB &color)
{
	os << "Color data\n\tRed:\t" << color.r << "\n\tGreen:\t" << color.g << "\n\tBlue:\t" << color.b;
	return os;
}
inline ostream& operator<<(ostream &os, const ColorRGBA &color)
{
	os << "Color data\n\tRed:\t" << color.r << "\n\tGreen:\t" << color.g << "\n\tBlue:\t" << color.b << "\n\tAlpha:\t" << color.a;
	return os;
}
//Color Preset
// From https://pythonhosted.org/ete2/reference/reference_svgcolors.html
//Red Colors
static const ColorRGBA COLOR_LIGHT_CORAL(0xF0, 0x80, 0x80);
static const ColorRGBA COLOR_SALMON(0xFA, 0x80, 0x72);
static const ColorRGBA COLOR_CRIMSON(0xDC, 0x14, 0x3C);
static const ColorRGBA COLOR_RED(0xFF, 0x00, 0x00);
//Pink Colors
static const ColorRGBA COLOR_PINK(0xFF, 0xC0, 0xCB);
static const ColorRGBA COLOR_DEEP_PINK(0xFF, 0x14, 0x93);
//Orange Colors
static const ColorRGBA COLOR_CORAL(0xFF, 0x7F, 0x50);
static const ColorRGBA COLOR_TOMATO(0xFF, 0x45, 0x00);
static const ColorRGBA COLOR_ORANGE_RED(0xFF, 0x45, 0x00);
static const ColorRGBA COLOR_ORANGE(0xFF, 0xA5, 0x00);
//Yellow Colors
static const ColorRGBA COLOR_GOLD(0xFF, 0xD7, 0x00);
static const ColorRGBA COLOR_YELLOW(0xFF, 0xFF, 0x00);
static const ColorRGBA COLOR_LIGHT_YELLOW(0xFF, 0xFF, 0xE0);
static const ColorRGBA COLOR_LEMON_CHIFFON(0xFF, 0xFA, 0xCD);
//Purple Colors
static const ColorRGBA COLOR_LAVENDER(0xE6, 0xE6, 0xFA);
static const ColorRGBA COLOR_VIOLET(0xEE, 0x82, 0xEE);
static const ColorRGBA COLOR_PURPLE(0x80, 0x00, 0x80);
static const ColorRGBA COLOR_INDIGO(0x4B, 0x00, 0x82);
static const ColorRGBA COLOR_SLATE_BLUE(0x6A, 0x5A, 0xCD);
static const ColorRGBA COLOR_GREEN(0x00, 0xFF, 0x00);
static const ColorRGBA COLOR_BLUE(0x00, 0x00, 0xFF);
static const ColorRGBA COLOR_WHITE(0xFF, 0xFF, 0xFF);
static const ColorRGBA COLOR_BLACK(0x00, 0x00, 0x00);
//Blue Colors
static const ColorRGBA COLOR_SKY_BLUE(0x87, 0xCE, 0xEB);
static const ColorRGBA COLOR_DEEP_SKY_BLUE(0x00, 0xBF, 0xFF);
//Brown Color
static const ColorRGBA COLOR_SADDLE_BROWN(0x8B, 0x45, 0x13);

//
static const ColorRGBA COLOR_TRANSPARENT_BLACK(0, 0, 0, 0);
#endif /* defined(__ColorData__) */