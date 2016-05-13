#include "Geometry/TriangleMesh.h"
#include "Tracer/Ray.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/Shader.h"
#include "Shading/TextureMapping.h"
#include "Shading/Texture.h"
/*

Mesh::Mesh()
{
	index = Shape::assignIndex();
}*/
TriangleMesh::TriangleMesh(const char* filename)
{
	//index = Shape::assignIndex();
	loadOBJ(filename);
	bounding();
	/*index = Shape::assignIndex();
	Shape::offsetUID(fids.size());*/
}
TriangleMesh::~TriangleMesh()
{
	
}
void TriangleMesh::bounding()
{
	for (int i = 0; i < verts.size(); ++i)
	{
		ObjBound.Union(verts[i]);
	}
}
void TriangleMesh::refine(vector<Shape*> &refined)
{
	for (int i = 0; i < fids.size(); i++)
	{
		Triangle* face = new Triangle(this, i);
		//face->shapeId = shapeId + i;
		refined.push_back(face);
	}
}
bool TriangleMesh::loadOBJ(const char* filename)
{
	//vector<PolyIndex> fids;
	if (ObjParser::parse(filename, verts, uvs, norms, fids))
	{
		// If triangulated
		bool unitUV_needed = false, unitN_needed = false;
		//size_t nsize = norms.size();
		size_t uvsize = uvs.size();
		//size_t nid[3] = { nsize, nsize + 1, nsize + 2 };
		size_t uvid[3] = { uvsize, uvsize + 1, uvsize + 2 };
		for (auto f : fids)
		{
			if (f.uv.size() == 0)
			{
				f.uv.insert(f.uv.end(), uvid, uvid + 3);
				unitUV_needed = true;
			}
		}
		if (unitUV_needed)
		{
			uvs.push_back(Point2f(0., 0.));
			uvs.push_back(Point2f(1., 0.));
			uvs.push_back(Point2f(1., 1.));
		}
		// Triangulation required!
		return true;
	}

	return false;
}
void TriangleMesh::printInfo(const string &msg) const
{
	if (!msg.empty())
	{
		cout << msg << endl;
	}
	for (int i = 0; i < verts.size(); i++)
	{
		cout << "Vertex:\t" << verts[i] << endl;
	}
	for (int i = 0; i < uvs.size(); i++)
	{
		cout << "UV:\t" << uvs[i] << endl;
	}
	for (int i = 0; i < norms.size(); i++)
	{
		cout << "Normal:\t" << norms[i] << endl;
	}
	for (int i = 0; i < fids.size(); i++)
	{
		cout << "Faces:\t";
		fids[i].printInfo();
	}
}
bool TriangleMesh::intersect(const Ray& inRay, DifferentialGeometry* queryPoint, Float *tHit, Float *rayEpsilon) const
{
	return false;
}
void TriangleMesh::exportVBO(
	vector<float>* vtx_array,
	vector<float>* uv_array,
	vector<float>* norm_array) const
{
	if (vtx_array != nullptr)
	{
		vtx_array->reserve(fids.size() * 9);
		for (int i = 0; i < fids.size(); i++)
		{
			auto cur_fid = fids[i];
			for (int j = 0; j < 3; j++)
			{
				auto& cur_vtx = verts[cur_fid.v[j] - 1];
				vtx_array->push_back(static_cast<float>(cur_vtx.x));
				vtx_array->push_back(static_cast<float>(cur_vtx.y));
				vtx_array->push_back(static_cast<float>(cur_vtx.z));
			}
		}
	}
	if (norm_array != nullptr)
	{
		norm_array->reserve(fids.size() * 9);
		for (int i = 0; i < fids.size(); i++)
		{
			auto cur_fid = this->fids[i];
			for (int j = 0; j < 3; j++)
			{
				auto& cur_norm = norms[cur_fid.n[j] - 1];
				norm_array->push_back(static_cast<float>(cur_norm.x));
				norm_array->push_back(static_cast<float>(cur_norm.y));
				norm_array->push_back(static_cast<float>(cur_norm.z));
			}
		}
	}
}

