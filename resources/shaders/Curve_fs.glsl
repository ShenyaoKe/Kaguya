#version 400
in vec3 pos;
in vec3 normal;

uniform vec3 Kd = vec3(0.6, 0.8, 1.f);
uniform vec3 La = vec3(0, 0, 0);
uniform vec3 light_pos = vec3(0.f, 1.f, 0.f);

out vec4 frag_color; // final colour of surface

void main()
{
	vec3 dist2eye = light_pos - pos;
	vec3 dir = normalize(dist2eye);
	float dot_prod = dot(dir, normal);
	dot_prod = (dot_prod + 1.f) / 2.0;
	vec3 Id = mix(La, Kd, dot_prod); // final diffuse intensity
	// final color	
	frag_color = vec4(Id, 1.f);
}