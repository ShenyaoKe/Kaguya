#if defined(_MSC_VER)
#pragma once
#endif

#ifndef KAGUYA_CORE
#define KAGUYA_CORE

#if defined(_WIN32) || defined(_WIN64)
#define KAGUYA_IS_WINDOWS
#elif defined(__APPLE__)
#define KAGUYA_IS_APPLE
#endif

//#include <stdio.h>
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <assert.h>
#include <iostream>
#include <algorithm>
using std::min;
using std::max;
using std::swap;
using std::sort;

// Containers
#include <vector>
using std::vector;
#include <list>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <ctime>
#include <string>
//#include <stdint.h>

#ifdef KAGUYA_DOUBLE_AS_FLOAT
typedef double Float;
#else
typedef float Float;
#endif // Float

#ifndef RIGHT_HAND_ORDER
#define RIGHT_HAND_ORDER
#endif

#if defined(KAGUYA_IS_WINDOWS)
#pragma warning (disable : 4305) // double constant assigned to float
#pragma warning (disable : 4244) // int -> float conversion
#pragma warning (disable : 4267) // size_t -> unsigned int conversion
#endif

using namespace std;


//Class declaration
class ColorWebRGB;
class ColorRGB;
class ColorRGBA;
class ColorHSV;
class ColorHSVA;

template <typename T>
class Vector2;
template <typename T>
class Vector3;
template <typename T>
class Point2;
template <typename T>
class Point3;
template <typename T>
class Normal3;

class Spectrum;
class Light;

class BBox;
struct KdAccelNode;
class KdTreeAccel;
class Shape;
class Ray;
struct DifferentialGeometry;
class Mesh;
class Triangle;

class Texture;
class TextureMapping;
class Shader;

class Camera;
class perspCamera;
class abstractCamera;

#if !defined(NDEBUG) && !defined(DEBUG)
#	define _DEBUG
#endif

#ifdef NDEBUG
#define Assert(expr) ((void)0)
#else
#define Assert(expr)                                                     \
    ((expr) ? (void)0 : assert("Assertion \"%s\" failed in %s, line %d", \
                               #expr, __FILE__, __LINE__))
#endif  // NDEBUG

const int default_resX = 640;
const int default_resY = 480;

#endif // __KAGUYA__
