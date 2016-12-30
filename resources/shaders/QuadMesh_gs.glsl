#version 450
/*
* Face Geometry Shader
*/
layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 proj_matrix;

out vec3 normal;
out vec3 pos;

void main()
{
	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	vec4 p2 = gl_in[2].gl_Position;
	vec4 p3 = gl_in[3].gl_Position;

	vec4 v1 = normalize(p1 - p0);
	vec4 v2 = normalize(p2 - p0);
	
	// Geometric Normal
	normal = normalize(cross(v1.xyz, v2.xyz));
	
	gl_Position = proj_matrix * p0;
	pos = p0.xyz;
	EmitVertex();

	gl_Position = proj_matrix * p1;
	pos = p1.xyz;
	EmitVertex();
	
	gl_Position = proj_matrix * p3;
	pos = p3.xyz;
	EmitVertex();

	gl_Position = proj_matrix * p2;
	pos = p2.xyz;
	EmitVertex();
}
