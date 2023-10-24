/*

Napisati program koji crta sarenu traku proizvoljnih boja okolo ivica prozora koji 
je dimenzija 500x500, a na sredini ekrana prvo slovo svog imena proizvoljne boje na
svijetlo sivoj pozadini. Sirina trake iznosi 30% sirine jednog kvadranta. Debljina
linija slova mora biti veæa od 1 piksela. Pozadina se tokom vremena smjenjuje od potpuno
bijele do potpuno crne boje.

*/

#include "v3.h"

#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/shader_util.h"

#define WIN_W 500
#define WIN_H 500

int v3_z03() {
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(WIN_W, WIN_H, "SDL2 >>> GLFW", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glewInit();

	// 1 VBO for a horizontal rect
	// 1 VBO for a vertical rect
	// 1 VBO for a letter.
	
	const float narrow = (1.0f / 6.0f); // width = 1/3 width of a quadrant which is 1/2 of the window so 1/6).
 	const float wide = 1.0f;
	float vertices_hor[] = {
		//	x				y

			+wide,		+narrow,
			+wide,		-narrow,
			-wide,      -narrow,

			+wide,      +narrow,
			-wide,      -narrow,
			-wide,      +narrow

	};
	float vertices_ver[] = {
		//	x				y

			+narrow,		+wide,
			+narrow,		-wide,
			-narrow,        -wide,

			+narrow,        +wide,
			-narrow,        -wide,
			-narrow,        +wide
	};
	float letter[] = {
		+1 / 5.0,	-1 / 5.0,
		-1 / 5.0,   -1 / 5.0,
		-1 / 5.0,   +2 / 5.0,
	};

	enum { HOR, VER, LETTER };
	unsigned int VBO[3];
	unsigned int VAO[3];

	glGenBuffers(3, VBO);
	glGenVertexArrays(3, VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[HOR]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_hor), vertices_hor, GL_STATIC_DRAW);
	glBindVertexArray(VAO[HOR]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[VER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_ver), vertices_ver, GL_STATIC_DRAW);
	glBindVertexArray(VAO[VER]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[LETTER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(letter), letter, GL_STATIC_DRAW);
	glBindVertexArray(VAO[LETTER]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	unsigned shader = create_shader_program("./shader/v3/z02.vert", "./shader/v3/z02.frag");
	unsigned uni_offset = glGetUniformLocation(shader, "uni_offset");
	unsigned uni_color = glGetUniformLocation(shader, "uni_color");

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		float bg_col = sinf(glfwGetTime() / 1.0f);
		glClearColor(bg_col, bg_col, bg_col, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		// Strip color (both strips use the same color).

		float r = sinf(glfwGetTime() / 5.1f);
		float g = cosf(glfwGetTime() / 1.2f + 3.14f);
		float b = sinf(glfwGetTime() / 1.5f + 3.14f);
		glUniform3f(uni_color, r, g, b);

		// Horizontal strips.

		glBindVertexArray(VAO[HOR]);

		glUniform2f(uni_offset, 0.0f, 1 - narrow);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniform2f(uni_offset, 0.0f, -(1 - narrow));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Vertical strips.

		glBindVertexArray(VAO[VER]);

		glUniform2f(uni_offset, 1 - narrow, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniform2f(uni_offset, -(1 - narrow), 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Letter

		glBindVertexArray(VAO[LETTER]);
		glUniform3f(uni_color, 0.0f, 0.0f, 1.0f);
		glLineWidth(5.0);
		glUniform2f(uni_offset, 0.0f, 0.0f);
		glDrawArrays(GL_LINE_STRIP, 0, 3);

		glfwSwapBuffers(win);
	}

	return 0;
}