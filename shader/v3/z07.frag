#version 330 core

out vec4 frag_color;
in float dist_from_x_axis;
in vec2 vert_pos;

void main() {
	frag_color = vec4(
		vert_pos.x * 20 * dist_from_x_axis, 
		vert_pos.y * 20 * dist_from_x_axis, 
		(vert_pos.x + vert_pos.y) * 20 * dist_from_x_axis, 
		1.0
	);
}