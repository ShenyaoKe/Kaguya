#include "PolyMesh.h"
#include "Geometry/TriangleMesh.h"
#include "Geometry/QuadMesh.h"

namespace Kaguya
{

PolyMesh::PolyMesh(std::vector<Point3f>              vertexBuffer,
				   std::vector<uint32_t>             indexBuffer,
				   size_t                            vertexCount,
				   size_t                            faceCount,
				   std::shared_ptr<TextureAttribute> texAttri,
				   std::shared_ptr<NormalAttribute>  normAttri)
	: mVertexBuffer(std::move(vertexBuffer))
	, mIndexBuffer(std::move(indexBuffer))
	, mVertexCount(vertexCount)
	, mFaceCount(faceCount)
	, mTextureAttribute(texAttri)
	, mNormalAttibute(normAttri)
{
}

PolyMesh::~PolyMesh()
{
}

void PolyMesh::getRenderBuffer(RenderBufferTrait* trait) const
{
	switch (polyMeshType())
	{
	case PolyMeshType::TRIANGLE:
		trait->renderType = GPURenderType::TRIANGLE;
		break;
	case PolyMeshType::QUAD:
		trait->renderType = GPURenderType::QUAD;
		break;
	default:
		break;
	}
	// attach vertex buffer
	trait->vertex.data = (void*)(mVertexBuffer.data());
	trait->vertex.count = mVertexBuffer.size();
	trait->vertex.size = sizeof(Point3f) * trait->vertex.count;
	trait->vertex.offset = 0;
	trait->vertex.stride = sizeof(Point3f);

	// attach index buffer
	trait->index.data = (void*)(mIndexBuffer.data());
	trait->index.count = mIndexBuffer.size();
	trait->index.size = sizeof(uint32_t) * trait->index.count;
	trait->index.offset = 0;
	trait->index.stride = sizeof(uint32_t);

	// Extract TextureCoordinates to Texture Buffer
	switch (mTextureAttribute->mType)
	{
	case AttributeType::VERTEX_VARYING:
	{
		trait->texAttriType = AttributeType::VERTEX_VARYING;
		trait->texcoords.attriData = mTextureAttribute->getValuePtr();
		trait->texcoords.dataSize = mTextureAttribute->getValueByteSize();
		break;
	}
	case AttributeType::FACE_VARYING:
	{
		trait->texAttriType = AttributeType::FACE_VARYING;
		trait->texcoords.attriData = mTextureAttribute->getValuePtr();
		trait->texcoords.dataSize = mTextureAttribute->getValueByteSize();
		trait->texcoords.attriIndex = mTextureAttribute->getIndexPtr();
		trait->texcoords.indexSize = mTextureAttribute->getIndexByteSize();
		break;
	}
	default:
		break;
	}
	// Extract Shading Normal to Texture Buffer
	switch (mNormalAttibute->mType)
	{
	case AttributeType::VERTEX_VARYING:
	{
		trait->normAttriType = AttributeType::VERTEX_VARYING;
		trait->normal.attriData = mNormalAttibute->getValuePtr();
		trait->normal.dataSize = mNormalAttibute->getValueByteSize();
		break;
	}
	case AttributeType::FACE_VARYING:
	{
		trait->normAttriType = AttributeType::FACE_VARYING;
		trait->normal.attriData = mNormalAttibute->getValuePtr();
		trait->normal.dataSize = mNormalAttibute->getValueByteSize();
		trait->normal.attriIndex = mNormalAttibute->getIndexPtr();
		trait->normal.indexSize = mNormalAttibute->getIndexByteSize();
		break;
	}
	default:
		break;
	}
}

size_t PolyMesh::tessellatedCount(const std::vector<uint32_t> &faceSizeBuffer, size_t faceSize)
{
	switch (faceSize)
	{
	case 3:
	{
		return std::accumulate(faceSizeBuffer.begin(),
							   faceSizeBuffer.end(),
							   (size_t)0,
							   [](size_t sum, uint32_t curSize) { return sum + curSize - 2; });
	}
	case 4:
	{
		return std::accumulate(faceSizeBuffer.begin(),
							   faceSizeBuffer.end(),
							   (size_t)0,
							   [](size_t sum, uint32_t curSize) { return sum + ((curSize - 1) >> 1); });
	}
	default:
	{
		return 0;
	}
	}
}

std::shared_ptr<PolyMesh> PolyMesh::createPolyMesh(std::vector<Point3f>              vertexBuffer,
												   std::vector<uint32_t>             indexBuffer,
												   const std::vector<uint32_t>      &faceSizeBuffer,
												   std::shared_ptr<TextureAttribute> texAttri,
												   std::shared_ptr<NormalAttribute>  normAttri)
{
	size_t triCount = tessellatedCount(faceSizeBuffer, TriangleMesh::getFaceSize());
	size_t quadCount = tessellatedCount(faceSizeBuffer, QuadMesh::getFaceSize());

	if (triCount * 3 < quadCount * 4)
	{
		return std::make_shared<TriangleMesh>(std::move(vertexBuffer),
											  std::move(indexBuffer),
											  faceSizeBuffer,
											  triCount,
											  texAttri,
											  normAttri,
											  triCount == faceSizeBuffer.size());
	}
	else
	{
		return std::make_shared<QuadMesh>(std::move(vertexBuffer),
										  std::move(indexBuffer),
										  faceSizeBuffer,
										  quadCount,
										  texAttri,
										  normAttri,
										  quadCount == faceSizeBuffer.size());
	}
}

std::shared_ptr<TriangleMesh> PolyMesh::createTriMesh(std::vector<Point3f>              vertexBuffer,
													  std::vector<uint32_t>             indexBuffer,
													  const std::vector<uint32_t>      &faceSizeBuffer,
													  std::shared_ptr<TextureAttribute> texAttri,
													  std::shared_ptr<NormalAttribute>  normAttri)
{
	size_t triCount = tessellatedCount(faceSizeBuffer, TriangleMesh::getFaceSize());

	return std::make_shared<TriangleMesh>(std::move(vertexBuffer),
										  std::move(indexBuffer),
										  faceSizeBuffer,
										  triCount,
										  texAttri,
										  normAttri,
										  triCount == faceSizeBuffer.size());
}
}
