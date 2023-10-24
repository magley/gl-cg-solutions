#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WIN_W 640
#define WIN_H 480

int main_template01() {
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(WIN_W, WIN_H, "SDL2 >>> GLFW", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glewInit();

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		glClearColor(0.25, 0.25, 0.25, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(win);
	}

	return 0;
}