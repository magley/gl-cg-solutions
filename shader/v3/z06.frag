#version 330 core

out vec4 frag_color;
uniform vec2 uni_win_size;

void main() {
	// gl_FragCoord is in screen coordinates, so
	// e.g. 0-1280 and 0-720, but we want (-1, 1).
	// It's just a slope intercept formula:

	float hw = uni_win_size.x / 2;
	float hh = uni_win_size.y / 2;
	float cx = (gl_FragCoord.x - hw) / hw;
	float cy = (gl_FragCoord.y - hh) / hh;

	// Determine color based on quadrant.

	frag_color = vec4(1, 1, 1, 1);

	if (cx > 0 && cy > 0) {
		frag_color = vec4(1, 0, 0, 1);
	}
	else if (cx > 0 && cy < 0) {
		frag_color = vec4(1, 0, 1, 1);
	}
	else if (cx < 0 && cy < 0) {
		frag_color = vec4(0, 0, 1, 1);
	}

	// Stripes

	if (cy < 0) {
		// We use gl_FragCoord.x here because it's easier.

		float cx_mod = mod(gl_FragCoord.x, 20);
		if (cx_mod >= 0 && cx_mod <= 10) {
			discard;
		}
	}
}