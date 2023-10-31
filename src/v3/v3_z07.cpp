/*

Nacrtati dva sarena trougla, jedan iznad x ose, jedan ispod x ose, trouglovi se
tokom vremena krecu po nevidljivoj kruznici dovoljno malog poluprecnika tako da
trouglovi ne prelaze x osu. Trougao iznad x ose se krece u smeru kazaljke na satu,
dok se trougao ispod x ose krece u suprotnom smeru. Boja touglova postaje
svetlija/tamnija što je trougao dalji/blizi x osi.

*/


#include "v3.h"

#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/shader_util.h"

#define WIN_W 600
#define WIN_H 600

int v3_z07() {
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(WIN_W, WIN_H, "SDL2 >>> GLFW", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glfwSwapInterval(1);
	glewInit();

	const float width = 0.25f / 2;
	const float height = 0.25f / 2;
	float vertices[] = {
		//	x				y

			-width,			-height,
			+width,			-height,
			 0.0f,			 height,
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	unsigned shader = create_shader_program("./shader/v3/z07.vert", "./shader/v3/z07.frag");
	unsigned uni_offset = glGetUniformLocation(shader, "uni_offset");
	unsigned uni_win_size = glGetUniformLocation(shader, "uni_win_size");

	float x1 = 0, y1 = -0.5;
	float x2 = 0, y2 = +0.5;

	int tick = 0;
	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(win, 1);
		}

		tick++;

		// ---------- Game logic --------------------------------

		x1 -= cosf(tick / 25.0f) * 0.02 / 2.5;
		y1 -= sinf(tick / 25.0f) * 0.02 / 2.5;

		x2 += cosf(tick / 25.0f) * 0.02 / 2.5;
		y2 += sinf(tick / 25.0f) * 0.02 / 2.5;

		// ---------- Rendering --------------------------------

		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		//glUniform2f(uni_win_size, WIN_W, WIN_H); // Could be extracted outside the loop if WIN_W and WIN_H never change.

		// Triangle 1

		glBindVertexArray(VAO);
		glUniform2f(uni_offset, x1, y1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Triangle 2

		glBindVertexArray(VAO);
		glUniform2f(uni_offset, x2, y2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(win);
	}

	return 0;
}