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
	void ChangeRotation(Point);
	void ChangePosition(Point);
	void ChangeSize(Point);
private:
	std::vector<Point> points;
	std::vector<Point> worldPoints;
	std::vector<Point> projectedPoints;
	std::vector<unsigned int> indices;
	Point position;
	Point size;
	Point rotation;
	int localWidth;
	int localHeight;
	Point bufferPosition;
	Point bufferSize;
	Point bufferRotation;
	int bufferWidth;
	int bufferHeight;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	void Move(Point);
	void Scale(Point, Point);
	void Rotate(Point, Point);
	void updateBuffer();
	void updateWorldPoints();
};


#endif