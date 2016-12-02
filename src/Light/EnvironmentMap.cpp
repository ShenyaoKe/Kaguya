#include "Light/EnvironmentMap.h"


EnvironmentMap::EnvironmentMap()
{
}

EnvironmentMap::EnvironmentMap(const string &filename)
{
	tex = FileTexture(filename);
	mapping = SphericalMapping2D(Point3f(), Vector3f(0,-1,0), Vector3f(-1,0,-1));
}
EnvironmentMap::~EnvironmentMap()
{
}
ColorRGBA EnvironmentMap::getColor(const Vector3f &dir) const
{
	Point2f uv = mapping.posToUV(Point3f(dir.x, dir.y, dir.z));

	if (uv.x == INFINITY || uv.x == -INFINITY)
	{
		uv.x = 0;
	}
	ColorRGBA ret = tex.getColor(Point2f(uv.x, uv.y));
	return ret;
}