#ifndef SHAPES_H
#define SHAPES_H

#include "points.h"
#include <vector>

enum ShapeType {
	CUBE
}; 

class Shape
{
public:
	unsigned int getVertex(int, int);
	int indicesSize() { return indices.size(); }
	Shape(Point, Point, ShapeType);
	void ChangeRotation(Vec3);
	void ChangePosition(Vec3);
	void ChangeSize(Vec3);
	private:
	std::vector<Point> points;
	std::vector<Point> worldPoints;
	std::vector<Point> projectedPoints;
	std::vector<unsigned int> indices;
	Vec3 position;
	Vec3 size;
	Vec3 rotation;
	int localWidth;
	int localHeight;
	Vec3 bufferPosition;
	Vec3 bufferSize;
	Vec3 bufferRotation;
	int bufferWidth;
	int bufferHeight;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	void Move(Vec3);
	void Scale(Vec3, Vec3);
	void Rotate(Vec3, Vec3);
	void updateBuffer();
	void updateWorldPoints();
};


#endif