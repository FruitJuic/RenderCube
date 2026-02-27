#include <iostream>
#include "shapes.h"
#include "points.h"
#include "projection.h"
#include <vector>
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

std::vector<Point> cubePoints = {
	Point(-1, 1, -1), // A 0
	Point(1, 1, -1), // B 1
	Point(-1, -1, -1), // C 2
	Point(1, -1, -1), // D 3
	Point(-1, 1, 1), // E 4
	Point(1, 1, 1), // F 5 
	Point(-1, -1, 1), // G 6
	Point(1, -1, 1) // H 7
};

std::vector<unsigned int> cubeIndices = { 
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


Shape::Shape(Point m_position, Point m_size, ShapeType type)
{
	position = Vec3{m_position.x, m_position.y, m_position.z};
	size = Vec3{m_size.x, m_size.y, m_size.z};
	rotation = Vec3{0, 0, 0};
	localWidth = 800;
	localHeight = 800;
	switch (type) {
	case CUBE:
		points = cubePoints;
		indices = cubeIndices;
		break;
	default:
		break;
	}
}

void Shape::Move(Vec3 translation)
{
	for (Point& point : worldPoints) {
		point.Translate(translation.x, translation.y, translation.z);
	}
}

void Shape::Scale(Vec3 scale, Vec3 pivot)
{
	for (Point& point : worldPoints) {
		point.Scale(scale.x, scale.y, scale.z, pivot.x, pivot.y, pivot.z);
	}
}

void Shape::Rotate(Vec3 rotation, Vec3 pivot)
{
	for (Point& point : worldPoints) {
		point.Rotate(rotation.x, rotation.y, rotation.z, pivot.x, pivot.y, pivot.z);
	}
}

void Shape::ChangeRotation(Vec3 newRotation)
{
	rotation.x += newRotation.x;
	rotation.y += newRotation.y;
	rotation.z += newRotation.z;
}

void Shape::ChangePosition(Vec3 newPosition)
{
	this->position.x += newPosition.x;
	this->position.y += newPosition.y;
	this->position.z += newPosition.z;
}

void Shape::ChangeSize(Vec3 newSize)
{
	size.x += newSize.x;
	size.y += newSize.y;
	size.z += newSize.z;
}

void Shape::updateBuffer()
{
	// Check if any transform values changed, regen buffer if so
	if(!(position.x == bufferPosition.x && position.y == bufferPosition.y && position.z == bufferPosition.z) || !(size.x == bufferSize.x && size.y == bufferSize.y && size.z == bufferSize.z) || !(rotation.x == bufferRotation.x && rotation.y == bufferRotation.y && rotation.z == bufferRotation.z) || localWidth != bufferWidth || localHeight != bufferHeight)
	{
		updateWorldPoints();

		bufferPosition = position;
		bufferSize = size;
		bufferRotation = rotation;
		bufferWidth = localWidth;
		bufferHeight = localHeight;

		projectedPoints = projectPoints(worldPoints);

		float vertices[projectedPoints.size() * 3];

		for (int i = 0; i < projectedPoints.size(); i++)
		{
			vertices[i * 3] = projectedPoints[i].x / (localWidth/2.0f);
			vertices[i * 3 + 1] = projectedPoints[i].y / (localHeight/2.0f);
			vertices[i * 3 + 2] = projectedPoints[i].z;
		}


		unsigned int workableIndices[indices.size()];
		for (int i = 0; i < indices.size(); i++)
		{
			workableIndices[i] = indices[i];
		}

		// Update buffer with new world points

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(workableIndices), workableIndices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0); 

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0); 
	}
}

void Shape::updateWorldPoints()
{
	worldPoints.clear();
	for (Point point : points) {
		worldPoints.push_back(point);
	}

	Move(position);
	Scale(size, position);
	Rotate(rotation, position);
}

unsigned int Shape::getVertex(int localWidth, int localHeight)
{
	this->localWidth = localWidth;
	this->localHeight = localHeight;
	updateBuffer();
	return VAO;
}