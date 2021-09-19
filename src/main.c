#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include "settings.h"
#include "shader.h"

static void doInit()
{

	shader_load(SHADER_BASIC);

	// vertex buffer object, holds vertices (not always coords)
	float vertices[] = {
		 0.0f,  0.5f,
		 0.5f, -0.5f,
		-0.5f, -0.5f
	};
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(float), (void *)vertices, GL_STATIC_DRAW);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}

static void doLogic()
{

}

static void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
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

	glfwTerminate();
	return EXIT_SUCCESS;
}
