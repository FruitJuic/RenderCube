#include "points.h"

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
	
}

void Point::Scale(float xScale, float yScale, float zScale, float pivotX, float pivotY, float pivotZ)
{
	x = pivotX + (x - pivotX) * xScale;
	y = pivotY + (y - pivotY) * yScale;
	z = pivotZ + (z - pivotZ) * zScale;
}