//
//  renderBuffer.h
//
//  Created by Shenyao Ke on 2/13/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __renderBuffer__
#define __renderBuffer__
#include "Core/rtdef.h"
// #include "Math/CGVector.h"
// #include "Image/ColorData.h"
#include "Image/ImageData.h"

class bufferData
{
	Vector3D DifferentialGeometry;
	Vector3D normalVec = Vector3D(0, 0, -1);
	ColorRGBA beauty, ambient, diffuse, specular;
	int objectID = 0;
	Float depth = 0;
public:
	

	bufferData()
	{
		beauty = COLOR_TRANSPARENT_BLACK;
		ambient = COLOR_TRANSPARENT_BLACK;
		diffuse = COLOR_TRANSPARENT_BLACK;
		specular = COLOR_TRANSPARENT_BLACK;
	}

	virtual ~bufferData()
	{
	}
	void init();
	void setDifferentialGeometry(const Vector3D& vec);
	void setNormalVector(const Vector3D& vec);
	void setBeauty(const ColorRGBA& rgb);
	void setAmbient(const ColorRGBA& rgb);
	void setDiffuse(const ColorRGBA& rgb);
	void setSpecular(const ColorRGBA& rgb);
	void setObjectID(int id);
	void setDepth(Float d);

	Vector3D getDifferentialGeometry() const;
	Vector3D getNormalVec() const;
	ColorRGBA getNormalAsColor() const;
	ColorRGBA getBeauty() const;
	ColorRGBA getAmbient() const;
	ColorRGBA getDiffuse() const;
	ColorRGBA getSpecular() const;
	int getObjectID() const;
	Float getDepth() const;
};
class renderBuffer
{
public:
	int width = 0;
	int height = 0;
	bufferData** data = NULL;

	renderBuffer()
	{
	}
	renderBuffer(int w, int h);
	virtual ~renderBuffer()
	{
	}

	void setDifferentialGeometry(int x, int y, const Vector3D& vec);
	void setNormalVector(int x, int y, const Vector3D& vec);
	void setObjectID(int x, int y, int id);
	void setDepth(int x, int y, Float d);

	void writeNormalImage(string filename);
	void writeObjectIDImage(string filename);
	void writeZDepthImage(string filename, Float maxDepth);
};

#endif
