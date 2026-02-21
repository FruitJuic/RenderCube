#ifndef PROJECTION_H
#define PROJECTION_H

#include "points.h"
#include <vector>

float projectedCoordinate(float, float);
Point projectPoint(Point);
std::vector<Point> projectPoints(std::vector<Point>);

#endif