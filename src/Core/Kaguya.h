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
#include <array>
#include <vector>
using std::vector;
#include <list>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
using floats_t = vector<float>;
using doubles_t = vector<double>;
using ui8s_t = vector<uint8_t>;
using ui16s_t = vector<uint16_t>;
using ui32s_t = vector<uint32_t>;
using int8s_t = vector<int8_t>;
using int16s_t = vector<int16_t>;
using int32s_t = vector<int32_t>;

#include <ctime>
#include <string>
//#include <stdint.h>

#include <memory>

#ifdef KAGUYA_DOUBLE_AS_FLOAT
typedef double Float;
#else
typedef float Float;
#endif // Float

#ifndef RIGHT_HAND_ORDER
#define RIGHT_HAND_ORDER
#endif

#if defined(KAGUYA_IS_WINDOWS)
#ifdef _MSC_VER
#pragma once
#pragma warning (disable:4996) // scanf_s error
#endif // _MSVC
#pragma warning (disable : 4305) // double constant assigned to float
#pragma warning (disable : 4244) // int -> float conversion
#pragma warning (disable : 4267) // size_t -> unsigned int conversion
#endif

#if !defined(NDEBUG) && !defined(_DEBUG)
#	define _DEBUG
#endif

#ifdef NDEBUG
#define Assert(expr) ((void)0)
#else
#define Assert(expr)                                                     \
    ((expr) ? (void)0 : assert("Assertion \"%s\" failed in %s, line %d", \
                               #expr, __FILE__, __LINE__))
#endif  // NDEBUG
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
template <typename T>
class Bounds2;
template <typename T>
class Bounds3;

class Spectrum;
class Light;

struct KdAccelNode;
class KdTreeAccel;
class Shape;
class Ray;
class DifferentialGeometry;
class TriangleMesh;
class Triangle;

class Texture;
class TextureMapping;
class Shader;

class Camera;
class perspCamera;
class abstractCamera;

class renderBuffer;

const int default_resX = 640;
const int default_resY = 480;

#endif // __KAGUYA__
