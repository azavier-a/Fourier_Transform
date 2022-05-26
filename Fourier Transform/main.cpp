#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include "IndexBuffer.h"
#include "VertexArray.h"

#include <string>
#include <iostream>

#define TAU (M_PI*2)
#define mapX(a) (a*2/wid - 1)
#define mapY(a) -(a*2/hei - 1)

const unsigned int wid = 800, hei = 800, minCircleQuality = 10, maxCircleQuality = 50, VERTICES = 50;

double mX, mY;

const char *vertShaderSource = R"glsl(
							    #version 330 core
						        layout (location = 0) in vec4 pos;
							    void main() {
									gl_Position = pos; 
								} )glsl",
		   *fragShaderSource = R"glsl(
							    #version 330 core
							    out vec4 color;
								uniform vec4 u_Color;
						        void main() { 
									color = u_Color; 
								} )glsl";

void circle(double* arr, double x, double y, double r) {
	// unsigned int VERTICES = r/10 + 10; // when r = 400, VERTICES = maxCircleQuality, when r = 0, VERTICES = minCircleQuality. 10 + r/10
	// arr = new double[VERTICES * 3];

	r *= (2.0 / wid);

	for (int v = 0; v <= VERTICES; v++) {
		double t = v * TAU / VERTICES;

		arr[v * 2] = x + r * cos(t);
		arr[v * 2 + 1] = y + r * sin(t);
	}
}

static void keyPressed(GLFWwindow* win, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(win, GL_TRUE);
}

static void mousePosCallback(GLFWwindow* win, double xPos, double yPos) {
	mX = mapX(xPos);
	mY = mapY(yPos);
}

void framebuffersizeCallback(GLFWwindow* win, int widt, int heig) {
	call(glViewport(0, 0, widt, heig));
}

static std::string compareShaderType(unsigned int type) {
	switch (type) {
	case GL_VERTEX_SHADER:
		return "vertex";
	case GL_FRAGMENT_SHADER:
		return "fragment";
	case GL_GEOMETRY_SHADER:
		return "geometry";
	default:
		return "error";
	}
}

static unsigned int compileShader(const std::string& source, unsigned int type) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	call(glShaderSource(id, 1, &src, nullptr));
	call(glCompileShader(id));

	// Errror handling.
	int r;
	call(glGetShaderiv(id, GL_COMPILE_STATUS, &r));
	if (r == GL_FALSE) {
		int l;
		call(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l));

		char* message = (char*)alloca(l * sizeof(char));

		call(glGetShaderInfoLog(id, l, &l, message));

		std::cout << "Failed to compile " << compareShaderType(type) << " shader:" << std::endl;
		std::cout << message << std::endl;

		call(glDeleteShader(id));
		exit(EXIT_FAILURE);
	}

	return id;
}

static int c_shaderProgram(const std::string& vertShdr, const std::string& fragShdr) {
	unsigned int vert = compileShader(vertShdr, GL_VERTEX_SHADER),
				 frag = compileShader(fragShdr, GL_FRAGMENT_SHADER),
				 prog = glCreateProgram();

	call(glAttachShader(prog, vert));
	call(glAttachShader(prog, frag));

	call(glLinkProgram(prog));

	call(glValidateProgram(prog));

	call(glDeleteShader(vert));
	call(glDeleteShader(frag));

	return prog;
}

int main(void) {
	GLFWwindow* win;

	if (!glfwInit()) exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	win = glfwCreateWindow(wid, hei, "Fourier Transform.", NULL, NULL); 
	glfwMakeContextCurrent(win);

	glfwSetFramebufferSizeCallback(win, framebuffersizeCallback);
	glfwSetKeyCallback(win, keyPressed);
	glfwSetCursorPosCallback(win, mousePosCallback);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) exit(EXIT_FAILURE);

	unsigned int shaderProgram = c_shaderProgram(vertShaderSource, fragShaderSource);

	double cVert[VERTICES*2];

	VertexArray va;
	VertexBuffer VBO(cVert, VERTICES * 2 * sizeof(double), GL_DYNAMIC_DRAW);
	VertexBufferLayout layout;

	layout.Push<double>(2);
	va.AddBuffer(VBO, layout);

	call(glUseProgram(shaderProgram));

	int loc = glGetUniformLocation(shaderProgram, "u_Color");

	call(glUniform4f(loc, 1.0f, 0.0f, 0.0f, 1.0f));

	double r = 10;
	while (!glfwWindowShouldClose(win)) {
		call(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		call(glUniform4f(loc, abs(mX), abs(mY), (mX + mY)/2.0f, 1.0f));

		circle(cVert, mX, mY, r);

		call(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cVert), cVert));

		va.Bind();

		call(glDrawArrays(GL_LINE_LOOP, 0, VERTICES));

		glfwSwapBuffers(win);
		glfwPollEvents();

		r += 0.025;
		if (r > 60) r = 10;
 	}

	call(glDeleteProgram(shaderProgram));
	glfwDestroyWindow(win);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}