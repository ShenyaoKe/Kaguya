#include "Light/Light.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/Texture.h"

/************************************************************************/
/* Basic Light                                                          */
/************************************************************************/
Light::Light()
{
}
Light::Light(const Spectrum& its)
{
	lightSpectrum = its;
}
Light::~Light()
{
}
void Light::setExposure(const Float& xps)
{
	exposure = xps;
}
void Light::setDecayType(LIGHT_DECAY_TYPE dctype)
{
	decayType = dctype;
}
void Light::setRadius(const Float& rd)
{
	radius = rd;
}
LIGHT_TYPE Light::getLightType() const
{
	return type;
}
Spectrum Light::getSpectrum(const DifferentialGeometry* queryPoint) const
{
	//Spectrum ret;// = Spectrum(getIntensity(queryPoint), lightSpectrum.color);
	return Spectrum();
}
Float Light::getDistance(const DifferentialGeometry* queryPoint) const
{
	return (pos - queryPoint->P).length();
}
void Light::printInfo() const
{
	lightSpectrum.printInfo();
	// 	cout << "Intensity:\t" << intensity.intensity << endl;
	// 	color.printInfo();
}
/************************************************************************/
/* Directional Light                                                    */
/************************************************************************/
directionalLight::directionalLight()
{
	type = LT_DIRECTIONAL_LIGHT;
}
directionalLight::directionalLight(const Vector3f &vec)
{
	dir = Normalize(vec); type = LT_DIRECTIONAL_LIGHT;
}
directionalLight::directionalLight(const Vector3f &vec, const Spectrum& spt)
{
	dir = Normalize(vec); lightSpectrum = spt; type = LT_DIRECTIONAL_LIGHT;
}
directionalLight::~directionalLight()
{
}
void directionalLight::printInfo() const
{
	//cout << "Intensity:\t" << intensity << endl;
	cout << "Directional Light Direction:\t" << dir << endl;
	lightSpectrum.printInfo();
	//color.printInfo();
}
Float directionalLight::getDistance(const DifferentialGeometry* queryPoint) const
{
	return INFINITY;
}
/************************************************************************/
/* Point Light                                                          */
/************************************************************************/
pointLight::pointLight()
{
	type = LT_POINT_LIGHT;
}
pointLight::pointLight(const Point3f &p, const Float& its)
{
	pos = p; lightSpectrum.intensity = its; type = LT_POINT_LIGHT;
}
pointLight::pointLight(const Point3f &p, const Spectrum& spt)
{
	pos = p; lightSpectrum = spt; type = LT_POINT_LIGHT;
}
pointLight::~pointLight()
{}
void pointLight::printInfo() const
{
	cout << "Point Light Position:\t" << pos << endl;
	lightSpectrum.printInfo();
}
/************************************************************************/
/* Spot Light                                                           */
/************************************************************************/
spotLight::spotLight()
{
	type = LT_SPOT_LIGHT;
}
spotLight::spotLight(const Point3f &p, const Vector3f &d)
{
	pos = p;
	dir = d;
	type = LT_SPOT_LIGHT;
}
spotLight::~spotLight()
{
}
spotLight::spotLight(const Point3f &p, const Vector3f &d, const Float& ca, const Float& pa, const Float& dpo)
{
	pos = p;
	dir = Normalize(d);
	setAngles(ca, pa);
	dropoff = dpo;
	type = LT_SPOT_LIGHT;
}
spotLight::spotLight(const Point3f &p, const Vector3f &d, const Float& ca, const Float& pa, const Float& dpo, const Spectrum& spt)
{
	pos = p;
	dir = Normalize(d);
	setAngles(ca, pa);
	dropoff = dpo;
	lightSpectrum = spt;
	type = LT_SPOT_LIGHT;
}
void spotLight::printInfo() const
{

}
void spotLight::setAngles(const Float& ca, const Float& pa)
{
	coneAngle = ca;
	penumbraAngle = pa;
	updateCosAngle();
}
void spotLight::updateCosAngle()
{
	cosCA = cos(DegToRad(coneAngle));
	cosPA = cos(DegToRad(coneAngle + penumbraAngle));
}
void spotLight::setDropOff(const Float& dpo)
{
	dropoff = dpo;
}
Float spotLight::getIntensity(const DifferentialGeometry* queryPoint) const
{
	Float dist = getDistance(queryPoint);
	Float tmpIts = Dot(queryPoint->P - pos,  dir);

	if (penumbraAngle != 0)
	{
		tmpIts = tmpIts > cosCA ? cosCA : (tmpIts < cosPA ? cosPA : tmpIts);
		tmpIts = (tmpIts - cosPA) / (cosCA - cosPA);
		tmpIts = 0.5 - 0.5 * cos(tmpIts * M_PI);
	}
	else
	{
		tmpIts = tmpIts >= cosCA ? 1 : 0;
	}
	if (exposure == 0 && decayType == DECAY_CONSTANT)
	{
		return tmpIts;
	}
	else
	{
		return tmpIts * pow(2, exposure) / (dist * dist);
	}
}
/************************************************************************/
/* Area Light                                                           */
/************************************************************************/
areaLight::areaLight()
{
	nx = Vector3f(1, 0, 0);
	ny = Vector3f(0, 1, 0);
	nz = Vector3f(0, 0, 1);
	type = LT_AREA_LIGHT;
}
areaLight::areaLight(const Point3f &p, const Float& shpSize)
{
	nx = Vector3f(1, 0, 0);
	ny = Vector3f(0, 1, 0);
	nz = Vector3f(0, 0, 1);
	pos = p;
	size = shpSize;
	type = LT_AREA_LIGHT;
}
areaLight::areaLight(const Point3f &p, const Float& shpSize, const Spectrum& spt)
{
	nx = Vector3f(1, 0, 0);
	ny = Vector3f(0, 1, 0);
	nz = Vector3f(0, 0, 1);
	pos = p;
	size = shpSize;
	lightSpectrum = spt;
	type = LT_AREA_LIGHT;
}
areaLight::areaLight(const Point3f &p, const Vector3f &dir, const Vector3f &up, const Float& shpSize, const Spectrum& spt)
{
	nz = Normalize(dir);
	nx = Normalize(Cross(nz, up));
	ny = Cross(nx, nz);
	pos = p;
	size = shpSize;
	lightSpectrum = spt;
	type = LT_AREA_LIGHT;
}
areaLight::~areaLight()
{
}
Float areaLight::getIntensity(const DifferentialGeometry* queryPoint) const
{
	/*if (queryPoint->lightDir * nz > 0)//If on the other side
	{
		return 0;
	}*/
	//
	if (exposure == 0 && decayType == DECAY_CONSTANT)
	{
		return lightSpectrum.intensity;
		;
	}
	else if (decayType == DECAY_CONSTANT)
	{
		return lightSpectrum.intensity * pow(2, exposure);
	}
	else
	{
		Float dist = getDistance(queryPoint);
		return lightSpectrum.intensity * pow(2, exposure) / dist / dist;
	}
}
