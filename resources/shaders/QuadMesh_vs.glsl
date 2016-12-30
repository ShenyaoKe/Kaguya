#version 450
layout(location = 0) in vec3 vp;

uniform mat4 view_matrix, proj_matrix;

void main()
{
	gl_Position = vec4(vp, 1.0);
}