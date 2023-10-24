/*

Napisati program koji na lijevoj polovini ekrana crta zastavu drzave 
po proizvoljnom izboru, a na desnoj polovini prozora crta zastavu Japana.

*/

#include "v3.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/shader_util.h"
#include <math.h>

#define WIN_W 960
#define WIN_H 540

int v3_z02() {
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(WIN_W, WIN_H, "SDL2 >>> GLFW", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glewInit();

	// All verticies in the buffers assume (0, 0) is the center of their mesh.
	// Because we use glViewport to draw the "left side" and the "right side",
	// this is fine. It's essentially tricking opengl into changing what the
	// "center" is. We could've baked the coordinates in the two buffers below
	// and not use glViewport.

	// One stripe of the flag, taking up 1 width and 1/3 height.
	// Don't use EBO, it's just more code for a marginal optimization.
	// Colors are passed as uniforms (uni_color) because it's easier. 

	const float width = 1.0f;
	const float height = 1 / 3.0f;
	float vertices_flag_stripe[] = {
		//	x				y

			+width,			+height,
			+width,			-height,
			-width,         -height,

			+width,         +height,
			-width,         -height,
			-width,         +height
	};

	unsigned int VBO_flag_stripe, VAO_flag_stripe;
	glGenBuffers(1, &VBO_flag_stripe);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_flag_stripe);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_flag_stripe), vertices_flag_stripe, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_flag_stripe);
	glBindVertexArray(VAO_flag_stripe);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	// Japan's flag has a circle. The following buffer contains vertices for a "circle".
	// To draw (approximate) a circle, draw a regular n-gon, where n is a large number.
	// Intuition (move the slider): https://www.geogebra.org/m/JzSp4dv5

	const float radius = 0.5f;
	const int n = 100;
	float vertices_circle[n * 2] = { 0 }; // n * 2  because each vertex has 2 components (x, y).
	for (int i = 0; i < n; i++) {
		float angle = (360.0 / n) * i;
		float angle_radians = angle * 3.14f / 180;
		float c = cosf(angle_radians);
		float s = sinf(angle_radians);
		vertices_circle[(i * 2) + 0] = 0.0 + c * radius; // x
		vertices_circle[(i * 2) + 1] = 0.0 + s * radius; // y
	}

	unsigned int VBO_circle, VAO_circle;
	glGenBuffers(1, &VBO_circle);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_circle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circle), vertices_circle, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_circle);
	glBindVertexArray(VAO_circle);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	unsigned shader = create_shader_program("./shader/v3/z02.vert", "./shader/v3/z02.frag");
	unsigned uni_offset = glGetUniformLocation(shader, "uni_offset"); // This is like the position of individual objects.
	unsigned uni_color = glGetUniformLocation(shader, "uni_color");

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0); // Because we clear with "white", we
		// don't have to draw the 3rd stripe, or the the back part of Japan's flag.
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		// Left side.

		glViewport(0, 0, WIN_W / 2, WIN_H);
		glBindVertexArray(VAO_flag_stripe);

		glUniform2f(uni_offset, 0.0f, height * 2);
		glUniform3f(uni_color, 1.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6); // Red stripe

		glUniform2f(uni_offset, 0.0f, 0);
		glUniform3f(uni_color, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6); // Blue stripe

		// Right side.

		glViewport(WIN_W / 2, 0, WIN_W / 2, WIN_H);
		glBindVertexArray(VAO_circle);

		glUniform2f(uni_offset, 0, 0);
		glUniform3f(uni_color, 1.0f, 0.0f, 0.0f);
		glDrawArrays(GL_POLYGON, 0, n); // !!


		glfwSwapBuffers(win);
	}

	return 0;
}