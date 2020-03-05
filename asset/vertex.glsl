#version 410 core

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_texture_coordinate;

out vec3 out_color;
out vec2 out_texture_coordinate;

void main()
{
	out_color = in_color;
	out_texture_coordinate = in_texture_coordinate;
	gl_Position = in_position;
}
