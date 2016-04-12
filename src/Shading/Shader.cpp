#include "Shading/Shader.h"
#include "Geometry/DifferentialGeometry.h"

Shader::Shader()
{
}
Shader::~Shader()
{
}
void Shader::setDiffuse(const ColorRGB &diff)
{
	diffuse = diff;
}
void Shader::setSpecular(const ColorRGB &spec)
{
	specular = spec;
}
void Shader::setSolidSpecular(const Float& spth)
{
	specType = SOLID_SPEC;
	specThreshold = spth;
}
void Shader::setSoftSpecular()
{
	specType = SOFT_SPEC;
}
void Shader::setOpacity(const bool& opc)
{
	opacity = opc;
}
void Shader::attachAmbientTexture(Texture* &aTex)
{
	ambiTex = aTex;
}
void Shader::attachDiffuseTexture(Texture* &dTex)
{
	diffTex = dTex;
}
void Shader::attachSpecularTexture(Texture* &sTex)
{
	specTex = sTex;
}
bool Shader::getOpacity() const
{
	return opacity;
}
ColorRGBA Shader::getAmbient(const DifferentialGeometry* queryPoint) const
{
	if (ambiTex == nullptr)
	{
		return ambient;
	}
	else
	{
		return ambiTex->getColor(queryPoint);
	}
}
ColorRGBA Shader::getDiffuseAt(const DifferentialGeometry* queryPoint, const Light* light) const
{
	ColorRGBA ret;
	Float cosTheta = queryPoint->getDiffuseTheta();
	Float c = cosTheta > diff_max ? diff_max : (cosTheta < diff_min ? diff_min : cosTheta);
	c = (c - diff_min) / (diff_max - diff_min);
	//return (diffuse * light->getColor() * light->getIntensity(dist) * c + ambient * (1 - c)).returnClamp();

	if (diffTex == nullptr)
	{
		Spectrum sptr = light->getSpectrum(queryPoint);
		//(diffuse*sptr).printInfo();
		ret = ColorRGBA(diffuse * sptr, c);
		//return ColorRGBA(diffuse * light->getColor() * light->getIntensity(queryPoint) * c);
		return ret;
	}
	else
	{
		Spectrum sptr = light->getSpectrum(queryPoint);
		//return ColorRGBA(diffTex->getColor(queryPoint) * light->getColor() * light->getIntensity(queryPoint) * c);
		//return ColorRGBA(diffTex->getColor(queryPoint) * light->getSpectrum(queryPoint), c);
		//diffTex->getColor(queryPoint).printInfo();
		ret = ColorRGBA(diffTex->getColor(queryPoint) * sptr, c);
		//return ColorRGBA(diffuse * light->getColor() * light->getIntensity(queryPoint) * c);
		return ret;
	}
	// .returnClamp();
}
ColorRGBA Shader::getSpecularAt(const DifferentialGeometry* queryPoint, const Light* light) const
{
	ColorRGBA ret;
	Float cosTheta = queryPoint->getSpecularTheta();
	Float s = cosTheta > spec_max ? spec_max : (cosTheta < spec_min ? spec_min : cosTheta);
	s = (s - spec_min) / (spec_max - spec_min);
	if (specType == SOFT_SPEC)
	{
		s = pow(s, 20);
	}
	else
	{
		s = s > specThreshold ? 1 : 0;
	}
	//return (specular * light->getColor() * light->getIntensity(dist) * s + diffColor * (1 - s)).returnClamp();
	if (specTex == nullptr)
	{
		//return ColorRGBA(specular * light->getColor() * light->getIntensity(queryPoint), s).returnClamp();
		ret = ColorRGBA(specular * light->getSpectrum(queryPoint));// .returnClamp();
	}
	else
	{
		//return ColorRGBA(specTex->getColor(queryPoint) * light->getColor() * light->getIntensity(queryPoint), s).returnClamp();
		ret = ColorRGBA(specTex->getColor(queryPoint) * light->getSpectrum(queryPoint), s);// .returnClamp();
	}

	ret.a = s;
	return ret;
	//return ( diffColor * (1 - s)).returnClamp();
}
ColorRGBA Shader::getColor(const DifferentialGeometry* queryPoint, const Light* light) const
{
	if (queryPoint->pos.z > 10)
	{
		//cout << "hehe";
	}
	//light->getDirection(queryPoint);
	//Float dist = light->getDistanceFromPoint(intersectPoint);
	//Float its = light->getIntensity(intersectPoint);
	//ColorRGB tmpDiff = getDiffuseAt(lightDir * normalVec);
	ColorRGBA retAmbi = getAmbient(queryPoint);
	ColorRGBA retDiff = getDiffuseAt(queryPoint, light);
	ColorRGBA retSpec = getSpecularAt(queryPoint, light);
	//return compAdd(retDiff, ambient);
	return compAdd(retSpec, compAdd(retDiff, retAmbi));
}
/************************************************************************/
/* Lambert Shader                                                       */
/************************************************************************/
Lambert::Lambert()
{
}
Lambert::Lambert(const ColorRGB &diff, const ColorRGB &amb, const Float& dmin, const Float& dmax)
{
	diffuse = diff;
	ambient = amb;
	diff_min = dmin;
	diff_max = dmax;
}
Lambert::~Lambert()
{
}
ColorRGBA Lambert::getSpecularAt(const DifferentialGeometry* queryPoint, const Light* light) const
{
	return ColorRGBA(0, 0, 0, 0);
}
ColorRGBA Lambert::getColor(const DifferentialGeometry* queryPoint, const Light* light) const
{
	light->getDirection(queryPoint);
	//Float dist = light->getDistanceFromPoint(intersectPoint);
	//ColorRGB tmpDiff = getDiffuseAt(lightDir * normalVec);
	ColorRGBA retDiff = getDiffuseAt(queryPoint, light);
	//ColorRGBA retSpec = getSpecularAt(reflectDir * lightDir, dist, light);
	return compAdd(retDiff, getAmbient(queryPoint));
	//return compAdd(retSpec, compAdd(retDiff, ambient));
}
/************************************************************************/
/* Phong Shader                                                         */
/************************************************************************/
Phong::Phong()
{
}
Phong::Phong(const ColorRGBA& diff, const ColorRGBA& spec, const ColorRGBA& amb)
{
	diffuse = diff;
	specular = spec;
	ambient = amb;
}
Phong::~Phong()
{
}
Phong::Phong(const ColorRGBA& diff, const ColorRGBA& spec, const ColorRGBA& amb, const Float& dmin, const Float& dmax, const Float& smin, const Float& smax)
{
	diffuse = diff;
	specular = spec;
	ambient = amb;
	setDiffuseRange(dmin, dmax);
	setSpecularRange(smin, smax);
}
void Phong::setDiffuseRange(const Float& dmin, const Float& dmax)
{
	diff_max = dmax;
	diff_min = dmin;
}
void Phong::setSpecularRange(const Float& smin, const Float& smax)
{
	spec_min = smin;
	spec_max = smax;
}
void Phong::setCosinePower(const Float& power)
{
	cospower = power;
}
ColorRGBA Phong::getSpecularAt(const DifferentialGeometry* queryPoint, const Light* light) const
{
	Float cosTheta = queryPoint->getSpecularTheta();
	// 	if (cosTheta > 0)
	// 		cout << cosTheta << endl;
	Float s = cosTheta > spec_max ? spec_max : (cosTheta < spec_min ? spec_min : cosTheta);
	//if (queryPoint->objID > 0)
	s = (s - spec_min) / (spec_max - spec_min);
	s = pow(s, cospower);

	if (specTex == nullptr)
	{
		//return ColorRGBA(specular * light->getColor() * light->getIntensity(queryPoint), s).returnClamp();
		return ColorRGBA(specular * light->getSpectrum(queryPoint), s);// .returnClamp();
	}
	else
	{
		//return ColorRGBA(specTex->getColor(queryPoint) * light->getColor() * light->getIntensity(queryPoint), s).returnClamp();
		return ColorRGBA(specTex->getColor(queryPoint) * light->getSpectrum(queryPoint), s);// .returnClamp();
	}
}
/************************************************************************/
/* Gooch Shader                                                         */
/************************************************************************/
/*
Gooch::Gooch()
{
}*/
Gooch::~Gooch()
{
}
Gooch::Gooch(const ColorRGB &wmC, const ColorRGB &clC, const ColorRGB &diff)
{
	warmColor = wmC;
	coolColor = clC;
	diffuse = diff;
}
Gooch::Gooch(const ColorRGB &wmC, const ColorRGB &clC, const ColorRGB &diff, const ColorRGB &silh, const Float& dmin, const Float& dmax)
	: warmColor(wmC), coolColor(clC), silhoutteColor(silh)
{
	diffuse = diff;
	diff_min = dmin;
	diff_max = dmax;
}

