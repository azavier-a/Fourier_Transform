#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>

#define TAU (M_PI*2)
#define mapX(a) (a*2/wid - 1)
#define mapY(a) -(a*2/hei - 1)

const unsigned int wid = 800, hei = 800, minCircleQuality = 10, maxCircleQuality = 50, VERTICES = 50;

double mX, mY;


const char *vertShaderSource = "#version 330 core\n"
						       "layout (location = 0) in vec3 aPos;\n"
							   "void main() { gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }",
		   *fragShaderSource = "#version 330 core\n"
							   "out vec4 FragColor;\n"
						       "void main() { FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f); }";

void circle(double* arr, double x, double y, double r) {
	// unsigned int VERTICES = r/10 + 10; // when r = 400, VERTICES = maxCircleQuality, when r = 0, VERTICES = minCircleQuality. 10 + r/10
	// arr = new double[VERTICES * 3];

	r *= (2.0 / wid);

	for (int v = 0; v <= VERTICES; v++) {
		double t = v * TAU / VERTICES;

		arr[v * 3] = x + r * cos(t);
		arr[v * 3 + 1] = y + r * sin(t);
		arr[v * 3 + 2] = 0.0f;
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
/*
void input(GLFWwindow* win) {
	if(glfwGetKey(win, GLFW_KEY_))
}
*/
void framebuffersizeCallback(GLFWwindow* win, int widt, int heig) {
	glViewport(0, 0, widt, heig);
}

void init() {
	glViewport(0, 0, wid, hei);
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

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER),
		   fragShader = glCreateShader(GL_FRAGMENT_SHADER),
		   shaderProgram = glCreateProgram(),
   	       VBO, VAO;

	glShaderSource(vertShader, 1, &vertShaderSource, NULL);
	glShaderSource(fragShader, 1, &fragShaderSource, NULL);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	double cVert[VERTICES*3];
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 150, 0, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	init();

	double r = 10;
	while (!glfwWindowShouldClose(win)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		circle(cVert, mX, mY, r);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cVert), cVert);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glDrawArrays(GL_LINE_LOOP, 0, VERTICES);

		glfwSwapBuffers(win);
		glfwPollEvents();

		r += 0.025;
		if (r > 60) r = 10;
 	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(win);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}