#version 460 core
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 a_TexCoords;
out vec2 v_TexCoords;

void main()
{
	gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
	v_TexCoords.x = in_Position.x / 2 + 0.5;
	v_TexCoords.y = in_Position.y / 2 + 0.5;
}