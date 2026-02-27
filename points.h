#ifndef POINTS_H
#define POINTS_H

struct Vec3 {
	float x;
	float y;
	float z;
};

class Point
{
public:
	float x;
	float y;
	float z;
	Point(float xVal, float yVal, float zVal);
	Point() = default;
	void Translate(float xVal, float yVal, float zVal);
	void Rotate(float xAngle, float yAngle, float zAngle, float pivotX, float pivotY, float pivotZ);
	void Scale(float xScale, float yScale, float zScale, float pivotX, float pivotY, float pivotZ);
	bool Equals(Point);
};

#endif