#include <iostream>
#include "points.h"
#include "projection.h"
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
int localHeight = 600;
float aspectRatio = (float)localWidth / (float)localHeight;
float xMove = 0.0f;
float yMove = 0.0f;
float zMove = 0.0f;
float moveSpeed = 1.0f;

void rotate()
{
	std::cout << "Rotating points..." << std::endl;
}


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
}

int main()
{

	// Window Setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "Wireframe", NULL, NULL);
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

	glViewport(0, 0, 800, 600);
 
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
	// End shader setup

	// Create list of points for vertices of a cube
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

	Point origin = Point(10, 0, 0);

	for (int i = 0; i < points.size(); i++)
	{
		points[i].Translate(origin.x, origin.y, origin.z);
		points[i].Scale(50.0f, 50.0f, 50.0f, origin.x, origin.y, origin.z);
	}

	//float vertices[] = {
	//	0.5f,  0.5f, 0.0f,  // top right
	//	0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f   // top left 
    	//};

    	unsigned int indices[] = {  // note that we start from 0!
		0, 1,  // AB
		0, 2 , // AC 
		2, 3, // CD
		1, 3, // BD 
		4, 5, // EF
		4, 6, // EG
		6, 7, // GH
		5, 7, // FH
		0, 4, // AE
		1, 5, // BF
		2, 6, // CG
		3, 7 // DH
    	};

	
	

	while(!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);


		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < points.size(); i++)
		{
			points[i].Translate(xMove, yMove, zMove);
		}
		origin.Translate(xMove, yMove, zMove);

		xMove = 0.0f;
		yMove = 0.0f;
		zMove = 0.0f;

		std::vector<Point> projectedPoints = projectPoints(points);

		float vertices[projectedPoints.size() * 3];

		for (int i = 0; i < projectedPoints.size(); i++)
		{
			vertices[i * 3] = projectedPoints[i].x / (localWidth / 2.0f);
			vertices[i * 3 + 1] = projectedPoints[i].y / (localHeight / 2.0f);
			vertices[i * 3 + 2] = projectedPoints[i].z;
		}

		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0); 

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0); 

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_LINES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		// Swap buffers and poll IO events

		glfwSwapBuffers(window);
		glfwPollEvents();    
	}
	

	glfwTerminate();
	std::cout << "Terminated GLFW" << std::endl;
	return 0;
}