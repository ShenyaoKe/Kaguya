#version 450
#extension GL_ARB_bindless_texture : require

//layout (bindless_sampler) uniform sampler2D textureID;
uniform sampler2D tex;
in vec2 uv;
out vec4 frag_color; // final colour of surface

void main()
{
	frag_color = vec4(texture(tex, uv).rgb, 1);
	//frag_color = vec4(uv, 0, 1);
	//frag_color = vec4(1, 1, 0, 1);
}

