#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "cglm/cglm.h"

#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include "settings.h"
#include "shader.h"
#include "texture.h"

unsigned int vao;
unsigned int cube_shader;
unsigned int u_time;

vec3 axis = { 0.0f, 0.0f, 1.0f };
unsigned int u_trans;
mat4 trans;

static void doInit()
{
	glm_mat4_identity(trans);

	float vertices[] = {
		-0.25f,  0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 1.0f,
		 0.25f, -0.5f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		-0.75f, -0.5f,		0.0f, 1.0f,		1.0f, 1.0f, 1.0f,
		 0.75f,  0.5f,		1.0f, 0.0f,		1.0f, 1.0f, 1.0f
	};

	unsigned int elements[] = {
		0, 1, 2,
		0, 3, 1
	};

	// vertex array object, holds all this state
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// vertex buffer object, holds vertices (not always coords)
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void *)vertices, GL_STATIC_DRAW);


	// element buffer object
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), (void *)elements, GL_STATIC_DRAW);

	// vertex attribute config and shader binding
	cube_shader = shader_load(SHADER_BASIC);
	glUseProgram(cube_shader);

	printf("1\n");
	int pos_attrib = glGetAttribLocation(cube_shader, "position");
	glEnableVertexAttribArray(pos_attrib);
	glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
	printf("2\n");
	int tex_attrib = glGetAttribLocation(cube_shader, "in_texcoord");
	glEnableVertexAttribArray(tex_attrib);
	glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(2 * sizeof(float)));
	printf("3\n");
	int col_attrib = glGetAttribLocation(cube_shader, "in_color");
	glEnableVertexAttribArray(col_attrib);
	glVertexAttribPointer(col_attrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(4 * sizeof(float)));
	printf("4\n");

	// texture 1
	glActiveTexture(GL_TEXTURE0);
	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	struct texture texture_data;
	texture_loadFromPNG(&texture_data, "res/lena512color.png");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data.width, texture_data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(glGetUniformLocation(cube_shader, "tex"), 0);

	// texture 2
	glActiveTexture(GL_TEXTURE1);
	unsigned int tex2;
	glGenTextures(1, &tex2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	struct texture texture2_data;
	texture_loadFromPNG(&texture2_data, "res/world.png");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture2_data.width, texture2_data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2_data.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(glGetUniformLocation(cube_shader, "tex2"), 1);

	u_trans = glGetUniformLocation(cube_shader, "trans");

	u_time = glGetUniformLocation(cube_shader, "time");

	glBindVertexArray(0);
	glUseProgram(0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

}

float time = 0.0f;
int direction = 1;
static void doLogic()
{

	time += (float)direction * 0.01f;
	if (time >= 1.0f || time <= 0.0f) direction = -direction;

	glm_rotate(trans, glm_rad(1.0f), axis);

	glUseProgram(cube_shader);
	glUniform1f(u_time, time);
	glUniformMatrix4fv(u_trans, 1, GL_FALSE, (float *)trans);
	glUseProgram(0);

}

static void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vao);
	glUseProgram(cube_shader);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

static void error_callback(int error, const char *description)
{
	fprintf(stderr, "ERROR: %d:\n%s\n", error, description);
}


static void cursor_pos_callback(GLFWwindow *win, double xpos, double ypos)
{

}

static void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch(key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(win, GLFW_TRUE);
			break;
		case GLFW_KEY_SPACE:
			glUseProgram(cube_shader);
			vec3 translation = {0.05f, 0.01f, 0.0f};
			glm_translate(trans, translation);
			glUseProgram(0);
			break;
		}
	}
}

int main(int argc, char *argv[])
{

	glfwSetErrorCallback(error_callback);
	printf("GLFW version string: %s\n", glfwGetVersionString());
	if (!glfwInit()) {
		fprintf(stderr, "Unable to initialise GLFW\n");
		return EXIT_FAILURE;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#if GAME_FULLSCREEN == 1
	GLFWwindow *win = glfwCreateWindow(GAME_WIDTH, GAME_HEIGHT, "An OpenGL Game", glfwGetPrimaryMonitor(), NULL); // fullscreen
#endif
#if GAME_FULLSCREEN == 0
	GLFWwindow *win = glfwCreateWindow(GAME_WIDTH, GAME_HEIGHT, "An OpenGL Game", NULL, NULL); // windowed
#endif
	if (!win) {
		fprintf(stderr, "Unable to create window\n");
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(win);

	// get proper OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		fprintf(stderr, "ERROR: unable to initialise GLAD");
		exit(EXIT_FAILURE);
	}
	printf("OpenGL Version %d.%d loaded\n", GLVersion.major, GLVersion.minor);
	if (!GLAD_GL_VERSION_2_1) {
		fprintf(stderr, "ERROR: must be running at least OpenGL 2.1\n");
		exit(EXIT_FAILURE);
	}

	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(win, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	glfwSetCursorPosCallback(win, cursor_pos_callback);
	glfwSetKeyCallback(win, key_callback);
	glfwSwapInterval(GAME_SWAPINTERVAL);

	doInit();

	while (!glfwWindowShouldClose(win)) {

		doLogic();

		draw();

		glfwSwapBuffers(win);

		glfwPollEvents();
		
	}

	shader_delete(cube_shader);
	glfwTerminate();
	return EXIT_SUCCESS;
}
