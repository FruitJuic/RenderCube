#ifndef SHAPES_H
#define SHAPES_H

#include "points.h"
#include <vector>

enum ShapeType {
	CUBE,
	PYRAMID
}; 

class Shape
{
public:
	unsigned int getVertex(Point, int, int);
	int indicesSize() { return indices.size(); }
	Shape(Point, Point, ShapeType);
	void Rotate(Point);
	void Move(Point);
	void Scale(Point);
	void SetRotation(Point);
	void SetPosition(Point);
	void SetScale(Point);
	void ResetRotation();
	void ResetPosition();
	void ResetScale();
private:
	std::vector<Point> points;
	std::vector<Point> worldPoints;
	std::vector<Point> projectedPoints;
	std::vector<unsigned int> indices;
	Point initialPosition;
	Point initialSize;
	Point initialRotation;
	Point position;
	Point size;
	Point rotation;
	Point playerPosition;
	int localWidth;
	int localHeight;
	Point bufferPosition;
	Point bufferSize;
	Point bufferRotation;
	Point bufferPlayerPosition;
	int bufferWidth;
	int bufferHeight;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	void updateBuffer();
	void updateWorldPoints();
};


#endif