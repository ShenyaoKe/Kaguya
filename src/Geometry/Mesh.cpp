#include "Mesh.h"
#include "Core/Utils.h"
#include "Geometry/PolyMesh.h"

namespace Kaguya
{

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool objFileParser::parse(const char*            filename,
						  std::vector<Point3f>  &verts,
						  std::vector<Point2f>  &uvs,
						  std::vector<Normal3f> &norms,
						  std::vector<uint32_t> &faceId,
						  std::vector<uint32_t> &texcoordId,
						  std::vector<uint32_t> &normId,
						  std::vector<uint32_t> &faceCount)
{
	std::FILE* fp = std::fopen(filename, "r");
	if (fp == nullptr)
	{
		return false;
	}
	int err;
	char buff[256] = {};
	char lineHeader[3] = {};
	Float val[3] = {};
	int32_t indices[3];
	char endflg;
	size_t vertCount = 0;
	size_t texcoordCount = 0;
	size_t normCount = 0;

	while (true)
	{
		lineHeader[0] = lineHeader[1] = 0;
		err = fscanf(fp, "%2s", &lineHeader);
		if (err == EOF)
		{
			break;
		}
		// Vertex
		if (strcmp(lineHeader, "v") == 0)
		{
			fscanf(fp, "%f %f %f\n", val, val + 1, val + 2);
			verts.emplace_back(val);
			vertCount++;
		}
		// Texture Coordinate
		else if (strcmp(lineHeader, "vt") == 0)
		{
			fscanf(fp, "%f %f\n", val, val + 1);
			uvs.emplace_back(val);
			texcoordCount++;
		}
		// Vertex Normal
		else if (strcmp(lineHeader, "vn") == 0)
		{
			//float val[3];
			fscanf(fp, "%f %f %f\n", val, val + 1, val + 2);
			norms.emplace_back(val);
			normCount++;
		}
		// Face Index
		else if (strcmp(lineHeader, "f") == 0)
		{
			//PolyIndex fid;
			err = fscanf(fp, "%s", &buff);
			indices[1] = indices[2] = 0;
			index_t ft = facetype(buff, indices);
			auto addIndex = [](std::vector<uint32_t> &indices,
							   size_t bufferSize, int32_t id)
			{
				indices.push_back(id > 0 ? id - 1 : bufferSize + id);
			};

			addIndex(faceId, vertCount, indices[0]);
			int count = 1;
			endflg = fgetc(fp);
			switch (ft)
			{
			case VTN://111
				addIndex(texcoordId, texcoordCount, indices[1]);
				addIndex(normId, normCount, indices[2]);
				while (endflg != '\n' && endflg != '\r' && endflg != '\0')
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d/%d/%d", indices, indices + 1, indices + 2);
					addIndex(faceId, vertCount, indices[0]);
					addIndex(texcoordId, texcoordCount, indices[1]);
					addIndex(normId, normCount, indices[2]);
					count++;
					endflg = fgetc(fp);
				}
				faceCount.push_back(count);
				break;
			case VT://011
				texcoordId.push_back(indices[1] - 1);
				while (endflg != '\n' && endflg != '\r' && endflg != '\0')
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d/%d", indices, indices + 1);
					addIndex(faceId, vertCount, indices[0]);
					addIndex(texcoordId, texcoordCount, indices[1]);
					count++;
					endflg = fgetc(fp);
				}
				faceCount.push_back(count);
				break;
			case VN://101
				normId.push_back(indices[2] - 1);
				while (endflg != '\n' && endflg != '\r' && endflg != '\0')
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d//%d", indices, indices + 2);
					addIndex(faceId, vertCount, indices[0]);
					addIndex(normId, normCount, indices[2]);
					count++;
					endflg = fgetc(fp);
				}
				faceCount.push_back(count);
				break;
			case V://001
				while (endflg != '\n' && endflg != '\r' && endflg != EOF)
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d", indices);
					addIndex(faceId, vertCount, indices[0]);
					count++;
					endflg = fgetc(fp);
				}
				faceCount.push_back(count);
				break;
			default:
				break;
			}
		}
		// Comment
		else if (strcmp(lineHeader, "#") == 0)
		{
			fscanf(fp, "%[^\r\n]", &buff);
		}
		// Others
		else
		{
			// skip everything except \n or \r
			fscanf(fp, "%[^\r\n]", &buff);
		}
	}
	fclose(fp);
	return true;
}

Mesh* createMesh(const std::string &filename, MeshType meshType)
{
	std::vector<Point3f>  vertexBuffer;
	std::vector<Point2f>  textureCoords;
	std::vector<Normal3f> norms;
	std::vector<uint32_t> faceIndexBuffer;
	std::vector<uint32_t> texcoordsIndexBuffer;
	std::vector<uint32_t> normIndexBuffer;
	std::vector<uint32_t> faceCount;
	TextureAttribute*     texAttr;
	NormalAttribute*      normAttr;
	if (Utils::endsWith(filename, "obj"))
	{
		if (!objFileParser::parse(filename.c_str(),
								  vertexBuffer,
								  textureCoords,
								  norms,
								  faceIndexBuffer,
								  texcoordsIndexBuffer,
								  normIndexBuffer,
								  faceCount))
		{
			return nullptr;
		}
	}
	/*else if (Utils::endsWith(filename, "ply"))
	{
	}*/
	texAttr = texcoordsIndexBuffer.size() == faceIndexBuffer.size()
		? new TextureAttribute(textureCoords, texcoordsIndexBuffer)
		: new TextureAttribute;
	normAttr = normIndexBuffer.size() == faceIndexBuffer.size()
		? new NormalAttribute(norms, normIndexBuffer)
		: new NormalAttribute;
	if (meshType == MeshType::POLYGONAL_MESH)
	{
		return PolyMesh::createPolyMesh(vertexBuffer,
										faceIndexBuffer,
										faceCount,
										std::shared_ptr<TextureAttribute>(texAttr),
										std::shared_ptr<NormalAttribute>(normAttr));
	}
	else if (meshType == MeshType::SUBDIVISION_MESH)
	{

	}

	return nullptr;
}

}
