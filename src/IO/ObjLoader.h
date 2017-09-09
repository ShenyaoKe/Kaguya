#pragma once
#include "Core/Kaguya.h"
#include "Math/Geometry.h"
#include "Geometry/PrimitiveAttribute.h"

namespace Kaguya
{

struct ObjBuffers
{
	// Vertex Buffers
	std::vector<Point3f>  vertexBuffer;
	std::vector<Point2f>  uvBuffer;
	std::vector<Normal3f> normBuffer;
	// Index Buffers
	std::vector<uint32_t> faceIndexBuffer;
	std::vector<uint32_t> uvIndexBuffer;
	std::vector<uint32_t> normIndexBuffer;
	// Face Sides
	std::vector<uint32_t> faceSizeBuffer;
};

class ObjLoader
{
private:
	enum index_t : uint8_t
	{
		V = 1 << 0,
		UV = 1 << 1,
		NORM = 1 << 2,
		VT = V | UV,
		VN = V | NORM,
		VTN = V | UV | NORM
	};

	ObjLoader(const std::string &filename);
	~ObjLoader();

public:
	static std::vector<std::shared_ptr<Primitive>> load(const std::string &filename);
	static std::shared_ptr<TriangleMesh> loadTriangleMesh(const std::string &filename);

	static bool loadRawBuffers(ObjBuffers &outBuffers, const std::string &filename);

private:
	index_t facetype(const char* str, int32_t* val);

	void parse(std::FILE* fp);
	void parseFace(std::FILE* fp);

	void updateBufferRange();
	void clearMeshBuffers();

	void finalizeAttributes();

	std::shared_ptr<Primitive> finalizeMesh();
	std::shared_ptr<TriangleMesh> finalizeTriangeMesh();

private:
	std::FILE*                              fp;
	// Vertex Buffers
	std::vector<Point3f>                    vertexBuffer;
	std::vector<Point2f>                    uvBuffer;
	std::vector<Normal3f>                   normBuffer;
	// Index Buffers
	std::vector<uint32_t>                   faceIndexBuffer;
	std::vector<uint32_t>                   uvIndexBuffer;
	std::vector<uint32_t>                   normIndexBuffer;
	// Face Sides
	std::vector<uint32_t>                   faceSizeBuffer;
	// Face
	uint32_t                                vertRange[2];
	uint32_t                                uvRange[2];
	uint32_t                                normRange[2];
	// Size & Offset
	size_t                                  vertCount;
	size_t                                  uvCount;
	size_t                                  normCount;

	// Attribute
	std::shared_ptr<TextureAttribute>       texAttr;
	std::shared_ptr<NormalAttribute>        normAttr;

	std::vector<std::shared_ptr<Primitive>> mPrims;

	std::string                             mFilename;
};

}

