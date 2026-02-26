#include "points.h"
#include <vector>

float FOCAL_LENGTH = 500.0f;

float projectedCoordinate(float coordinate, float z, int localWidth)
{
	/*
		Projection formula: x projected = (focal length * x) / (focal length + z)
	*/

	return (coordinate * FOCAL_LENGTH) / (FOCAL_LENGTH + z);
}

Point projectPoint(Point point, int localWidth)
{
	return Point(projectedCoordinate(point.x, point.z, localWidth),projectedCoordinate(point.y, point.z, localWidth),0);
}

std::vector<Point> projectPoints(std::vector<Point> points, int localWidth)
{
	std::vector<Point> projectedPoints;
	for (int i = 0; i < points.size(); i++)
	{
		projectedPoints.push_back(projectPoint(points[i], localWidth));
	}
	return projectedPoints;
}