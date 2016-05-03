#version 450
in layout(location = 0) vec2 vp;

//uniform mat4 transform;
out vec2 uv;
void main()
{
	gl_Position = vec4(vp, 0., 1.0);
	uv = (vp + 1) * 0.5;
}