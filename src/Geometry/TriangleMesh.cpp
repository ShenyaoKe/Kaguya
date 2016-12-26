#include "Geometry/TriangleMesh.h"
#include "Tracer/Ray.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/Shader.h"
#include "Shading/TextureMapping.h"
#include "Shading/Texture.h"

TriangleMesh::TriangleMesh(const char* filename)
{
	loadOBJ(filename);
	bounding();
}
TriangleMesh::~TriangleMesh()
{
}
void TriangleMesh::bounding()
{
	for (auto &v : verts)
	{
		ObjBound.Union(v);
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
	if (objFileParser::parse(filename, verts, uvs, norms, fids))
	{
		// If triangulated
		bool unitUV_needed = false, unitN_needed = false;
		//size_t nsize = norms.size();
		uint32_t uvsize = uvs.size();
		//size_t nid[3] = { nsize, nsize + 1, nsize + 2 };
		uint32_t uvid[] = { uvsize, uvsize + 1, uvsize + 2 };
		for (auto &f : fids)
		{
			if (f.uv.size() == 0)
			{
				f.uv.insert(f.uv.end(), uvid, uvid + 3);
				unitUV_needed = true;
			}
		}
		if (unitUV_needed)
		{
			uvs.push_back(Point2f(0, 0));
			uvs.push_back(Point2f(1, 0));
			uvs.push_back(Point2f(1, 1));
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
bool TriangleMesh::intersect(const Ray &inRay, DifferentialGeometry* dg,
                             Float* tHit, Float* rayEpsilon) const
{
	return false;
}
void TriangleMesh::postIntersect(const Ray &inRay, DifferentialGeometry* dg) const
{
	// TODO: Implement post-intersection method
}

void TriangleMesh::getBufferObject(BufferTrait* vertTraits,
                                   BufferTrait* vidTraits) const
{
    if (vertTraits)
    {
        vertTraits->data    = (void*)(verts.data());
        vertTraits->count   = verts.size();
        vertTraits->size    = sizeof(Point3f) * vertTraits->count;
        vertTraits->offset  = 0;
        vertTraits->stride  = sizeof(Point3f);
    }
    if (vidTraits)
    {
        vidTraits->data     = (void*)(vids.data());
        vidTraits->count    = vids.size();
        vidTraits->size     = sizeof(uint32_t) * vidTraits->count;
        vidTraits->offset   = 0;
        vidTraits->stride   = sizeof(uint32_t);
    }
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
				auto &cur_vtx = verts[cur_fid.v[j] - 1];
				vtx_array->push_back(static_cast<float>(cur_vtx.x));
				vtx_array->push_back(static_cast<float>(cur_vtx.y));
				vtx_array->push_back(static_cast<float>(cur_vtx.z));
			}
		}
	}
    if (uv_array != nullptr)
    {
        uv_array->reserve(fids.size() * 6);
        for (int i = 0; i < fids.size(); i++)
        {
            auto cur_fid = this->fids[i];
            for (int j = 0; j < 3; j++)
            {
                auto &cur_uv = uvs[cur_fid.uv[j] - 1];
                uv_array->push_back(static_cast<float>(cur_uv.x));
                uv_array->push_back(static_cast<float>(cur_uv.y));
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
				auto &cur_norm = norms[cur_fid.n[j] - 1];
				norm_array->push_back(static_cast<float>(cur_norm.x));
				norm_array->push_back(static_cast<float>(cur_norm.y));
				norm_array->push_back(static_cast<float>(cur_norm.z));
			}
		}
	}
}

void TriangleMesh::exportIndexedVBO(vector<float>* vtx_array,
                                    vector<float>* uv_array,
	                                vector<float>* norm_array,
                                    vector<unsigned int>* idx_array) const
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
		auto &point = verts[i];
		vtx_array->push_back(static_cast<float>(point.x));
		vtx_array->push_back(static_cast<float>(point.y));
		vtx_array->push_back(static_cast<float>(point.z));
	}
	for (int i = 0; i < fids.size(); i++)
	{
		auto &fid = fids[i];
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
}

