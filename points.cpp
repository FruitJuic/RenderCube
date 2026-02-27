#include "points.h"
#include <cmath>

Point::Point(float xVal, float yVal, float zVal)
{
	x = xVal;
	y = yVal;
	z = zVal;
}

void Point::Translate(float xVal, float yVal, float zVal)
{
	x += xVal;
	y += yVal;
	z += zVal;
}

void Point::Rotate(float xAngle, float yAngle, float zAngle, float pivotX, float pivotY, float pivotZ)
{
	// Rotate around vertical axis
	float cosY = cos(xAngle);
	float sinY = sin(xAngle);

	float newX = cosY * (x - pivotX) + sinY * (z - pivotZ) + pivotX;
	float newZ = -sinY * (x - pivotX) + cosY * (z - pivotZ) + pivotZ;
	x = newX;
	z = newZ;

	// Rotate around horizontal axis
	float cosX = cos(yAngle);
	float sinX = sin(yAngle);

	float newY = cosX * (y - pivotY) - sinX * (z - pivotZ) + pivotY;
	newZ = sinX * (y - pivotY) + cosX * (z - pivotZ) + pivotZ;
	y = newY;
	z = newZ;

	// Rotate around depth axis
	float cosZ = cos(zAngle);
	float sinZ = sin(zAngle);

	newX = cosZ * (x - pivotX) - sinZ * (y - pivotY) + pivotX;
	newY = sinZ * (x - pivotX) + cosZ * (y - pivotY) + pivotY;
	x = newX;
	y = newY;
}

void Point::Scale(float xScale, float yScale, float zScale, float pivotX, float pivotY, float pivotZ)
{
	x = pivotX + (x - pivotX) * xScale;
	y = pivotY + (y - pivotY) * yScale;
	z = pivotZ + (z - pivotZ) * zScale;
}

bool Point::Equals(Point other)
{
	return x == other.x && y == other.y && z == other.z;
}