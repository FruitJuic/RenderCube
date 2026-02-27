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
float xMove = 0.0f;
float yMove = 0.0f;
float zMove = 0.0f;
float xRotate = 0.0f;
float yRotate = 0.0f;
float zRotate = 0.0f;
float moveSpeed = 1.0f;
float rotateSpeed = 0.01f;


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
		yMove -= moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		yMove += moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		zMove -= moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		zMove += moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		xMove += moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		xMove -= moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		xRotate -= rotateSpeed;
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		xRotate += rotateSpeed;
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		yRotate -= rotateSpeed;
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		yRotate += rotateSpeed;
	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		zRotate += rotateSpeed;
	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		zRotate -= rotateSpeed;
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

	std::vector<Point> points = {
		Point(-1, 1, -1), // A 0
		Point(1, 1, -1), // B 1
		Point(-1, -1, -1), // C 2
		Point(1, -1, -1), // D 3
		Point(-1, 1, 1), // E 4
		Point(1, 1, 1), // F 5 
		Point(-1, -1, 1), // G 6
		Point(1, -1, 1) // H 7
	};

	Point origin = Point(0, 0, 0);

	//float vertices[] = {
	//	0.5f,  0.5f, 0.0f,  // top right
	//	0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f   // top left 
    	//};

    	//unsigned int indices[] = {  // note that we start from 0!
	//	0, 1,  // AB
	//	0, 2 , // AC 
	//	2, 3, // CD
	//	1, 3, // BD 
	//	4, 5, // EF
	//	4, 6, // EG
	//	6, 7, // GH
	//	5, 7, // FH
	//	0, 4, // AE
	//	1, 5, // BF
	//	2, 6, // CG
	//	3, 7 // DH
    	//};

	Shape cube = Shape(origin, Point{50.0f, 50.0f, 50.0f}, CUBE);
	Shape cube2 = Shape(Point(100, 0, 0), Point{50.0f, 50.0f, 50.0f}, CUBE);
	
	std::vector<Shape> shapes = {cube, cube2};

	while(!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);


		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		cube.ChangePosition(Point(xMove, yMove, zMove));
		cube.ChangeRotation(Point(xRotate, yRotate, zRotate));

		xMove = 0.0f;
		yMove = 0.0f;
		zMove = 0.0f;
		xRotate = 0.0f;
		yRotate = 0.0f;
		zRotate = 0.0f;

		for (Shape& activeObject : shapes) {
			unsigned int VAO = activeObject.getVertex(localWidth, localHeight);

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