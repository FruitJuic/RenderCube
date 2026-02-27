#include "points.h"
#include <vector>

float FOCAL_LENGTH = 500.0f;

float projectedCoordinate(float coordinate, float z)
{
	/*
		Projection formula: x projected = (focal length * x) / (focal length + z)
	*/

	return (coordinate * FOCAL_LENGTH) / (FOCAL_LENGTH + z);
}

Point projectPoint(Point point)
{
	return Point(projectedCoordinate(point.x, point.z),projectedCoordinate(point.y, point.z),0);
}

std::vector<Point> projectPoints(std::vector<Point> points)
{
	std::vector<Point> projectedPoints;
	for (int i = 0; i < points.size(); i++)
	{
		projectedPoints.push_back(projectPoint(points[i]));
	}
	return projectedPoints;
}