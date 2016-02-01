#pragma once
#ifndef __KAGUYA__
#define __KAGUYA__
//#include <stdio.h>
#include <cstdio>
#include <cstdint>
#include <assert.h>
#include <iostream>
#include <algorithm>

// Containers
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <time.h>

#ifdef KAGUYA_DOUBLE_AS_FLOAT
typedef double Float;
#else
typedef float Float;
#endif // Float

#ifndef RIGHT_HAND_ORDER
#define RIGHT_HAND_ORDER
#endif
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
struct KdAccelNode;
class KdTreeAccel;
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
