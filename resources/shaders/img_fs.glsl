#version 400
#extension GL_ARB_bindless_texture : require

//layout (bindless_sampler) uniform sampler2D textureID;
uniform sampler2D tex;
in vec2 uv;
out vec4 frag_color; // final colour of surface

void main()
{
	frag_color = texture(tex, uv);
}

