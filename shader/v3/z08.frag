#version 330 core

out vec4 frag_color;
uniform vec3 uni_col;

void main() {
	frag_color = vec4(uni_col, 1.0);
}