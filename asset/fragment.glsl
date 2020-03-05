#version 410 core

layout(location = 0) out vec4 frag_color;

in vec3 out_color;
in vec2 out_texture_coordinate;

uniform sampler2D texture1;

void main()
{
	frag_color = mix(
		texture(texture1, out_texture_coordinate), 
		vec4(out_color, 1), 
		0.3);
}
