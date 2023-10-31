/*

Napisati program koji na tamno sivoj pozadini svake 3 sekunde nacrta elipsu
(preko prethodnih elipsi). Prva elipsa je duza po X osi, druga je duza po Y osi, 
treca je duza po X osi, cetvrta po Y, itd. Svaka naredna elipsa je manja od prethodne
tako da ne izlazi iz nje. Dimenzije i boje elipsi su proizvoljne. ESC zatvara program.

*/


#include "v3.h"

#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/shader_util.h"
#include <vector>

#define WIN_W 600
#define WIN_H 600

struct Ellipse {
	float w, h;
	float r, g, b;

	Ellipse(float w, float h) : w(w), h(h) {
		r = (rand() % 255) / 255.0;
		g = (rand() % 255) / 255.0;
		b = (rand() % 255) / 255.0;
	}
};

int v3_z08() {
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

	const int circle_precision = 100;
	float vertices[circle_precision * 2] = { 0 };

	for (int i = 0; i < circle_precision; i ++) {
		const float angle = (360.0 / circle_precision) * i * 3.14f / 180.0f;
		float x = 0, y = 0;

		x = 0.0 + 1.0f * cos(angle);
		y = 0.0 + 1.0f * sin(angle);
		vertices[i * 2 + 0] = x;
		vertices[i * 2 + 1] = y;
	}

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	unsigned shader = create_shader_program("./shader/v3/z08.vert", "./shader/v3/z08.frag");
	unsigned uni_scale = glGetUniformLocation(shader, "uni_scale");
	unsigned uni_col = glGetUniformLocation(shader, "uni_col");

	float glo_w = 1.0f;
	float glo_h = 1.0f;
	bool shrink_w = true;
	std::vector<Ellipse> ellipses;

	int tick = 0;

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(win, 1);
		}

		tick++;

		// ---------- Game logic --------------------------------

		if (tick % 180 == 0) {
			if (shrink_w) {
				float temp = glo_h;
				glo_h = glo_w;
				glo_w = temp;
				glo_w -= 0.175f;
				glo_h -= 0.025f;
			}
			else {
				float temp = glo_w;
				glo_w = glo_h;
				glo_h = temp;
				glo_h -= 0.175f;
				glo_w -= 0.025f;
			}

			glo_w = fmax(glo_w, 0.01);
			glo_h = fmax(glo_h, 0.01);

			shrink_w != true;

			ellipses.push_back(Ellipse(glo_w, glo_h));
		}

		// ---------- Rendering --------------------------------

		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(VAO);

		// Triangle 1

		for (int i = 0; i < ellipses.size(); i++) {
			const Ellipse& e = ellipses[i];

			glUniform2f(uni_scale, e.w, e.h);
			glUniform3f(uni_col, e.r, e.g, e.b);

			glDrawArrays(GL_POLYGON, 0, sizeof(vertices) / 2);
		}

		glfwSwapBuffers(win);
	}

	return 0;
}