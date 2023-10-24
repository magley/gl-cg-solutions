#pragma once

unsigned int create_shader(unsigned int shader_enum, const char* source_fname);
unsigned int create_shader_program(unsigned vertex, unsigned fragment);
unsigned int create_shader_program(const char* vertex_fname, const char* fragment_fname);