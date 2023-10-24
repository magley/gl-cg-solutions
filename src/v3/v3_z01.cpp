/*

Napisati program koji crta ljubicasti, jednakokraki trougao u gornjem
desnom kvadrantu prozora, na tamno sivoj pozadini. Baza trougla je duga
0.3 duzine jednog kvadranta prozora, a visina je 0.5 dužine jednog kvadranta.
Pritiskom na dugme B, pozadina mijenja boju u bijelu, a pritiskom na dugme
S se vraca na tamno sivu. Program se zatvara pritiskom na dugme ESC.

*/

#include "v3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/shader_util.h"

#define WIN_W 640
#define WIN_H 480

int v3_z01() {
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(WIN_W, WIN_H, "SDL2 >>> GLFW", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glewInit();

	// Triangle vertices. Since normalized coordinates go from -1 to 1, that means
	// the entire screen is 2x2 in size. This means that "half" the screen would
	// be, for example, from -0.5 to 0.5, which is of size 1.
	// The purple color is hard-coded in the fragment shader.

	const float width = 0.3f;
	const float height = 0.5f;
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

	unsigned shader = create_shader_program("./shader/v3/z01.vert", "./shader/v3/z01.frag");

	// Draw in top-right quadrant. (0, 0) is the bottom-left quadrant.

	glViewport(WIN_W / 2, WIN_H / 2, WIN_W / 2, WIN_H / 2);

	bool should_background_be_white = false;
	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		// Update background color based on user input.

		if (glfwGetKey(win, GLFW_KEY_B)) should_background_be_white = true;
		if (glfwGetKey(win, GLFW_KEY_S)) should_background_be_white = false;
		float bgcol = should_background_be_white ? 1.0f : 0.25f;
		glClearColor(bgcol, bgcol, bgcol, 1.0);

		// Close program if ESC is pressed

		if (glfwGetKey(win, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(win, 1);

		// Draw

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(win);
	}

	return 0;
}