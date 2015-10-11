#pragma once
#ifndef __PRIMITIVE__
#define __PRIMITIVE__
//#include "Core/rtdef.h"
#include "Geometry/Shape.h"
#include "Shading/Shader.h"

class Primitive
{
public:
	Primitive();
	virtual ~Primitive();
private:
	Shape* shape;

};


#endif // __PRIMITIVE__