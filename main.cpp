#include <iostream>
#include "points.h"
#include "projection.h"
#include "shapes.h"
#include <vector>
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";

int localWidth = 800;
int localHeight = 800;
float aspectRatio = (float)localWidth / (float)localHeight;
float playerMoveX = 0.0f;
float playerMoveY = 0.0f;
float playerMoveZ = 0.0f;
float modeX = 0.0f;
float modeY = 0.0f;
float modeZ = 0.0f;
float modeSpeed[3] = {1.0f, 0.01f, 0.5f,};
// 0=Object Spawn, 1=Object Move, 2=Object Rotate, 3=Object Scale
int mode = 0;
int modeObject = 0;
bool cycleObject = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	localWidth = width;
	localHeight = height;
	aspectRatio = (float)localWidth / (float)localHeight;
} 

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		playerMoveY += moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		playerMoveY -= moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		playerMoveZ += moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		playerMoveZ -= moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		playerMoveX -= moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		playerMoveX += moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		modeX += modeSpeed[mode];
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		modeX -= modeSpeed[mode];
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modeY -= modeSpeed[mode];
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modeY += modeSpeed[mode];
	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modeZ += modeSpeed[mode];
	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modeZ -= modeSpeed[mode];
	if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		mode = 0;
	if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		mode = 1;
	if(glfwGetKey(window, GLFW_KEY_2) == GLFW_KEY_PRESS)
		mode = 2;
	if(glfwGetKey(window, GLFW_KEY_3) == GLFW_KEY_PRESS)
		mode = 3;
	if(glfwGetKey(window, GLFW_KEY_T) == GLFW_KEY_RELEASE)
		cycleObject = true;
}

int main()
{

	// Window Setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(localWidth, localHeight, "Wireframe", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, localWidth, localHeight);
 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// End window setup

	// Shader setup
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);
	// End shader setup

	Point origin = Point(0, 0, 0);

	Shape cube = Shape(origin, Point(50.0f, 50.0f, 50.0f), CUBE);
	Shape cube2 = Shape(Point(100, 0, 0), Point(50.0f, 50.0f, 50.0f), CUBE);
	Point playerPosition = Point(0, 0, 0);
	std::vector<Shape> shapes = {cube, cube2};

	while(!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);


		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		playerPosition.Translate(xMove, yMove, zMove);

		if(cycleObject)
		{
			++modeObject;
			if(modeObject >= shapes.size())
				modeObject = 0;
			cycleObject = false;
		}
		
		if(mode == 1)
		{
			shapes[modeObject].Move(Point(modeX, modeY, modeZ));
		}
		else if(mode == 2)
		{
			shapes[modeObject].Rotate(Point(modeX, modeY, modeZ));
		}
		else if(mode == 3)
		{
			shapes[modeObject].Scale(Point(modeX, modeY, modeZ))
		}

		xMove = 0.0f;
		yMove = 0.0f;
		zMove = 0.0f;
		modeX = 0.0f;
		modeY = 0.0f;
		modeZ = 0.0f;

		for (Shape& activeObject : shapes) {
			unsigned int VAO = activeObject.getVertex(playerPosition, localWidth, localHeight);

			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawElements(GL_LINES, activeObject.indicesSize(), GL_UNSIGNED_INT, 0);
		}
		// Swap buffers and poll IO events

		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}
	

	glfwTerminate();
	std::cout << "Terminated GLFW" << std::endl;
	return 0;
}