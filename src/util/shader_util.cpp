#include "shader_util.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

static std::string readfile(const char* fname) {
	std::ifstream t(fname);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}


unsigned int create_shader(unsigned int shader_enum, const char* source_fname) {
	std::string source = readfile(source_fname);
	const char* c = source.c_str();
	unsigned int shader;
	shader = glCreateShader(shader_enum);
	glShaderSource(shader, 1, &c, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		char log[512];
		glGetShaderInfoLog(shader, 512, NULL, log);
		printf(RED "Error compiling shader '%s'\n%s\n" RESET, source_fname, log);
		return -1;
	}

	printf("Compiled shader '%s'\n", source_fname);
	return shader;
}

unsigned int create_shader_program(unsigned vertex, unsigned fragment) {
	unsigned prog = glCreateProgram();
	glAttachShader(prog, vertex);
	glAttachShader(prog, fragment);
	glLinkProgram(prog);

	int success;
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success) {
		char log[512];
		glGetProgramInfoLog(prog, 512, NULL, log);
		printf(RED "Error linking program\n%s\n" RESET, log);
		return -1;
	}

	printf("Linked program\n");
	return prog;
}

unsigned int create_shader_program(const char* vertex_fname, const char* fragment_fname) {
	unsigned shd_basic_vert = create_shader(GL_VERTEX_SHADER, vertex_fname);
	unsigned shd_basic_frag = create_shader(GL_FRAGMENT_SHADER, fragment_fname);
	unsigned prog_basic = create_shader_program(shd_basic_vert, shd_basic_frag);
	return prog_basic;
}