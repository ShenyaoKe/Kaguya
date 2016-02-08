#version 430
//uniform sampler2D modelTex;
uniform mat4 view_matrix;
in vec3 normal;
in vec2 TexCoord;  // From the geometry shader
in vec3 position_eye, normal_eye;

vec3 Kd = vec3(0.6, 0.8, 1);
vec3 La = vec3(0, 0, 0); // grey ambient colour
// fixed point light properties
vec3 light_position_world = vec3(0.0, 100.0, 2.0);


out vec4 frag_color; // final colour of surface

void main()
{
	if (gl_FrontFacing)
	{
		//frag_color = vec4(uid_fs * 0.05, 0., 0, 1.0);
		// ambient intensity
		//vec3 Kd = texture(modelTex, TexCoord).xyz * 1.2;
		// diffuse intensity
		// raise light position to eye space
		vec3 light_position_eye = light_position_world;// vec3(view_matrix * vec4(light_position_world, 1.0));
		vec3 distance_to_light_eye = light_position_eye - position_eye;
		vec3 direction_to_light_eye = normalize(distance_to_light_eye);
		float dot_prod = dot(direction_to_light_eye, normal_eye);
		dot_prod = (dot_prod + 1.0) / 2.0;
		//dot_prod = max(dot_prod, 0.0);
		//dot_prod = ((dot_prod + 1.0) / 2);
		vec3 Id = mix(La, Kd, dot_prod); // final diffuse intensity

		// final colour
		frag_color = vec4(Id, 1.0);
	}
	else
	{
		frag_color = vec4(0, 0, 1, 1.0);
	}
}

