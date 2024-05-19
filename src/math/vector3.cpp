#include "vector3.h"
#include <iostream>
#include "../math/hmath_utils.h"
#include <cmath>


const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);

Vector3 operator*(float scalar, const Vector3& vector) {
	return vector * scalar; // This calls the member operator* defined in the class.
}


Vector3 Vector3::Cross(Vector3 a, Vector3 b)
{

	float crossX = a.y * b.z - a.z * b.y;
	float crossY = a.z * b.x - a.x * b.z;
	float crossZ = a.x * b.y - a.y * b.x;


	return Vector3(crossX, crossY, crossZ);
}

Vector3 Vector3::Normalize(Vector3 vector) {
	float mag = Magnitude(vector);
	if (mag == 0) {
		std::cerr << "Error: Cannot normalize a zero-magnitude vector." << std::endl;
		return Vector3(0, 0, 0); // Or handle the zero vector case as you see fit
	}
	return Vector3(vector.x / mag, vector.y / mag, vector.z / mag);
}

float Vector3::Dot(Vector3 a, Vector3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Vector3::Magnitude(Vector3 vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

}
float Vector3::SqrtMagnitude(Vector3 vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

}

float Vector3::Angle(Vector3 from, Vector3 to) {
	// Normalize vectors to ensure accurate angle calculation
	Vector3 normFrom = Normalize(from);
	Vector3 normTo = Normalize(to);

	// Calculate the dot product of normalized vectors
	float dot = Dot(normFrom, normTo);

	// Clamp the dot product to avoid precision errors
	dot = fmax(fmin(dot, 1.0), -1.0);

	// Calculate the angle in radians
	float angleInRadians = acos(dot);
	// Convert the angle to degrees
	float angleInDegrees = angleInRadians * (180.0 / PI);

	return angleInDegrees;
}

Vector3 Vector3::Rotate(const Vector3& vec, float angleRadians, const Vector3& axis)
{
		
	// Normalize the axis to ensure it's a unit vector
	Vector3 normalizedAxis = Normalize(axis);

	// Rodrigues' rotation formula
	Vector3 rotatedVector = vec * cos(angleRadians) +
		Cross(normalizedAxis, vec) * sin(angleRadians) +
		normalizedAxis * Dot(normalizedAxis, vec) * (1 - cos(angleRadians));

	return rotatedVector;
}

const Vector3 Vector3::MakeVector3(const float* pointer)
{
	Vector3 result;
	memcpy(const_cast<float*>(result.value_ptr()), pointer, sizeof(Vector3));
	return result;
}


