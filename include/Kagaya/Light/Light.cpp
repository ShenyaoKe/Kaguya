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
void Light::setExposure(const double& xps)
{
	exposure = xps;
}
void Light::setDecayType(LIGHT_DECAY_TYPE dctype)
{
	decayType = dctype;
}
void Light::setRadius(const double& rd)
{
	radius = rd;
}
ColorRGBA Light::getColor() const
{
	return lightSpectrum.color;
}
double Light::getSpecAmout(const Vector3D& DifferentialGeometry, const Vector3D& reflectDir) const
{
	return lightSpectrum.intensity;
}
LIGHT_TYPE Light::getLightType() const
{
	return type;
}
double Light::getIntensity(const double& dist) const
{
	if (exposure == 0 && decayType == DECAY_CONSTANT)
	{
		return lightSpectrum.intensity;
	}
	else
	{
		return lightSpectrum.intensity * pow(2, exposure) / dist / dist;
	}
}
double Light::getIntensity(const DifferentialGeometry *queryPoint) const
{
	return getIntensity(queryPoint->lightDist);
}
Spectrum Light::getSpectrum(const DifferentialGeometry *queryPoint) const
{
	Spectrum ret = Spectrum(getIntensity(queryPoint), lightSpectrum.color);
	return ret;
}
void Light::getDirection(const DifferentialGeometry *queryPoint) const
{
	queryPoint->lightDir = Normalize(pos - queryPoint->pos);
}
double Light::getDistance(const DifferentialGeometry *queryPoint) const
{
	return (pos - queryPoint->pos).getLength();
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
directionalLight::directionalLight(const Vector3D& vec)
{
	dir = Normalize(vec); type = LT_DIRECTIONAL_LIGHT;
}
directionalLight::directionalLight(const Vector3D& vec, const Spectrum& spt)
{
	dir = Normalize(vec); lightSpectrum = spt; type = LT_DIRECTIONAL_LIGHT;
}
directionalLight::~directionalLight()
{
}
void directionalLight::printInfo() const
{
	//cout << "Intensity:\t" << intensity << endl;
	cout << "Directional Light Direction:" << endl;
	dir.printInfo();
	lightSpectrum.printInfo();
	//color.printInfo();
}
double directionalLight::getIntensity(const double& dist) const
{
	if (exposure == 0 && decayType == DECAY_CONSTANT)
	{
		return lightSpectrum.intensity;
	}
	else
	{
		return lightSpectrum.intensity * pow(2, exposure);
	}
}
double directionalLight::getIntensity(const DifferentialGeometry *queryPoint) const
{
	return getIntensity(INFINITY);
}
void directionalLight::getDirection(const DifferentialGeometry *queryPoint) const
{
	queryPoint->lightDir = -dir;
}
double directionalLight::getDistance(const DifferentialGeometry *queryPoint) const
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
pointLight::pointLight(const Vector3D& vec, const double& its)
{
	pos = vec; lightSpectrum.intensity = its; type = LT_POINT_LIGHT;
}
pointLight::pointLight(const Vector3D& vec, const Spectrum& spt)
{
	pos = vec; lightSpectrum = spt; type = LT_POINT_LIGHT;
}
pointLight::~pointLight()
{}
void pointLight::printInfo() const
{
	cout << "Point Light Position:" << endl;
	pos.printInfo();
	lightSpectrum.printInfo();
}
/************************************************************************/
/* Spot Light                                                           */
/************************************************************************/
spotLight::spotLight()
{
	type = LT_SPOT_LIGHT;
}
spotLight::spotLight(const Vector3D& pPos, const Vector3D& pDir)
{
	pos = pPos;
	dir = pDir;
	type = LT_SPOT_LIGHT;
}
spotLight::~spotLight()
{
}
spotLight::spotLight(const Vector3D& pPos, const Vector3D& pDir, const double& ca, const double& pa, const double& dpo)
{
	pos = pPos;
	dir = Normalize(pDir);
	setAngles(ca, pa);
	dropoff = dpo;
	type = LT_SPOT_LIGHT;
}
spotLight::spotLight(const Vector3D& pPos, const Vector3D& pDir, const double& ca, const double& pa, const double& dpo, const Spectrum& spt)
{
	pos = pPos;
	dir = Normalize(pDir);
	setAngles(ca, pa);
	dropoff = dpo;
	lightSpectrum = spt;
	type = LT_SPOT_LIGHT;
}
void spotLight::printInfo() const
{

}
void spotLight::setAngles(const double& ca, const double& pa)
{
	coneAngle = ca;
	penumbraAngle = pa;
	updateCosAngle();
}
void spotLight::updateCosAngle()
{
	cosCA = cos(DegreeToRadian(coneAngle));
	cosPA = cos(DegreeToRadian(coneAngle + penumbraAngle));
}
void spotLight::setDropOff(const double& dpo)
{
	dropoff = dpo;
}
double spotLight::getIntensity(const DifferentialGeometry *queryPoint) const
{
	double dist = getDistance(queryPoint);
	double tmpIts = -queryPoint->lightDir * dir;

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
Vector3D spotLight::getLightPos() const
{
	return pos;
}

ImageSpotLight::ImageSpotLight()
{
	type = LT_SPOT_LIGHT;
}
ImageSpotLight::ImageSpotLight(const Vector3D& pPos, const Vector3D& pDir, const Vector3D& upVec, const double& ca, const double& pa, const double& dpo, const Spectrum& spt) : spotLight(pPos, pDir, ca, pa, dpo, spt)
{
	nx = Normalize(dir.crossMul(upVec));
	ny = Normalize(nx.crossMul(dir));
}
ImageSpotLight::~ImageSpotLight()
{
	delete tex;
	tex = NULL;
}
void ImageSpotLight::assignImage(Texture* &newTex)
{
	tex = newTex;
}
void ImageSpotLight::setSize(const double& xSize, const double& ySize)
{
	sx = xSize;
	sy = ySize;
}
double ImageSpotLight::getIntensity(const DifferentialGeometry *queryPoint) const
{
	double dist = getDistance(queryPoint);
	double tmpIts = -queryPoint->lightDir * dir;
	double maskstr = tmpIts;

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
	if (tex != NULL)
	{
		Vector3D imgPos = queryPoint->lightDir / maskstr - dir;

		tmpIts *= tex->getColor(Vector3D(0.5 - imgPos * nx / sx / 2, 0.5 - imgPos * ny / sy / 2, 0)).a;
	}
	if (exposure != 0 || decayType != DECAY_CONSTANT)
	{
		tmpIts *= pow(2, exposure) / (dist * dist);
	}

	return tmpIts;
}

Spectrum ImageSpotLight::getSpectrum(const DifferentialGeometry *queryPoint) const
{
	double dist = getDistance(queryPoint);
	double maskstr = -queryPoint->lightDir * dir;
	double tmpIts = maskstr;
	ColorRGBA tmpC = lightSpectrum.color;

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
	if (tex != NULL)
	{
		Vector3D imgPos = queryPoint->lightDir / maskstr - dir;

		tmpC = tex->getColor(Vector3D(0.5 - imgPos * nx / sx / 2, 0.5 - imgPos * ny / sy / 2, 0));

		tmpIts *= tmpC.a;
	}
	if (exposure != 0 || decayType != DECAY_CONSTANT)
	{
		tmpIts *= pow(2, exposure) / (dist * dist);
	}

	return Spectrum(lightSpectrum.intensity * tmpIts, tmpC);
}
/************************************************************************/
/* Area Light                                                           */
/************************************************************************/
areaLight::areaLight()
{
	nx = Vector3D(1, 0, 0);
	ny = Vector3D(0, 1, 0);
	nz = Vector3D(0, 0, 1);
	type = LT_AREA_LIGHT;
}
areaLight::areaLight(const Vector3D& pVec, const double& shpSize)
{
	nx = Vector3D(1, 0, 0);
	ny = Vector3D(0, 1, 0);
	nz = Vector3D(0, 0, 1);
	pos = pVec;
	size = shpSize;
	type = LT_AREA_LIGHT;
}
areaLight::areaLight(const Vector3D& pVec, const double& shpSize, const Spectrum& spt)
{
	nx = Vector3D(1, 0, 0);
	ny = Vector3D(0, 1, 0);
	nz = Vector3D(0, 0, 1);
	pos = pVec;
	size = shpSize;
	lightSpectrum = spt;
	type = LT_AREA_LIGHT;
}
areaLight::areaLight(const Vector3D& pVec, const Vector3D& dir, const Vector3D& up, const double& shpSize, const Spectrum& spt)
{
	nz = Normalize(dir);
	nx = Normalize(nz.crossMul(up));
	ny = nx.crossMul(nz);
	pos = pVec;
	size = shpSize;
	lightSpectrum = spt;
	type = LT_AREA_LIGHT;
}
areaLight::~areaLight()
{
}
double areaLight::getIntensity(const DifferentialGeometry *queryPoint) const
{
	if (queryPoint->lightDir * nz > 0)//If on the other side
	{
		return 0;
	}
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
		double dist = getDistance(queryPoint);
		return lightSpectrum.intensity * pow(2, exposure) / dist / dist;
	}
}
void areaLight::getDirection(const DifferentialGeometry *queryPoint) const
{
	double x = ((queryPoint->shiftX + unitRandom(20)) / queryPoint->sample - 0.5) * size;
	double y = ((queryPoint->shiftY + unitRandom(20)) / queryPoint->sample - 0.5) * size;
	//(pos + (sample % sampleSize) * size / sampleSize * nx + (sample / sampleSize) * size * ny - pointPos).printInfo();
	queryPoint->lightDir = Normalize(pos + x * nx + y * ny - queryPoint->pos);
}

double areaLight::getDistance(const DifferentialGeometry *queryPoint) const
{
	double x = ((queryPoint->shiftX + unitRandom(20)) / queryPoint->sample - 0.5) * size;
	double y = ((queryPoint->shiftY + unitRandom(20)) / queryPoint->sample - 0.5) * size;

	return (pos + x * nx + y * ny - queryPoint->pos).getLength();
}