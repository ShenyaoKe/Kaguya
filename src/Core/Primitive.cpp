#include "Core/Primitive.h"

uint32_t Primitive::nextID = 1;


Primitive::Primitive()
	: primitiveID(nextID++)
{
}


Primitive::~Primitive()
{
}
