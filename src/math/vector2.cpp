#include "vector2.h"
#include <iostream>
#include "../math/hmath_utils.h"
#include <cmath>




Vector2 operator*(float scalar, const Vector2& vector)
{
	return vector * scalar; // This calls the member operator* defined in the class.
}

Vector2 Vector2::Normalize(Vector2 vector)
{
	float mag = Magnitude(vector);
	if (mag == 0)
	{
		std::cerr << "Error: Cannot normalize a zero-magnitude vector." << std::endl;
		return Vector2(0, 0); // Or handle the zero vector case as you see fit
	}
	return Vector2(vector.x / mag, vector.y / mag);
}

float Vector2::Dot(Vector2 a, Vector2 b)
{
	return a.x * b.x + a.y * b.y;
}

float Vector2::Magnitude(Vector2 vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);

}

float Vector2::Angle(Vector2 from, Vector2 to)
{
	// Normalize vectors to ensure accurate angle calculation
	Vector2 normFrom = Normalize(from);
	Vector2 normTo = Normalize(to);

	// Calculate the dot product of normalized vectors
	float dot = Dot(normFrom, normTo);

	// Clamp the dot product to avoid precision errors
	dot = fmax(fmin(dot, 1.0), -1.0);

	// Calculate the angle in radians
	float angleInRadians = acos(dot);
	// Convert the angle to degrees
	float angleInDegrees = angleInRadians * (180.0f / PI);

	return angleInDegrees;
}

Vector2 Vector2::Rotate(const Vector2& vec, float angleRadians)
{
	float cosTheta = cos(angleRadians);
	float sinTheta = sin(angleRadians);

	return Vector2(
		vec.x * cosTheta - vec.y * sinTheta,
		vec.x * sinTheta + vec.y * cosTheta
	);
}

const Vector2 Vector2::MakeVector2(const float* pointer)
{
	Vector2 result;
	memcpy(const_cast<float*>(result.value_ptr()), pointer, sizeof(Vector2));
	return result;
}



