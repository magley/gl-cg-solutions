#version 330 core
layout (location = 0) in vec2 pos;
uniform vec2 uni_offset;
out float dist_from_x_axis;
out vec2 vert_pos;

void main()
{
	gl_Position = vec4(pos + uni_offset, 0.0, 1.0);
	dist_from_x_axis = abs(uni_offset.y) / 2;
	vert_pos = pos;
}