#ifndef VECTOR3_CLASS_H
#define VECTOR3_CLASS_H


#include "glm/glm.hpp"
#include "Vector2.h"




class Vector3
{
public:

	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float u, v, w; };
	};
	Vector2 xy;


	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z), xy(x,y) {}



	operator glm::vec3() const
	{

		return glm::vec3(x, y, z);
	}


	const float* value_ptr() const
	{
		return &x;
	}

	static Vector3 Cross(Vector3 a, Vector3 b);
	static Vector3 Normalize(Vector3 vector);
	static float Dot(Vector3 a, Vector3 b);

	static float Magnitude(Vector3 vector);

	static float SqrtMagnitude(Vector3 vector);

	static float Angle(Vector3 from, Vector3 to);


	static Vector3 Rotate(const Vector3& vec, float angleRadians, const Vector3& axis);



	static const Vector3 Right;
	static const Vector3 Up;
	static const Vector3 Forward;

	static const Vector3 MakeVector3(const float* pointer); // Create vector from pointer


	// Operators

	Vector3 operator * (float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}




	Vector3 operator * (const Vector3& other) const {

		float resultX = x * other.x;
		float resultY = y * other.y;
		float resultZ = z * other.z;

		return Vector3(resultX,resultY,resultZ);
	}

	Vector3 operator / (const Vector3& other) const {

		float resultX = x / other.x;
		float resultY = y / other.y;
		float resultZ = z / other.z;

		return Vector3(resultX, resultY, resultZ);

	}

	Vector3 operator + (const Vector3& other) const {

		float resultX = x + other.x;
		float resultY = y + other.y;
		float resultZ = z + other.z;

		return Vector3(resultX, resultY, resultZ);

	}

	Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}


	Vector3 operator-() const {
		return { -x, -y, -z };
	}

	Vector3 operator - (const Vector3& other) const {

		float resultX = x - other.x;
		float resultY = y - other.y;
		float resultZ = z - other.z;

		return Vector3(resultX, resultY, resultZ);

	}

	bool operator == (const Vector3& other) const {

		return x == other.x && y == other.y && z == other.z;

	}

	bool operator != (const Vector3& other) const {

		return x != other.x || y != other.y || z != other.z;

	}



};

Vector3 operator*(float scalar, const Vector3& vector);

#endif