void TriangleMesh::exportIndexedVBO(
	vector<float>* vtx_array, vector<float>* uv_array,
	vector<float>* norm_array, vector<unsigned int>* idx_array) const
{
	bool has_vert(false), has_texcoord(false), has_normal(false), has_uid(false);
	
	if (vtx_array != nullptr)
	{
		vtx_array->clear();
		vtx_array->reserve(verts.size() * 3);
		has_vert = true;
	}
	/*if (this->fids[0]->uv >= 0 && uv_array != nullptr)
	{
		*uv_array = new vbo_t[size * 6];
		texcoord = *uv_array;
		has_texcoord = true;
	}
	if (this->fids[0]->n >= 0 && norm_array != nullptr)
	{
		*norm_array = new vbo_t[size * 9];
		nms = *norm_array;
		has_normal = true;
	}*/
	if (idx_array != nullptr)
	{
		idx_array->clear();
		idx_array->reserve(fids.size() * 3);
		has_uid = true;
	}

	for (int i = 0; i < verts.size(); i++)
	{
		auto& point = verts[i];
		vtx_array->push_back(static_cast<float>(point.x));
		vtx_array->push_back(static_cast<float>(point.y));
		vtx_array->push_back(static_cast<float>(point.z));
	}
	for (int i = 0; i < fids.size(); i++)
	{
		auto& fid = fids[i];
		idx_array->push_back(static_cast<uint32_t>(fid.v[0] - 1));
		idx_array->push_back(static_cast<uint32_t>(fid.v[1] - 1));
		idx_array->push_back(static_cast<uint32_t>(fid.v[2] - 1));
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
Triangle::Triangle()
	: mesh(nullptr), p{ nullptr }, uv{ nullptr }, n{ nullptr }
{
	//shapeId = Shape::assignIndex();
	mesh = nullptr;
}

Triangle::Triangle(TriangleMesh *inMesh, size_t fn)
	: mesh(inMesh), p{ nullptr }, uv{ nullptr }, n{ nullptr }
{

	auto& faceIndex = inMesh->fids[fn];
	if (faceIndex.v.size() > 0)
	{
		this->setPoint(&inMesh->verts[faceIndex.v[0] - 1],
			&inMesh->verts[faceIndex.v[1] - 1], &inMesh->verts[faceIndex.v[2] - 1]);
	}
	/************************************************************************/
	/* UV                                                                   */
	/************************************************************************/
	if (faceIndex.uv.size() > 0)
	{
		this->setUV(&inMesh->uvs[faceIndex.uv[0] - 1],
			&inMesh->uvs[faceIndex.uv[1] - 1], &inMesh->uvs[faceIndex.uv[2] - 1]);
	}
	/************************************************************************/
	/* Normals                                                              */
	/************************************************************************/
	if (faceIndex.n.size() > 0)
	{
		this->setNormal(&inMesh->norms[faceIndex.n[0] - 1],
			&inMesh->norms[faceIndex.n[1] - 1], &inMesh->norms[faceIndex.n[2] - 1]);
	}
	else
	{
		inMesh->norms.push_back(
			Normal3f(Cross(*p[2] - *p[1], *p[0] - *p[1])));
		auto &n = inMesh->norms.back();
		this->setNormal(&n, &n, &n);
	}
	this->bounding();
}
void Triangle::bounding()
{
	ObjBound = Union(Bounds3f(*p[0], *p[1]), *p[2]);
}
void Triangle::attachMesh(const TriangleMesh* inMesh)
{
	mesh = inMesh;
}
void Triangle::setPoint(Point3f* p0, Point3f* p1, Point3f* p2)
{
	p[0] = p0;
	p[1] = p1;
	p[2] = p2;
}
void Triangle::setUV(Point2f* uv0, Point2f* uv1, Point2f* uv2)
{
	uv[0] = uv0;
	uv[1] = uv1;
	uv[2] = uv2;
}
void Triangle::setNormal(Normal3f* n0, Normal3f* n1, Normal3f* n2)
{
	n[0] = n0;
	n[0] = n1;
	n[0] = n2;
}
bool Triangle::intersect(const Ray& inRay,
	DifferentialGeometry* queryPoint,
	Float *tHit, Float *rayEpsilon) const
{
	Vector3f v1 = *p[1] - *p[0];
	Vector3f v2 = *p[2] - *p[0];
	Vector3f areaVec = Cross(v1, v2);
	Vector3f normal = Normalize(areaVec);

	Float rayT;//ray triangle DifferentialGeometry length
	Float cosTheta = Dot(normal, inRay.d);
	
	rayT = Dot(normal, (*p[0] - inRay.o)) / cosTheta;
	if (rayT < inRay.tmin || rayT > inRay.tmax)
	{
		return false;
	}
	*tHit = rayT;
	
	*rayEpsilon = reCE * *tHit;
	
	//inRay.tmin = rayT;
	Point3f ph = inRay(rayT);
	Vector3f A2 = Cross(*p[0] - ph, *p[1] - ph);
	Vector3f A1 = Cross(*p[2] - ph, *p[0] - ph);

	int maxIndex = abs(areaVec[0]) > abs(areaVec[1]) ? 0 : 1;
	maxIndex = abs(areaVec[maxIndex]) > abs(areaVec[2]) ? maxIndex : 2;
	// Barycentric coordinate
	Float s = A1[maxIndex] / areaVec[maxIndex], t = A2[maxIndex] / areaVec[maxIndex];
	Float w = 1. - s - t;
	if (!inUnitRange(s) || !inUnitRange(t) || !inUnitRange(w))
	{
		return false;
	}

	// Compute dpdu, dpdv
	Vector3f dpdu, dpdv;
	Float du1 = uv[1]->x - uv[0]->x;
	Float dv1 = uv[1]->y - uv[0]->y;
	Float du2 = uv[2]->x - uv[0]->x;
	Float dv2 = uv[2]->y - uv[0]->y;
	Float detUV = du1 * dv2 - dv1 * du2;
	if (detUV == 0.)
	{
		CoordinateSystem(normal, &dpdu, &dpdv);
	}
	Float invDetUV = 1. / detUV;
	dpdu = (v1 * dv2 - v2 * dv1) * invDetUV;
	dpdv = (v2 * du1 - v1 * du2) * invDetUV;

	// Interpolate UV parametric coordinates
	Point2f tuv = *uv[0] * w + *uv[1] * s + *uv[2] * t;

	*queryPoint = DifferentialGeometry(ph, Normal3f(normal), dpdu, dpdv, Normal3f(), Normal3f(), tuv, this);
	return true;
}
void Triangle::getNormal(DifferentialGeometry* queryPoint) const
{
	if (mesh->normalMap != nullptr && mesh->UV_Mapping != nullptr)
	{
		ColorRGBA tmpNormal = mesh->normalMap->getColor(queryPoint) * 2 - ColorRGBA(1, 1, 1, 1);
		tmpNormal.printInfo();
		queryPoint->norm = Normalize(
			- Normal3f(queryPoint->dpdu) * tmpNormal.r
			- Normal3f(queryPoint->dpdv) * tmpNormal.g
			+ queryPoint->norm * tmpNormal.b);
	}
	else
	{
		Float du1 = uv[0]->x - uv[2]->x;
		Float du2 = uv[1]->x - uv[2]->x;
		Float dv1 = uv[0]->y - uv[2]->y;
		Float dv2 = uv[1]->y - uv[2]->y;
		Vector3f dp1 = *p[0] - *p[2], dp2 = *p[1] - *p[2];
		Float det = du1 * dv2 - dv1 * du2;//Determinant
		if (det == 0)
		{
			//Do something
		}
		else
		{
			det = 1.0 / det;
			queryPoint->dpdu = Normalize((dp1 * dv2 - dp2 * dv1) * det);
			queryPoint->dpdv = Normalize((dp1 * -du2 + dp2 * du1) * det);
		}
	}
}

bool ObjParser::parse(const char* filename,
	vector<Point3f> &verts,
	vector<Point2f> &uvs, vector<Normal3f> &norms,
	vector<PolyIndex> &polys)
{
#ifdef _DEBUG
	clock_t start, end;//Timer
	start = clock();
#endif
	
	FILE* fp = fopen(filename, "r");
	if (fp == nullptr)
	{
		return false;
	}
	int err;
	char buff[256] = {};
	char lineHeader[3] = {};
	double val[3] = {};
	uint32_t indices[3];
	char endflg;

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
			fscanf(fp, "%lf %lf %lf\n", val, val + 1, val + 2);
			verts.push_back(Point3f(val));
		}
		// Texture Coordinate
		else if (strcmp(lineHeader, "vt") == 0)
		{
			fscanf(fp, "%lf %lf\n", val, val + 1);
			uvs.push_back(Point2f(val));
		}
		// Vertex Normal
		else if (strcmp(lineHeader, "vn") == 0)
		{
			//float val[3];
			fscanf(fp, "%lf %lf %lf\n", val, val + 1, val + 2);
			norms.push_back(Normal3f(val));
		}
		// Face Index
		else if (strcmp(lineHeader, "f") == 0)
		{
			PolyIndex fid;
			err = fscanf(fp, "%s", &buff);
			indices[1] = indices[2] = 0;
			index_t ft = facetype(buff, indices);
			fid.push_back(indices);
			endflg = fgetc(fp);
			switch (ft)
			{
			case VTN://111
				while (endflg != '\n' && endflg != '\r' && endflg != '\0')
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d/%d/%d", indices, indices + 1, indices + 2);
					fid.push_back(indices);
					endflg = fgetc(fp);
				}
				break;
			case VT://011
				while (endflg != '\n' && endflg != '\r' && endflg != '\0')
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d/%d", indices, indices + 1);
					fid.push_back(indices);
					endflg = fgetc(fp);
				}
				break;
			case VN://101
				while (endflg != '\n' && endflg != '\r' && endflg != '\0')
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d//%d", indices, indices + 2);
					fid.push_back(indices);
					endflg = fgetc(fp);
				}
				break;
			case V://001
				while (endflg != '\n' && endflg != '\r' && endflg != EOF)
				{
					ungetc(endflg, fp);
					fscanf(fp, "%d", indices);
					fid.push_back(indices);
					endflg = fgetc(fp);
				}
				break;
			default:
				break;
			}
			fid.size = fid.v.size();
			polys.push_back(fid);
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

#ifdef _DEBUG
	end = clock();
	cout << "OBJ File " << filename << " Loding Time:" << (Float)(end - start) / CLOCKS_PER_SEC << "s" << endl;//Timer
#endif // _DEBUG

	return true;
}
