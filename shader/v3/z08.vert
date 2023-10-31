#version 330 core

layout (location = 0) in vec2 pos;
uniform vec2 uni_scale;

void main()
{
	vec2 pos_scaled = vec2(pos.x * uni_scale.x, pos.y * uni_scale.y);
	gl_Position = vec4(pos_scaled, 0.0, 1.0);
}