#version 430
in layout(location = 0) vec3 vp;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(vp, 1.0);
	gl_Position.z = -1;
}