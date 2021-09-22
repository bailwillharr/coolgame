#include <stdio.h>
#include <stdlib.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader.h"

#include "shaders/shaders.h"

static unsigned int compile_shader(const char **src_shad, const int *src_shad_len, int shader_type)
{
	unsigned int id = glCreateShader(shader_type);
	glShaderSource(id, 1, src_shad, src_shad_len);
	glCompileShader(id);
	// check for errors in compilation
	int status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(id, 512, NULL, buffer);
		fprintf(stderr, "Shader compilation error:\n");
		fputs(buffer, stderr);
		exit(EXIT_FAILURE);
	}

	return id;
}

unsigned int shader_load(enum shader s)
{
	const char *src_vert, *src_frag;
	int src_vert_len, src_frag_len;
	switch (s) {
	case SHADER_BASIC:
		src_vert = (char *)&_binary_src_shaders_basic_vert_start;
		src_frag = (char *)&_binary_src_shaders_basic_frag_start;
		src_vert_len =
			&_binary_src_shaders_basic_vert_end -
			&_binary_src_shaders_basic_vert_start;
		src_frag_len =
			&_binary_src_shaders_basic_frag_end -
			&_binary_src_shaders_basic_frag_start;
		break;
	default:
		fprintf(stderr, "Unknown shader enum: %d", s);
		exit(EXIT_FAILURE);
	}

	unsigned int vert_shader = compile_shader(&src_vert, &src_vert_len, GL_VERTEX_SHADER);
	unsigned int frag_shader = compile_shader(&src_frag, &src_frag_len, GL_FRAGMENT_SHADER);

	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program);
	glValidateProgram(shader_program);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	return shader_program;
	
}

void shader_delete(unsigned int id)
{
	glDeleteProgram(id);
}
