//
//  Shader.h
//
//  Created by Shenyao Ke on 2/10/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once

#if 0
#include "Core/Kaguya.h"
#include "Geometry/Intersection.h"
#include "Shading/Texture.h"
#include "Light/Light.h"
//#include <vector>

namespace Kaguya
{

enum SPECULAR_TYPE
{
    SOLID_SPEC = 0,
    SOFT_SPEC = 1
};
class Shader
{
public:
    Shader();
    virtual ~Shader();

    virtual void setDiffuse(const ColorRGB &diff);
    virtual void setSpecular(const ColorRGB &spec);
    //virtual void assignDiffuse(ColorRGB* &diff){ &diffuse = diff; };
    //virtual void assignAmbient(ColorRGB&amb){ ambient = amb; };
    void setSolidSpecular(const Float& spth);
    void setSoftSpecular();
    virtual void setOpacity(const bool& opc);

    virtual void attachAmbientTexture(Texture* &aTex);
    virtual void attachDiffuseTexture(Texture* &dTex);
    virtual void attachSpecularTexture(Texture* &sTex);

    virtual bool getOpacity() const;
    virtual ColorRGBA getAmbient(const Intersection* queryPoint) const;
    virtual ColorRGBA getDiffuseAt(const Intersection* queryPoint, const Light* light) const;
    virtual ColorRGBA getSpecularAt(const Intersection* queryPoint, const Light* light) const;
    //virtual ColorRGB getColorAt(const Float& cosTheta) const{ return ColorRGB(); };
    //virtual ColorRGBA getColor(const Vector3D &intersectPoint, const Vector3D &reflectDir, const Vector3D &normalVec, const Light* light) const;
    virtual ColorRGBA getColor(const Intersection* queryPoint, const Light* light) const;

protected:
    ColorRGBA diffuse = COLOR_WHITE;
    ColorRGBA specular = COLOR_WHITE;
    ColorRGBA ambient = COLOR_BLACK;

    Texture* ambiTex = nullptr;
    Texture* diffTex = nullptr;
    Texture* specTex = nullptr;

    Float diff_max = 1;
    Float diff_min = 0;
    Float spec_max = 1;
    Float spec_min = 0;

    SPECULAR_TYPE specType = SOFT_SPEC;
    Float specThreshold = 0.6;

    bool opacity = true;
private:
};
/************************************************************************/
/* Lambert Shader                                                       */
/************************************************************************/
class Lambert :public Shader
{
public:
    Lambert();
    Lambert(const ColorRGB &diff, const ColorRGB &amb, const Float& dmin, const Float& dmax);
    ~Lambert();

    //void setLambert(const ColorRGB &diff, const ColorRGB &amb, const Float& dmin, const Float& dmax);
    //void setDiffuseRange(const Float& dmin, const Float& dmax);
    ColorRGBA getSpecularAt(const Intersection* queryPoint, const Light* light) const;
    ColorRGBA getColor(const Intersection* queryPoint, const Light* light) const;
protected:
private:
};
/************************************************************************/
/* Phong Shader                                                         */
/************************************************************************/
class Phong :public Shader
{
public:
    Phong();
    Phong(const ColorRGBA& diff, const ColorRGBA& spec, const ColorRGBA& amb);
    Phong(const ColorRGBA& diff, const ColorRGBA& spec, const ColorRGBA& amb, const Float& dmin, const Float& dmax, const Float& smin, const Float& smax);
    ~Phong();

    void setDiffuseRange(const Float& dmin, const Float& dmax);
    void setSpecularRange(const Float& smin, const Float& smax);
    void setCosinePower(const Float& power);
    //ColorRGB getColor(const Float& cosTheta);
    ColorRGBA getSpecularAt(const Intersection* queryPoint, const Light* light) const;
protected:

private:
    Float cospower = 20;
};
/************************************************************************/
/* Gooch Shader                                                         */
/************************************************************************/
/*
class Gooch :public Shader
{
public:
    Gooch()
        : warmColor(1.0, 1.0, 0.0), coolColor(0.0, 0.0, 1.0), silhoutteColor(0.0, 0.0, 0.0) {}
    Gooch(const ColorRGB &wmC, const ColorRGB &clC, const ColorRGB &diff);
    Gooch(const ColorRGB &wmC, const ColorRGB &clC, const ColorRGB &diff, const ColorRGB &silh);
    Gooch(const ColorRGB &wmC, const ColorRGB &clC, const ColorRGB &diff, const ColorRGB &silh, const Float& dmin, const Float& dmax);
    ~Gooch();

    void setDiffuseRange(const Float& dmin, const Float& dmax);
    ColorRGBA getDiffuseAt(const Intersection* queryPoint, const Light* light) const;
    ColorRGBA getDiffuseWithEdgeAt(const Float& cosTheta, const Light* light) const;
    //ColorRGB getSpecularAt(const Float& cosTheta, const ColorRGB &diffColor, const Light* light) const;
    ColorRGBA getColor(const Intersection* queryPoint, const Light* light) const;
protected:

private:
    ColorRGBA warmColor;
    ColorRGBA coolColor;
    ColorRGBA silhoutteColor;
    //Float specularThreshold = 0.8;
};*/
/************************************************************************/
/* Solid Specular                                                       */
/************************************************************************/
/*
class ShapedSpecShader :public Shader
{
public:
ShapedSpecShader(){};
ShapedSpecShader(const ColorRGB &diff, const ColorRGB &spec);
~ShapedSpecShader(){};


ColorRGBA getSpecularAt(const Vector3D &Intersection, const Vector3D &reflectDir, const ColorRGBA& diffColor, const Light* light) const;
ColorRGBA getColor(const Vector3D &intersectPoint, const Vector3D &reflectDir, const Vector3D &normalVec, const Light* light) const;
protected:

private:
};
ShapedSpecShader::ShapedSpecShader(const ColorRGB &diff, const ColorRGB &spec)
{
diffuse = diff;
specular = spec;
}
ColorRGBA ShapedSpecShader::getSpecularAt(const Vector3D &Intersection, const Vector3D &reflectDir, const ColorRGBA& diffColor, const Light* light) const
{
Float s = light->getSpecAmout(Intersection, reflectDir);
//s = (s - spec_min) / (spec_max - spec_min);
//cout << s << endl;
if ((light->getDirFromPoint(Intersection))*reflectDir < 0)
{
return diffColor;
}
else
return ColorRGBA(specular * light->getColor(), s).returnClamp();
//return ( diffColor * (1 - s)).returnClamp();

}
ColorRGBA ShapedSpecShader::getColor(const Vector3D &intersectPoint, const Vector3D &reflectDir, const Vector3D &normalVec, const Light* light) const
{
Vector3D lightDir = light->getDirFromPoint(intersectPoint);

Float dist = light->getDistanceFromPoint(intersectPoint);
//ColorRGB tmpDiff = getDiffuseAt(lightDir * normalVec);
Float edgeVec = normalVec * Vector3D(0, 0, -1);
//lightDir.printInfo();
//cout << lightDir * normalVec << endl;

ColorRGBA retDiff = getDiffuseAt(lightDir * normalVec, dist, light);
//ColorRGBA retSpec = getSpecularAt(reflectDir * lightDir, dist, light);
//return compAdd(retSpec, retDiff);
return getSpecularAt(intersectPoint, reflectDir, retDiff, light);
}*/

}
#endif