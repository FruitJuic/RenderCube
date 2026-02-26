#ifndef PROJECTION_H
#define PROJECTION_H

#include "points.h"
#include <vector>

float projectedCoordinate(float, float, int);
Point projectPoint(Point, int);
std::vector<Point> projectPoints(std::vector<Point>, int);

#endif