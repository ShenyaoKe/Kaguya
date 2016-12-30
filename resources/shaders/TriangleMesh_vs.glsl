#version 450
layout(location = 0) in vec3 vp;

uniform mat4 view_matrix;

void main()
{
	gl_Position = view_matrix * vec4(vp, 1.0);
}