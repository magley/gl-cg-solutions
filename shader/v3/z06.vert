#version 330 core
layout (location = 0) in vec2 pos;
uniform vec2 uni_offset;

void main()
{
	gl_Position = vec4(pos + uni_offset, 0.0, 1.0);
}