Gooch::Gooch(const ColorRGB &wmC, const ColorRGB &clC, const ColorRGB &diff, const ColorRGB &silh)
	: warmColor(wmC), coolColor(clC), silhoutteColor(silh)
{
	warmColor = wmC;
	coolColor = clC;
	diffuse = diff;
	silhoutteColor = silh;
}
void Gooch::setDiffuseRange(const Float& dmin, const Float& dmax)
{
	diff_min = dmin;
	diff_max = dmax;
}
ColorRGBA Gooch::getDiffuseAt(const DifferentialGeometry* queryPoint, const Light* light) const
{
	Float cosTheta = queryPoint->getDiffuseTheta();
	ColorRGBA wmC = warmColor + diffuse * diff_max;
	ColorRGBA clC = coolColor + diffuse * diff_min;
	//Float c = cosTheta > diff_max ? diff_max : (cosTheta < diff_min ? diff_min : cosTheta);
	Float c = (cosTheta + 1) / 2;
	return (wmC * c + clC * (1 - c)).returnClamp();
}
ColorRGBA Gooch::getColor(const DifferentialGeometry* queryPoint, const Light* light) const
{
	light->getDirection(queryPoint);
	//Float dist = light->getDistance(queryPoint);
	//ColorRGB tmpDiff = getDiffuseAt(lightDir * normalVec);
	Float edgeVec = queryPoint->normal * queryPoint->reflectDir;
	if (edgeVec < 0.3&&edgeVec > -0.3)
	{
		return silhoutteColor * (queryPoint->getDiffuseTheta() + 1) / 2;
	}
	// 	else

	ColorRGBA retDiff = getDiffuseAt(queryPoint, light);
	ColorRGBA retSpec = getSpecularAt(queryPoint, light);
	return compAdd(retSpec, retDiff);
	//return getSpecularAt(reflectDir * lightDir, getDiffuseAt(lightDir * normalVec, light), light);
}