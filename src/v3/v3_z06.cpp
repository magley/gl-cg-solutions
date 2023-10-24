/*

Nacrtati bijeli trougao na crnoj pozadini u gornjem lijevom
kvadrantu ekrana, koji se pomijera po ekranu sa WSAD tasterima,
a vraæa na koordinatni poèetak tasterom R.
Dio trougla koji preðe u gornji desni kvadrant biva obojen crvenom bojom,
dio koji preðe u donji desni dio biva obojen ljubicastom bojom,
a dio koji preðe u lijevi donji kvadrant biva obojen plavom bojom.
Dio trougla koji preðe na donju polovinu ekrana se crta u vertikalnim prugama.
Ukoliko trougao priðe donjem desnom uglu, ili se pritisne taster ESC, 
program se zatvara. Taster R postavlja trougao nazad na poèetni polozaj.

*/


#include "v3.h"

#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/shader_util.h"

#define WIN_W 600
#define WIN_H 600

int v3_z06() {
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

	const float width = 0.25f;
	const float height = 0.25f;
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


	unsigned shader = create_shader_program("./shader/v3/z06.vert", "./shader/v3/z06.frag");
	unsigned uni_offset = glGetUniformLocation(shader, "uni_offset");
	unsigned uni_win_size = glGetUniformLocation(shader, "uni_win_size");

	float player_x = -0.5, player_y = +0.5;

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(win, 1);
		}

		// ---------- Game logic --------------------------------

		// Move player.

		player_x += (glfwGetKey(win, GLFW_KEY_D) - glfwGetKey(win, GLFW_KEY_A)) * 0.025f;
		player_y -= (glfwGetKey(win, GLFW_KEY_S) - glfwGetKey(win, GLFW_KEY_W)) * 0.025f;

		// If [R], move to start.

		if (glfwGetKey(win, GLFW_KEY_R)) {
			player_x = -0.5;
			player_y = +0.5;
		}

		// If player touches bottom-right corner, exit.

		if (player_x + width >= 1 && player_y - height <= -1) {
			glfwSetWindowShouldClose(win, 1);
		}

		// ---------- Rendering --------------------------------

		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glUniform2f(uni_win_size, WIN_W, WIN_H); // Could be extracted outside the loop if WIN_W and WIN_H never change.

		// Player.

		glBindVertexArray(VAO);
		glUniform2f(uni_offset, player_x, player_y);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(win);
	}

	return 0;
}