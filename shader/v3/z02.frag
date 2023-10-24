#version 330 core

out vec4 frag_color;
uniform vec3 uni_color;

void main() {
	frag_color = vec4(uni_color, 1.0);
}