Triangle::Triangle(TriangleMesh* inMesh, size_t fn)
	: mesh(inMesh), p{ nullptr }, uv{ nullptr }, n{ nullptr }
{

	auto &faceIndex = inMesh->fids[fn];
	if (faceIndex.v.size() > 0)
	{
		this->setPoint(&inMesh->verts[faceIndex.v[0] - 1],
			           &inMesh->verts[faceIndex.v[1] - 1],
                       &inMesh->verts[faceIndex.v[2] - 1]);
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
			            &inMesh->norms[faceIndex.n[1] - 1],
                        &inMesh->norms[faceIndex.n[2] - 1]);
	}
	else
	{
		inMesh->norms.push_back(
			Normal3f(cross(*p[2] - *p[1], *p[0] - *p[1])));
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

bool Triangle::intersect(const Ray &inRay,
	                     DifferentialGeometry* dg,
	                     Float* tHit, Float* rayEpsilon) const
{
	// Moller¨CTrumbore Intersection Algorithm
	Vector3f v1 = *p[1] - *p[0];
	Vector3f v2 = *p[2] - *p[0];
	Vector3f areaVec = cross(v1, v2);
	Vector3f normal = normalize(areaVec);
	
	//ray triangle DifferentialGeometry length
	Float rayt = dot(normal, (*p[0] - inRay.o))
		/ dot(normal, inRay.d);
	if (rayt < inRay.tmin || rayt > inRay.tmax) return false;
	
	//inRay.tmin = rayT;
	Point3f ph = inRay(rayt);
	Vector3f A2 = cross(*p[0] - ph, *p[1] - ph);
	Vector3f A1 = cross(*p[2] - ph, *p[0] - ph);

	int maxIndex = abs(areaVec[0]) > abs(areaVec[1]) ? 0 : 1;
	maxIndex = abs(areaVec[maxIndex]) > abs(areaVec[2]) ? maxIndex : 2;
	// Barycentric coordinate
	Float s = A1[maxIndex] / areaVec[maxIndex];
	Float t = A2[maxIndex] / areaVec[maxIndex];
	Float w = 1.0 - s - t;
	if (!inUnitRange(s) || !inUnitRange(t) || !inUnitRange(w))
	{
		return false;
	}

	*dg = DifferentialGeometry(ph, Normal3f(normal), Vector2f(s, t), this);

	inRay.u = s;
	inRay.v = t;
	*tHit = rayt;
	*rayEpsilon = reCE * *tHit;

	return true;
}

bool Triangle::intersectWatertight(const Ray &inRay,
                                   DifferentialGeometry* dg,
                                   Float* tHit, Float* rayEpsilon) const
{

    Vector3f rd = Vector3f(-10, -6, -11);
    Point3f ro = Point3f(10, 6, 11);
    
    Vector3f vro = Vector3f(ro);
    Point3f v0 = *p[0] - vro;
    Point3f v1 = *p[1] - vro;
    Point3f v2 = *p[2] - vro;

    // Permute Ray
    int kz = maxDimension(abs(rd));
    int kx = kz + 1; if (kx == 3) kx = 0;
    int ky = kx + 1; if (ky == 3) ky = 0;
    Vector3f pmDir = permute(rd, kx, ky, kz);

    // Permute Points
    v0 = permute(v0, kx, ky, kz);
    v1 = permute(v1, kx, ky, kz);
    v2 = permute(v2, kx, ky, kz);
    
    // Shear Ray
    float sz = 1.0f / pmDir.z;
    float sx = -pmDir.x * sz;
    float sy = -pmDir.y * sz;

    // Shear Points
    v0.x += sx * v0.z;
    v0.y += sy * v0.z;
    v1.x += sx * v1.z;
    v1.y += sy * v1.z;
    v2.x += sx * v2.z;
    v2.y += sy * v2.z;

    // area = (p_i+1 - p_i) x (p - p_i), where p = (0, 0)
    //      = p_i x p_i+1 = x_i * y_i+1 - x_i+1 * y_i
    Float e0 = v1.x * v2.y - v2.x * v1.y;
    Float e1 = v2.x * v0.y - v0.x * v2.y;
    Float e2 = v0.x * v1.y - v1.x * v0.y;
    if (e0 < 0 || e1 < 0 || e2 < 0) return false;

    v0.z *= sz;
    v1.z *= sz;
    v2.z *= sz;
    Float det = e0 + e1 + e2;
    Float invDet = 1.0f / det;
    // TODO: finish intersection
    // Barycentric Coordinates
    Float u = e0 * invDet;
    Float v = e1 * invDet;
}

void Triangle::postIntersect(const Ray &inRay,
	                         DifferentialGeometry* dg) const
{
	Vector3f v1 = *p[1] - *p[0];
	Vector3f v2 = *p[2] - *p[0];

	// Compute dpdu, dpdv
	Vector3f dpdu, dpdv;
	Float du1 = uv[1]->x - uv[0]->x;
	Float dv1 = uv[1]->y - uv[0]->y;
	Float du2 = uv[2]->x - uv[0]->x;
	Float dv2 = uv[2]->y - uv[0]->y;
	Float detUV = du1 * dv2 - dv1 * du2;
	if (detUV == 0.0)
	{
		coordinateSystem(Vector3f(dg->Ng), &dg->dPdu, &dg->dPdv);
	}
	else
	{
		Float invDetUV = 1. / detUV;
		dg->dPdu = (v1 * dv2 - v2 * dv1) * invDetUV;
		dg->dPdv = (v2 * du1 - v1 * du2) * invDetUV;
	}

	// Interpolate Texture Coordinates
	Float s = dg->st.x;
	Float t = dg->st.y;
	Float w = 1.0 - s - t;
	dg->uv = *uv[0] * w + *uv[1] * s + *uv[2] * t;
}
void Triangle::getNormal(DifferentialGeometry* queryPoint) const
{
	// TODO: Triangle shading methods
	if (mesh->normalMap != nullptr && mesh->UV_Mapping != nullptr)
	{
		ColorRGBA tmpNormal = mesh->normalMap->getColor(queryPoint) * 2 - ColorRGBA(1, 1, 1, 1);
		tmpNormal.printInfo();
		queryPoint->Ng = normalize(
			- Normal3f(queryPoint->dPdu) * tmpNormal.r
			- Normal3f(queryPoint->dPdv) * tmpNormal.g
			+ queryPoint->Ng * tmpNormal.b);
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
			queryPoint->dPdu = normalize((dp1 * dv2 - dp2 * dv1) * det);
			queryPoint->dPdv = normalize((dp1 * -du2 + dp2 * du1) * det);
		}
	}
}

