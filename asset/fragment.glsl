#version 410 core

layout(location = 0) out vec4 frag_color;

in vec3 out_normal;
in vec2 out_texcoord;

uniform sampler2D texture1;

void main()
{
	vec2 tex_coordinate = out_texcoord;
	frag_color = texture(texture1, tex_coordinate);
}
