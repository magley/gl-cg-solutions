/*

Nacrtati proizvoljnu plavu zvijezdu na proizvoljnoj poziciji.
Nacrtati poluprovidan kvadrat na centru ekrana, cija je stranica
duga 20% jednog kvadranta i implementirati sledece funkcionalnosti tastera:

ESC - Zatvara program
Strelice (Arrow keys) - Pomjeranje kvadrata, tako da kvadrat ne može preci ivice prozora
R - Vraca kvadrat na centar ekrana;
1 - Prikazuje samo tjemena poligona koji cine kvadrat, koja su povecana da budu lakše uocljiva
2 - Prikazuje ivice poligona (tzv. Wireframe mode)
3 - Prikazuje kvadrat ispunjen bojom
4 - Iskljucuje providnost kvadrata
5 - Ukljucuje providnost kvadrata
Kvadrat se crta preko trougla, i na trougao ne uticu funkcionalnosti tastera 1-5.

*/


#include "v3.h"

#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/shader_util.h"

#define WIN_W 600
#define WIN_H 600

int v3_z04() {
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(WIN_W, WIN_H, "SDL2 >>> GLFW", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glEnable(GL_BLEND); // new
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // new
	glEnable(GL_PROGRAM_POINT_SIZE); // new
	glfwSwapInterval(1);
	glewInit();

	float vertices_star[] = {
		// x      y
		+0.50,   +0.0,
		-0.50,   +0.0,
		+0.35,   -0.5,
		+0.00,   +0.35,
		-0.35,   -0.5,
	};

	const float w = (1.0 / 5.0) / 2.0;
	float vertices_square[] = {
		//   x    y
			+w,	 +w,
			+w,	 -w,
			-w,  -w,

			+w,  +w,
			-w,  -w,
			-w,  +w
	};

	unsigned int VBO_star, VAO_star;
	glGenBuffers(1, &VBO_star);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_star);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_star), vertices_star, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_star);
	glBindVertexArray(VAO_star);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	unsigned int VBO_square, VAO_square;
	glGenBuffers(1, &VBO_square);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_square);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_square), vertices_square, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_square);
	glBindVertexArray(VAO_square);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	unsigned shader = create_shader_program("./shader/v3/z04.vert", "./shader/v3/z04.frag");
	unsigned uni_offset = glGetUniformLocation(shader, "uni_offset");
	unsigned uni_color = glGetUniformLocation(shader, "uni_color");

	// Moving square's state.

	float player_x = 0, player_y = 0;
	float player_line_width = 1.0f;
	float player_opacity = 0.5f;
	int player_draw_mode = GL_TRIANGLES;

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(win, 1);
		}

		// ---------- Game logic --------------------------------

		// Move player and limit position to screen.

		player_x += (glfwGetKey(win, GLFW_KEY_RIGHT) - glfwGetKey(win, GLFW_KEY_LEFT)) * 0.025f;
		player_y -= (glfwGetKey(win, GLFW_KEY_DOWN) - glfwGetKey(win, GLFW_KEY_UP)) * 0.025f;

		if (player_x + w >= 1.0f) {
			player_x = 1.0f - w;
		}
		else if (player_x - w <= -1.0f) {
			player_x = -1.0f + w;
		}

		if (player_y + w >= 1.0f) {
			player_y = 1.0f - w;
		}
		else if (player_y - w <= -1.0f) {
			player_y = -1.0f + w;
		}

		if (glfwGetKey(win, GLFW_KEY_R)) {
			player_x = 0;
			player_y = 0;
		}

		// Player draw mode.

		if (glfwGetKey(win, GLFW_KEY_1)) {
			player_draw_mode = GL_POINTS;
			player_line_width = 4;
		}
		else if (glfwGetKey(win, GLFW_KEY_2)) {
			player_draw_mode = GL_LINE_LOOP;
			player_line_width = 1;
		}
		else if (glfwGetKey(win, GLFW_KEY_3)) {
			player_draw_mode = GL_TRIANGLES;
		}
		else if (glfwGetKey(win, GLFW_KEY_4)) {
			player_opacity = 1.0f;
		}
		else if (glfwGetKey(win, GLFW_KEY_5)) {
			player_opacity = 0.5f;
		}

		// ---------- Rendering --------------------------------

		glClearColor(0.18, 0.18, 0.18, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);

		// Star.

		glBindVertexArray(VAO_star);
		glUniform4f(uni_color, 0.0825f, 0.525f, 0.95f, 1.0f);
		glUniform2f(uni_offset, 0.0f, 0.0f);
		glLineWidth(5.0);
		glDrawArrays(GL_LINE_LOOP, 0, 5);
		glLineWidth(1.0); // Just in case.

		// Square (since it's in front of the star, it must be drawn after the star (painter's algorithm)).

		glBindVertexArray(VAO_square);
		glUniform4f(uni_color, 0.85f, 0.95f, 0.125f, player_opacity);
		glUniform2f(uni_offset, player_x, player_y);
		glPointSize(player_line_width);
		glDrawArrays(player_draw_mode, 0, 6);
		glPointSize(1); // Just in case.

		glfwSwapBuffers(win);
	}

	return 0;
}