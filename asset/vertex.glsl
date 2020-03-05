#version 410 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

out vec3 out_normal;
out vec2 out_texcoord;

uniform mat4 perspective;
uniform mat4 view;

void main()
{
	out_normal = in_normal;
	out_texcoord = in_texcoord;
	gl_Position = perspective * view * vec4(in_position, 1.0);
}
