#include "Tracer/renderBuffer.h"

/************************************************************************/
/* Buffer Data Function Definition                                      */
/************************************************************************/
void bufferData::init()
{
	DifferentialGeometry = Vector3D();
	normalVec = Vector3D(0, 0, -1);
	objectID = -1;
	depth = 0;
}
void bufferData::setDifferentialGeometry(const Vector3D& vec)
{
	DifferentialGeometry = vec;
}
void bufferData::setNormalVector(const Vector3D& vec)
{
	normalVec = vec;
}
void bufferData::setBeauty(const ColorRGBA& rgb)
{
	beauty = rgb;
}
void bufferData::setAmbient(const ColorRGBA& rgb)
{
	ambient = rgb;
}
void bufferData::setDiffuse(const ColorRGBA& rgb)
{
	diffuse = rgb;
}
void bufferData::setSpecular(const ColorRGBA& rgb)
{
	specular = rgb;
}
void bufferData::setObjectID(int id)
{
	objectID = id;
}
void bufferData::setDepth(Float d)
{
	depth = d;
}
Vector3D bufferData::getDifferentialGeometry() const
{
	return this->DifferentialGeometry;
}
Vector3D bufferData::getNormalVec() const
{
	return this->normalVec;
}
ColorRGBA bufferData::getNormalAsColor() const
{
	return ColorRGBA(0.5f - this->normalVec.x / 2, 0.5f - this->normalVec.y / 2, 0.5f - this->normalVec.z / 2);
}
ColorRGBA bufferData::getBeauty() const
{
	return this->beauty;
}

ColorRGBA bufferData::getAmbient() const
{
	return this->ambient;
}
ColorRGBA bufferData::getDiffuse() const
{
	return this->diffuse;
}
ColorRGBA bufferData::getSpecular() const
{
	return this->specular;
}
int bufferData::getObjectID() const
{
	return this->objectID;
}
Float bufferData::getDepth() const
{
	return this->depth;
}
/************************************************************************/
/* Render Buffer Function Definition                                    */
/************************************************************************/
renderBuffer::renderBuffer(int w, int h)
{
	width = w;
	height = h;
	data = new bufferData*[width];
	for (int i = 0; i < width; i++)
	{
		data[i] = new bufferData[height];
	}
}
void renderBuffer::setDifferentialGeometry(int x, int y, const Vector3D& vec)
{
	if (x < 0 || x >= width || y < 0 || y > height)
	{
		cout << "Set DifferentialGeometry failed!" << endl;
	}
	else
	{
		data[x][y].setDifferentialGeometry(vec);
	}

}
void renderBuffer::setNormalVector(int x, int y, const Vector3D& vec)
{
	if (x < 0 || x >= width || y < 0 || y > height)
	{
		cout << "Set normal vector failed!" << endl;
	}
	else
	{
		data[x][y].setNormalVector(vec);
	}
}
void renderBuffer::setObjectID(int x, int y, int id)
{

	if (x < 0 || x >= width || y < 0 || y > height)
	{
		cout << "Set object ID failed!" << endl;
	}
	else
	{
		data[x][y].setObjectID(id);
	}
}
void renderBuffer::setDepth(int x, int y, Float d)
{

	if (x < 0 || x >= width || y < 0 || y > height)
	{
		cout << "Set depth value failed!" << endl;
	}
	else
	{
		data[x][y].setDepth(d);
	}
}

