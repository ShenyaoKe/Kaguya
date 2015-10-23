#include "Geometry/Mesh.h"
#include "Tracer/Ray.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/Shader.h"
#include "Shading/TextureMapping.h"
#include "Shading/Texture.h"

Mesh::Mesh()
{
	index = Shape::assignIndex();
}
Mesh::Mesh(const char* filename)
{
	index = Shape::assignIndex();
	loadOBJ(filename);
	bounding();
	index = Shape::assignIndex();
	Shape::offsetUID(fids.size());
}
Mesh::~Mesh()
{
	for (int i = 0; i < vertices.size(); ++i)
	{
		delete vertices[i];
	}
	vertices.clear();
	for (int i = 0; i < uvs.size(); ++i)
	{
		delete uvs[i];
	}
	uvs.clear();
	for (int i = 0; i < normals.size(); ++i)
	{
		delete normals[i];
	}
	normals.clear();
}
void Mesh::bounding()
{
	for (int i = 0; i < vertices.size(); ++i)
	{
		ObjBound.Union(*vertices[i]);
	}
}
void Mesh::refine(vector<Shape*> &refined)
{
	for (int i = 0; i < fids.size(); i++)
	{
		Triangle* face = new Triangle(this, i);
		face->index = index + i;
		refined.push_back(face);
	}
}
vector<int> split(const string &s) {
	stringstream ss(s);
	string item;
	vector<int> tokens;
	while (getline(ss, item, ' '))
	{
		stringstream sub(item);
		while (getline(sub, item, '/'))
		{
			if (item == "")
			{
				tokens.push_back(NAN);
			}
			else
			{
				tokens.push_back(stoi(item));

			}
		}
	}
	
	return tokens;
}
bool Mesh::loadOBJ(const char* filename)
{
	clock_t start, end;//Timer
	start = clock();
	//////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/* C Stype                                                              */
	/************************************************************************/
	errno_t err;
	FILE *file;
	err = fopen_s(&file, filename, "r");
	if (err != 0)
	{
		cout << "Unable to open the file!" << endl;
		return false;
	}
	while (true)
	{
		char lineHeader[128];
		int res = fscanf_s(file, "%s", &lineHeader, _countof(lineHeader));
		if (res == EOF)
		{
			break;
		}
		if (strcmp(lineHeader, "v") == 0)
		{
			Point3D* vtx = new Point3D;
			fscanf_s(file, " %lf %lf %lf\n", &vtx->x, &vtx->y, &vtx->z);
			vertices.push_back(vtx);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			Vector2D* uv = new Vector2D;
			fscanf_s(file, " %lf %lf %lf\n", &uv->x, &uv->y);
			uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			Vector3D* normal = new Vector3D;
			fscanf_s(file, " %lf %lf %lf\n", &normal->x, &normal->y, &normal->z);
			normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			TriangleFaceIndex* faceIndex = new TriangleFaceIndex;
			//int faceIndex[3][3];
			// Does not support empty uv or normal
			fscanf_s(file, " %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&faceIndex->vtx[0], &faceIndex->uv[0], &faceIndex->n[0],
				&faceIndex->vtx[1], &faceIndex->uv[1], &faceIndex->n[1],
				&faceIndex->vtx[2], &faceIndex->uv[2], &faceIndex->n[2]);
			// Support empty uv or normal but slow
			/*char line[512];
			fgets(line, sizeof(line), file);
			vector<int> values = split(string(line));
			faceIndex->vtx[0] = values[0]; faceIndex->uv[0] = values[1]; faceIndex->n[0] = values[2];
			faceIndex->vtx[1] = values[3]; faceIndex->uv[1] = values[4]; faceIndex->n[1] = values[5];
			faceIndex->vtx[2] = values[6]; faceIndex->uv[2] = values[7]; faceIndex->n[2] = values[8];*/

			fids.push_back(faceIndex);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}
	fclose(file);

	/************************************************************************/
	/* C++ Style                                                            */
	/************************************************************************/
	/*ifstream file(filename, ios::binary);
	if (!file)
	{
		cout << "Unable to open the file!" << endl;
		return false;
	}
	Float value[3];
	string strs[3];
	while (!file.eof())
	{
		//string curline;
		string lineHeader;
		file >> lineHeader;
		//getline(file, curline);
		if (lineHeader == "v")
		{
			file >> value[0] >> value[1] >> value[2];
			/ *Point3D* vtx = new Point3D(value[0], value[1], value[2]);
			vertices.push_back(vtx);* /
		}
		else if (lineHeader == "vt")
		{
			file >> value[0] >> value[1];
			/ *Vector2D* uv = new Vector2D(value[0], value[1]);
			uvs.push_back(uv);* /
		}
		else if (lineHeader == "vn")
		{
			file >> value[0] >> value[1] >> value[2];
			/ *Vector3D* normal = new Vector3D(value[0], value[1], value[2]);
			normals.push_back(normal);* /
		}
		else if (lineHeader == "f")
		{
			//string vertex[3];
			file >> strs[0] >> strs[1] >> strs[2];
			//TriangleFaceIndex* faceIndex = new TriangleFaceIndex;
			/ *vector<int> v0 = split(strs[0], '/');
			vector<int> v1 = split(strs[1], '/');
			vector<int> v2 = split(strs[2], '/');
			faceIndex->vtx[0] = v0[0]; faceIndex->uv[0] = v0[1]; faceIndex->n[0] = v0[2];
			faceIndex->vtx[1] = v1[0]; faceIndex->uv[1] = v1[1]; faceIndex->n[1] = v1[2];
			faceIndex->vtx[2] = v2[0]; faceIndex->uv[2] = v2[1]; faceIndex->n[2] = v2[2];* /
			//int faceIndex[3][3];
			/ *fscanf_s(file, " %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&faceIndex->vtx[0], &faceIndex->uv[0], &faceIndex->n[0],
				&faceIndex->vtx[1], &faceIndex->uv[1], &faceIndex->n[1],
				&faceIndex->vtx[2], &faceIndex->uv[2], &faceIndex->n[2]);* /
			
			//fids.push_back(faceIndex);
		}
		else
		{
			string trash;
			getline(file, trash);
			// Probably a comment, eat up the rest of the line
			//continue;
		}
	}
	file.close();*/
	//////////////////////////////////////////////////////////////////////////
	end = clock();
	cout << "OBJ File " << filename << " Loding Time:" << (Float)(end - start) / CLOCKS_PER_SEC << "s" << endl;//Timer
	return true;
}
void Mesh::printInfo() const
{
	for (int i = 0; i < vertices.size(); i++)
	{
		cout << "Vertex:\t";
		vertices[i]->printInfo();
	}
	for (int i = 0; i < uvs.size(); i++)
	{
		cout << "UV:\t";
		uvs[i]->printInfo();
	}
	for (int i = 0; i < normals.size(); i++)
	{
		cout << "Normal:\t";
		normals[i]->printInfo();
	}
	for (int i = 0; i < fids.size(); i++)
	{
		cout << "Faces:\t";
		fids[i]->printInfo();
	}
}
bool Mesh::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const
{
	return false;
}
void Mesh::getNormal(const DifferentialGeometry *queryPoint) const
{
}
void Mesh::getUV(const DifferentialGeometry *queryPoint) const
{
}

/*
template <typename vbo_t>
void exportVBO(const Mesh *tri_mesh, int &size,
	vbo_t* &vtx_array, vbo_t* &uv_array, vbo_t* &norm_array)*/
template <typename vbo_t>
void Mesh::exportVBO(int &size,
	vbo_t* &vtx_array, vbo_t* &uv_array, vbo_t* &norm_array, int* &idx_array) const
{
	vbo_t *verts, *texcoord(nullptr), *nms(nullptr);
	int *idxs(nullptr);
	bool has_texcoord(false), has_normal(false);
	size = this->fids.size();
	vtx_array = new vbo_t[size * 9];
	idx_array = new int[size];
	verts = vtx_array;
	idxs = idx_array;
	if (this->fids[0]->uv >= 0)
	{
		uv_array = new vbo_t[size * 6];
		texcoord = uv_array;
		has_texcoord = true;
	}
	else
	{
		uv_array = nullptr;
	}
	if (this->fids[0]->n >= 0)
	{
		norm_array = new vbo_t[size * 9];
		nms = norm_array;
		has_normal = true;
	}
	else
	{
		norm_array = nullptr;
	}
	
	
	for (int i = 0; i < size; i++)
	{
		auto cur_fid = this->fids[i];
		for (int j = 0; j < 3; j++)
		{
			auto cur_vtx = this->vertices[cur_fid->vtx[j] - 1];
			*verts++ = static_cast<vbo_t>(cur_vtx->x);
			*verts++ = static_cast<vbo_t>(cur_vtx->y);
			*verts++ = static_cast<vbo_t>(cur_vtx->z);
		}
		*idxs++ = this->index + i;
		if (has_texcoord)
		{

			for (int j = 0; j < 3; j++)
			{
				auto cur_texcoord = this->uvs[cur_fid->uv[j] - 1];
				*texcoord++ = static_cast<vbo_t>(cur_texcoord->x);
				*texcoord++ = static_cast<vbo_t>(cur_texcoord->y);
			}
		}
		if (has_normal)
		{

			for (int j = 0; j < 3; j++)
			{
				auto cur_normal = this->normals[cur_fid->n[j] - 1];
				*nms++ = static_cast<vbo_t>(cur_normal->x);
				*nms++ = static_cast<vbo_t>(cur_normal->y);
				*nms++ = static_cast<vbo_t>(cur_normal->z);
			}
		}
	}
		
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
Triangle::Triangle()
{
	index = Shape::assignIndex();
	mesh = nullptr;
	p.reserve(3);
	uv.reserve(3);
	n.reserve(3);
}

Triangle::Triangle(Mesh *inMesh, int fn)
{
	this->index = inMesh->index + fn;
	this->attachMesh(inMesh);
	p.reserve(3);
	uv.reserve(3);
	n.reserve(3);

	const TriangleFaceIndex* faceIndex = inMesh->fids[fn];
	if (faceIndex->vtx[0] != NAN)
	{
		this->setPoint(inMesh->vertices[faceIndex->vtx[0] - 1],
			inMesh->vertices[faceIndex->vtx[1] - 1], inMesh->vertices[faceIndex->vtx[2] - 1]);
	}
	/************************************************************************/
	/* UV                                                                   */
	/************************************************************************/
	if (faceIndex->uv[0] != NAN && faceIndex->uv[0] >= 0)
	{
		this->setUV(inMesh->uvs[faceIndex->uv[0] - 1],
			inMesh->uvs[faceIndex->uv[1] - 1], inMesh->uvs[faceIndex->uv[2] - 1]);
	}
	else
	{
		Vector2D* v00 = new Vector2D(0.0, 0.0);
		Vector2D* v10 = new Vector2D(1.0, 0.0);
		Vector2D* v01 = new Vector2D(0.0, 1.0);

		this->setUV(v00, v10, v01);
	}
	/************************************************************************/
	/* Normals                                                              */
	/************************************************************************/
	if (faceIndex->n[0] != NAN && faceIndex->n[0] >= 0)
	{
		this->setNormal(inMesh->normals[faceIndex->n[0] - 1],
			inMesh->normals[faceIndex->n[1] - 1], inMesh->normals[faceIndex->n[2] - 1]);
	}
	else
	{
		Vector3D* n = new Vector3D(Normalize(Cross(*p[2] - *p[1], *p[0] - *p[1])));
		this->setNormal(n, n, n);
	}
	this->bounding();
}
Triangle::~Triangle()
{
//	mesh = nullptr;
// 	p.clear();
// 	uv.clear();
// 	n.clear();
}
void Triangle::bounding()
{
	ObjBound = Union(BBox(*p[0], *p[1]), *p[2]);
}
void Triangle::attachMesh(const Mesh* inMesh)
{
	mesh = inMesh;
}
void Triangle::setPoint(Point3D* p0, Point3D* p1, Point3D* p2)
{
	p.push_back(p0);
	p.push_back(p1);
	p.push_back(p2);
}
void Triangle::setUV(Point2D* uv0, Point2D* uv1, Point2D* uv2)
{
	uv.push_back(uv0);
	uv.push_back(uv1);
	uv.push_back(uv2);
}
void Triangle::setNormal(Vector3D* n0, Vector3D* n1, Vector3D* n2)
{
	n.push_back(n0);
	n.push_back(n1);
	n.push_back(n2);
}
bool Triangle::getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, Float *tHit, Float *rayEpsilon) const
{
	Vector3D v1 = *p[1] - *p[0];
	Vector3D v2 = *p[2] - *p[0];
	Vector3D A = v1.crossMul(v2);
	Vector3D normal = Normalize(A);

	Float rayT;//ray triangle DifferentialGeometry length
	Float cosTheta = normal * inRay.getDir();
	
	rayT = normal * (*p[0] - inRay.getPos()) / cosTheta;
	if (rayT <= 0)
	{
		return false;
	}
	*tHit = rayT;
	
	*rayEpsilon = reCE * *tHit;
	
	//inRay.tmin = rayT;
	Point3D ph = inRay(rayT);
	Vector3D A2 = (*p[0] - ph).crossMul(*p[1] - ph);
	Vector3D A1 = (*p[2] - ph).crossMul(*p[0] - ph);

	int maxIndex = 0;
	for (int i = 1; i < 3; i++)
	{
		maxIndex = abs(A[i]) > abs(A[maxIndex]) ? i : maxIndex;
	}
	Float s = A1[maxIndex] / A[maxIndex], t = A2[maxIndex] / A[maxIndex];
	if (inUnitRange(s) && inUnitRange(t) && inUnitRange(1 - s - t))
	{
		queryPoint->object = this;
		if (n[0] != nullptr)
		{
			queryPoint->normal = Normalize(*n[0] * (1 - s - t) + *n[1] * s + *n[2] * t);
		}
		if (uv[0] != nullptr)
		{
			queryPoint->UV = *uv[0] * (1 - s - t) + *uv[1] * s + *uv[2] * t;
		}
		return true;
	}
	return false;
}
void Triangle::getNormal(const DifferentialGeometry *queryPoint) const
{
	if (mesh->normalMap != nullptr && mesh->UV_Mapping != nullptr)
	{
		ColorRGBA tmpNormal = mesh->normalMap->getColor(queryPoint) * 2 - ColorRGBA(1, 1, 1, 1);
		tmpNormal.printInfo();
		queryPoint->normal = Normalize(
			- queryPoint->uDir * tmpNormal.r
			- queryPoint->vDir * tmpNormal.g
			+ queryPoint->normal * tmpNormal.b);
	}
	else
	{
		Float du1 = uv[0]->x - uv[2]->x;
		Float du2 = uv[1]->x - uv[2]->x;
		Float dv1 = uv[0]->y - uv[2]->y;
		Float dv2 = uv[1]->y - uv[2]->y;
		Vector3D dp1 = *p[0] - *p[2], dp2 = *p[1] - *p[2];
		Float det = du1 * dv2 - dv1 * du2;//determinant
		if (det == 0)
		{
			//Do something
		}
		else
		{
			det = 1.0 / det;
			queryPoint->uDir = Normalize((dv2 * dp1 - dv1 * dp2) * det);
			queryPoint->vDir = Normalize((-du2 * dp1 + du1 * dp2) * det);
		}
	}
}
void Triangle::getUV(const DifferentialGeometry *queryPoint) const
{
}
ColorRGBA Triangle::getColor(const DifferentialGeometry *queryPoint, const Light* light) const
{
	ColorRGBA ret;
	if (material != nullptr)
	{
		ret = material->getColor(queryPoint, light);
	}
	else
	{
		if (mesh->material != nullptr)
		{
			ret = mesh->material->getColor(queryPoint, light);
		}
	}
	return ret;
}

const Point3D& Triangle::closestPoint(const Point3D &point) const
{
	Vector3D ab = *p[1] - *p[0];
	Vector3D ac = *p[2] - *p[0];
	Vector3D ap = point - *p[0];

	// Check if in vertex region outside A
	Float d1 = ab * ap;
	Float d2 = ac * ap;
	if (d1 <= 0.0 && d2 <= 0.0)
	{
		return *p[0];
	}

	// Check if in vertex region outside B
	Vector3D bp = point - *p[1];
	Float d3 = ab * bp;
	Float d4 = ac * bp;
	if (d3 >= 0.0 && d4 <= d3)
	{
		return *p[1];
	}
	// Check if in edge region of AB, return projection on AB
	Float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0 && d1 >= 0.0 && d3 <= 0.0)
	{
		Float v = d1 / (d1 - d3); 
		return *p[0] + v * ab;
	}
	

	// Check if in vertex region outside C
	Vector3D cp = point - *p[2];
	Float d5 = ab * cp;
	Float d6 = ac * cp;
	if (d6 >= 0.0 && d5 <= d6)
	{
		return *p[2];
	}

	// Check if in edge region of AC, return projection on AC
	Float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0 && d2 >= 0.0 && d6 <= 0.0)
	{
		Float w = d2 / (d2 - d6);
		return *p[0] + w * ac;
	}
	// Check if in edge region of BC, return projection on BC
	Float va = d3 * d6 - d5 * d4;
	Vector3D bc = *p[2] - *p[1];
	if (va <= 0.0 && (d4 - d3) >= 0.0 && (d5 - d6) <= 0.0)
	{
		Float w = (d4 - d3) / (d4 - d3 + d5 - d6);
		return *p[0] + w * bc;
	}


	
	Float denom = 1.0 / (va + vb + vc);
	Float v = vb * denom;
	Float w = vc * denom;
	return *p[0] + v * ab + w * ac;	
}

/*
void exportVertices(Triangle* tri_mesh, Float* buffer)
{
	buffer = 
	for (int i = 0; i < 3; i++)
	{
		*buffer++ = triface->p[i]->x;
		*buffer++ = triface->p[i]->y;
		*buffer++ = triface->p[i]->z;
	}
}

void exportTexCoords(Triangle* tri_mesh, Float* buffer)
{
	for (int i = 0; i < 2; i++)
	{
		*buffer++ = triface->uv[i]->x;
		*buffer++ = triface->uv[i]->y;
	}
}

void exportNormals(Triangle* tri_mesh, Float* buffer)
{
	for (int i = 0; i < 3; i++)
	{
		*buffer++ = triface->n[i]->x;
		*buffer++ = triface->n[i]->y;
		*buffer++ = triface->n[i]->z;
	}
}
*/
