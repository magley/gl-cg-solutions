/*
Napisati fullscreen aplikaciju koja prikazuje crvenu tacku na centru ekrana 
(Tacka je veæa od 1 piksela) koja moze da se pomijera po ekranu pomocu WSAD 
tastera, a ukoliko izadje izvan ekrana, pojavljuje se na suprotnoj ivici. Boja 
tacke pulsira od crvene do zute tokom vremena, i pulsira brze što je dalja od 
koordinatnog pocetka. Tacka ostavlja trag trenutne boje za sobom. Taster ESC 
zatvara program. Taster R brise ekran i resetuje tacku na koordinatni pocetak.
*/


#include "v3.h"

#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/shader_util.h"

#define WIN_W 600
#define WIN_H 600

int v3_z05() {
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

	float vertices_point[] = {
		// x      y
		+0.0,   +0.0,
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_point), vertices_point, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);


	unsigned shader = create_shader_program("./shader/v3/z04.vert", "./shader/v3/z04.frag");
	unsigned uni_offset = glGetUniformLocation(shader, "uni_offset");
	unsigned uni_color = glGetUniformLocation(shader, "uni_color");


	// RED    = (1.0, 0.0, 0.0)
	// YELLOW = (1.0, 1.0, 0.0)
	// So to pulse from red to yellow, just change the amount of green.
	float player_green = 0.0f;
	float player_x = 0, player_y = 0;
	float player_size = 8;

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(win, 1);
		}

		// ---------- Game logic --------------------------------

		// Move player and wrap to other side.

		player_x += (glfwGetKey(win, GLFW_KEY_RIGHT) - glfwGetKey(win, GLFW_KEY_LEFT)) * 0.025f;
		player_y -= (glfwGetKey(win, GLFW_KEY_DOWN) - glfwGetKey(win, GLFW_KEY_UP)) * 0.025f;

		if (player_x > 1.0f) {
			player_x = -1.0f;
		}
		else if (player_x < -1.0f) {
			player_x = 1.0f;
		}
		if (player_y > 1.0f) {
			player_y = -1.0f;
		}
		else if (player_y < -1.0f) {
			player_y = 1.0f;
		}

		// Player color.
		
		float dist_from_center = player_x * player_x + player_y * player_y; // Squared distance but whatever.
		float pulse_speed = 1.0f + (3.0 * dist_from_center);

		player_green = fabs(sinf(glfwGetTime() * pulse_speed));

		// If [R], move to start

		if (glfwGetKey(win, GLFW_KEY_R)) {
			player_x = 0;
			player_y = 0;
		}

		// ---------- Rendering --------------------------------

		glClearColor(0, 0, 0, 1.0);

		// If you don't reset, the player will "leave a trail". This only works if
		// nothing else is being rendered, though.
		if (glfwGetKey(win, GLFW_KEY_R)) {
			glClear(GL_COLOR_BUFFER_BIT);
		}
		glUseProgram(shader);

		// Player.

		glBindVertexArray(VAO);
		glUniform4f(uni_color, 1.0f, player_green, 0.0, 1.0f);
		glUniform2f(uni_offset, player_x, player_y);
		glPointSize(player_size);
		glDrawArrays(GL_POINTS, 0, 1);
		
		glfwSwapBuffers(win);
	}

	return 0;
}