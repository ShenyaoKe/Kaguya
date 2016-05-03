#include "Tracer/renderBuffer.h"

/************************************************************************/
/* Render Buffer Function Definition                                    */
/************************************************************************/

renderBuffer::renderBuffer(uint32_t w, uint32_t h)
	: width(w), height(h), size(w * h)
	, p(size * 3), n(size * 3)
	, dpdu(size * 3), dpdv(size * 3)
	, dndu(size * 3), dndv(size * 3)
	, uv(size << 1), z(size), id(size)
{
}

void renderBuffer::resize(uint32_t w, uint32_t h)
{
	width = w;
	height = h;
	size = w * h;
	clear();

	size_t size3 = size * 3;
	size_t size2 = size * 2;

	p.resize(size3);
	n.resize(size3);

	uv.resize(size2);

	dpdu.resize(size3);
	dpdv.resize(size3);
	dndu.resize(size3);
	dndv.resize(size3);

	z.resize(size);
	id.resize(size);
}

void renderBuffer::clear()
{
	p.clear();
	n.clear();

	uv.clear();

	dpdu.clear();
	dpdv.clear();
	dndu.clear();
	dndv.clear();

	z.clear();
	id.clear();
}

bool renderBuffer::empty() const
{
	return p.size() == 0;
}
void renderBuffer::setBuffer(uint32_t x, uint32_t y,
	const DifferentialGeometry &geom, double zdepth)
{
	size_t index = y * width + x;
	if (index >= size) return;

	size_t id3 = index * 3, id2 = index << 1;

	Vec3ToFloats(geom.pos, p, id3);
	Vec3ToFloats(geom.norm, n, id3);
	Vec3ToFloats(geom.dpdu, dpdu, id3);
	Vec3ToFloats(geom.dpdv, dpdv, id3);
	Vec3ToFloats(geom.dndu, dndu, id3);
	Vec3ToFloats(geom.dndv, dndv, id3);

	Vec2ToFloats(geom.uv, uv, id2);
	z[index] = static_cast<float>(zdepth);
	id[index] = geom.shape->shapeID;
}