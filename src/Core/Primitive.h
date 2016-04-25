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
	virtual ~Primitive() = 0;

	const uint32_t primitiveID;
protected:
	static uint32_t nextID;

};

class GeometricPrimitive : public Primitive
{
public:
	GeometricPrimitive();
	~GeometricPrimitive();
protected:
	unique_ptr<Shape> geom;
	unique_ptr<Shader> shader;

private:
};


#endif // __PRIMITIVE__