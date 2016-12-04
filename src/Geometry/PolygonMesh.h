#pragma once
#include "Geometry/Shape.h"

class PolygonMesh : public Shape
{
public:
    PolygonMesh();
    virtual ~PolygonMesh();
};
enum class AttributesType
{
    CONSTANT,
    UNIFORM,
    VERTEX_VARYING,
    FACE_VARYING,
};
class PolyAttributes
{
public:
    
	PolyAttributes();
	~PolyAttributes();
protected:
	
private:
};