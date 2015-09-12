#pragma once
#ifndef __KAGUYA__
#define __KAGUYA__
//#include <stdio.h>
#include <cstdio>
#include <cstdint>
#include <assert.h>
#include <iostream>
#include <vector>
#include <time.h>

#ifdef KAGUYA_DOUBLE_AS_FLOAT
typedef double Float;
#else
typedef float Float;
#endif // Float

/*
#ifndef GLfloat
typedef float GLfloat
#endif // !GLfloat*/

using namespace std;


//Class declaration
class ColorWebRGB;
class ColorRGB;
class ColorRGBA;
class ColorHSV;
class ColorHSVA;

class Spectrum;
class Light;

class BBox;
class Shape;
class Ray;
struct DifferentialGeometry;
struct DifferentialGeometry;
class Mesh;
class Triangle;

class Texture;
class TextureMapping;
class Shader;

class baseCamera;
class perspCamera;
class abstractCamera;

#endif // __KAGUYA__
