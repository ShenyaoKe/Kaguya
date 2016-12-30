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
#include <type_traits>
#include <limits>
#include <iostream>
#include <algorithm>
#include <numeric>

// Containers
#include <array>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
using floats_t  = std::vector<float>;
using doubles_t = std::vector<double>;
using ui8s_t    = std::vector<uint8_t>;
using ui16s_t   = std::vector<uint16_t>;
using ui32s_t   = std::vector<uint32_t>;
using int8s_t   = std::vector<int8_t>;
using int16s_t  = std::vector<int16_t>;
using int32s_t  = std::vector<int32_t>;

#include <ctime>
#include <string>
//#include <stdint.h>

#include <memory>

#include "Core/error.h"

#ifdef KAGUYA_DOUBLE_AS_FLOAT
using Float = double;
#else
using Float = float;
#endif // Float

#ifndef RIGHT_HAND_ORDER
#define RIGHT_HAND_ORDER
#endif

#if defined(KAGUYA_IS_WINDOWS)
#ifdef _MSC_VER
#pragma once
#include <cfloat>
#include <intrin.h>
#pragma warning (disable:4996) // scanf_s error
#endif // _MSVC
#pragma warning (disable : 4305) // double constant assigned to float
#pragma warning (disable : 4244) // int -> float conversion
#pragma warning (disable : 4267) // size_t -> unsigned int conversion
#endif

//////////////////////////////////////////////////////////////////////////
// Assertion
//////////////////////////////////////////////////////////////////////////
#if !defined(NDEBUG) && !defined(_DEBUG)
#	define _DEBUG
#endif

#ifdef NDEBUG
#define Assert(expr) ((void)0)
#else
#define Assert(expr)                                                     \
    ((expr) ? (void)0 : Severe("Assertion \"%s\" failed in %s, line %d", \
                               #expr, __FILE__, __LINE__))
#endif  // NDEBUG

// Add bytes to struct for memory alignment
#define KAGUYA_UNIQUE_NAME_I(x, y) x##y
#define KAGUYA_UNIQUE_NAME(x, y) KAGUYA_UNIQUE_NAME_I(x, y)
#ifndef __COUNTER__
#define KAGUYA_PADDING_BYTE(size) \
        uint8_t KAGUYA_UNIQUE_NAME(Align, __LINE__)[size];
#else
#define KAGUYA_PADDING_BYTE(size) \
        uint8_t KAGUYA_UNIQUE_NAME(Align, __COUNTER__)[size];
#endif

namespace Kaguya
{

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
class Primitive;
class Ray;
class DifferentialGeometry;
class TriangleMesh;
class Triangle;

class Texture;
class TextureMapping;
class Shader;

class Camera;
class ProjectiveCamera;
class PerspectiveCamera;
class OrthographicCamera;

class renderBuffer;

const int default_resX = 640;
const int default_resY = 480;

}

#endif // __KAGUYA__
