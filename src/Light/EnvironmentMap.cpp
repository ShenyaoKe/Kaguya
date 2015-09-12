#include "Light/EnvironmentMap.h"


EnvironmentMap::EnvironmentMap()
{
}

EnvironmentMap::EnvironmentMap(const string& filename)
{
	tex = FileTexture(filename);
	mapping = SphericalMapping2D(Point3D(), Vector3D(0,-1,0), Vector3D(-1,0,-1));
}
EnvironmentMap::~EnvironmentMap()
{
}
ColorRGBA EnvironmentMap::getColor(const Vector3D& dir) const
{
	Point3D uv = mapping.posToUV(dir);
	//uv.printInfo();
	if (uv.x == INFINITY || uv.x == -INFINITY)
	{
		uv.x = 0;
	}
	ColorRGBA ret = tex.getColor(uv);
	return ret;
}