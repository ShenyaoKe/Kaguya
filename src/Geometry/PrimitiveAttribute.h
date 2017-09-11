#pragma once
#include "Math/Vector.h"

namespace Kaguya
{

enum class AttributeType : uint8_t
{
	UNDEFINED,
	CONSTANT,// all vertex the same
	UNIFORM,// each face has the same
	VERTEX_VARYING,// varies in each vertex
	FACE_VARYING,// varies in each face
};

template <typename T>
class AttributeRate
{
public:
	// Constant & Uniform
	AttributeRate(AttributeType attriType = AttributeType::UNIFORM)
		: mType(attriType)
	{
	}
	// Vertex Varying
	AttributeRate(std::vector<T> attriVals)
		: mValueBuffer(std::move(attriVals))
		, mType(AttributeType::VERTEX_VARYING)
	{
	}
	// Face Varying
	AttributeRate(std::vector<T>        attriVals,
				  std::vector<uint32_t> attriIndices)
		: mValueBuffer(std::move(attriVals))
		, mIndexBuffer(std::move(attriIndices))
		, mType(AttributeType::FACE_VARYING)
	{
	}
	~AttributeRate() {}

	bool isConstant() const
	{
		return mType == AttributeType::CONSTANT;
	}
	bool isUniform() const
	{
		return mType == AttributeType::UNIFORM;
	}
	bool isVertexVarying() const
	{
		return mType == AttributeType::VERTEX_VARYING;
	}
	bool isFaceVarying() const
	{
		return mType == AttributeType::FACE_VARYING;
	}

	void getVertexVarying(uint32_t primID, size_t primSize,
						  uint32_t* vIDs, T* targ) const
	{
		for (uint32_t i = 0; i < primSize; i++)
		{
			targ[i] = mValueBuffer[vIDs[i]];
		}
	}
	void getFaceVarying(uint32_t primID, size_t primSize,
						T* targ) const
	{
		uint32_t id = primID * primSize;
		for (uint32_t i = 0; i < primSize; i++)
		{
			targ[i] = mValueBuffer[mIndexBuffer[id + i]];
		}
	}

	void* getValuePtr() const
	{
		return (void*)mValueBuffer.data();
	}
	size_t getValueCount() const
	{
		return mValueBuffer.size();
	}
	// Size in bytes
	size_t getValueByteSize() const
	{
		return sizeof(T) * getValueCount();
	}
	void* getIndexPtr() const
	{
		return (void*)mIndexBuffer.data();
	}
	size_t getIndexCount() const
	{
		return mIndexBuffer.size();
	}
	// Size in bytes
	size_t getIndexByteSize() const
	{
		return sizeof(uint32_t) * getIndexCount();
	}

	std::vector<T>        mValueBuffer;
	std::vector<uint32_t> mIndexBuffer;
	AttributeType         mType;
};

using TextureAttribute = AttributeRate<Point2f>;
using NormalAttribute = AttributeRate<Normal3f>;

}
