#version 400
layout(location = 0) in vec2 vp;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(vp, 0, 1.0);
	gl_Position.z = -1;
}