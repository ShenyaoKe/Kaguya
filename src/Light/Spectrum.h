#pragma once

#include "Image/ColorData.h"

namespace Kaguya
{

/************************************************************************/
/* Spectrum                                                             */
/************************************************************************/
template<uint32_t nSpectrumSample>
class CoefSpectrum
{
public:
	CoefSpectrum(Float val = 0) { mCoefficients.fill(val); }
	~CoefSpectrum() {}

	CoefSpectrum operator+(const CoefSpectrum &other) const
	{
		CoefSpectrum ret = *this;
		for (size_t i = 0; i < nSpectrumSample; i++)
		{
			ret[i] += other[i];
		}
		return ret;
	}
	CoefSpectrum operator-(const CoefSpectrum &other) const
	{
		CoefSpectrum ret = *this;
		for (size_t i = 0; i < nSpectrumSample; i++)
		{
			ret[i] -= other[i];
		}
		return ret;
	}
	CoefSpectrum operator*(Float t) const
	{
		CoefSpectrum ret = *this;
		for (size_t i = 0; i < nSpectrumSample; i++)
		{
			ret[i] *= t;
		}
		return ret;
	}
	CoefSpectrum operator/(Float t) const
	{
		CoefSpectrum ret = *this;
		for (size_t i = 0; i < nSpectrumSample; i++)
		{
			ret[i] /= t;
		}
		return ret;
	}
	CoefSpectrum &operator+=(const CoefSpectrum &other)
	{
		for (size_t i = 0; i < nSpectrumSample; i++)
		{
			mCoefficients[i] += other[i];
		}
		return *this;
	}
	CoefSpectrum &operator-=(const CoefSpectrum &other)
	{
		for (size_t i = 0; i < nSpectrumSample; i++)
		{
			mCoefficients[i] -= other[i];
		}
		return *this;
	}
	CoefSpectrum &operator*=(Float t)
	{
		for (Float &coef : mCoefficients)
		{
			coef *= t;
		}
		return *this;
	}
	CoefSpectrum &operator/=(Float t)
	{

		for (Float &coef : mCoefficients)
		{
			coef /= t;
		}
		return *this;
	}

	friend CoefSpectrum operator*(Float t, const CoefSpectrum &spec)
	{
		return spec * t;
	}

	bool operator==(const CoefSpectrum &other)
	{
		return mCoefficients == other.mCoefficients;
	}

	Float &operator[](size_t i) { return mCoefficients[i]; }
	Float operator[](size_t i) const { return mCoefficients[i]; }

	static constexpr uint32_t sampleCount() { return nSpectrumSample; }

	bool hasNaN() const
	{
		for (auto coef : mCoefficients)
		{
			if (std::isnan(coef))
			{
				return true;
			}
		}
		return false;
	}

	bool isBlack() const
	{
		for (auto coef : mCoefficients)
		{
			if (std::abs(coef) > std::numeric_limits<Float>::epsilon())
			{
				return false;
			}
		}
		return true;
	}
	friend CoefSpectrum sqrt(const CoefSpectrum &s)
	{
		CoefSpectrum ret;
		for (size_t i = 0; i < nSpectrumSample; i++)
		{
			ret[i] = std::sqrt(s[i]);
		}
		return ret;
	}
	friend CoefSpectrum pow(const CoefSpectrum &s, Float e)
	{
		CoefSpectrum ret;
		for (size_t i = 0; i < nSpectrumSample; i++)
		{
			ret[i] = std::pow(s[i], e);
		}
		return ret;
	}
	friend CoefSpectrum exp(const CoefSpectrum &s)
	{
		CoefSpectrum ret;
		for (size_t i = 0; i < nSpectrumSample; i++)
		{
			ret[i] = std::exp(s[i]);
		}
		return ret;
	}

protected:
	std::array<Float, nSpectrumSample> mCoefficients;
};

/************************************************************************/
/* RGB Spectrum                                                         */
/************************************************************************/
class RGBSpectrum : public CoefSpectrum<3>
{
public:
	RGBSpectrum(Float val = 0.f) : CoefSpectrum<3>(val) {}
	RGBSpectrum(const Float val[3])
	{
		mCoefficients[0] = val[0];
		mCoefficients[1] = val[1];
		mCoefficients[2] = val[2];
	}
	~RGBSpectrum() {}

	//ColorRGBA operator*(const ColorRGBA &color2) const;
	RGBSpectrum operator*(Float t) const;
	RGBSpectrum operator/(Float t) const;

	//friend ColorRGBA operator*(const ColorRGBA &color, const RGBSpectrum &spec);
	friend RGBSpectrum operator*(Float t, const RGBSpectrum &spec);

	void printInfo() const;

protected:

private:
	using CoefSpectrum<3>::mCoefficients;
};

/************************************************************************/
/* Legacy Spectrum                                                      */
/************************************************************************/
#ifdef USE_LEGACY_SPECTRUM
class LegacySpectrum
{
public:
	LegacySpectrum();
	LegacySpectrum(Float its);
	LegacySpectrum(Float its, ColorRGBA spc);
	LegacySpectrum(ColorRGBA spc);
	~LegacySpectrum();

	ColorRGBA operator*(const ColorRGBA &color2) const;
	LegacySpectrum operator*(Float t) const;
	LegacySpectrum operator/(Float t) const;

	friend ColorRGBA operator*(const ColorRGBA &color, const LegacySpectrum &spec);
	friend LegacySpectrum operator*(Float t, const LegacySpectrum &spec);

	void printInfo() const;

public:
	ColorRGBA mColor;
	Float mIntensity = 0;
};
#endif

}
