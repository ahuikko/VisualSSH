#ifndef VECTOR4_CLASS_H
#define VECTOR4_CLASS_H


#include "glm/glm.hpp"


class Vector4
{
public:
	float x, y, z, w;

	Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w){}

	operator glm::vec4() const
	{
		return glm::vec4(x, y, z, w);
	}


	const float* value_ptr() const
	{
		return &x;
	}

	static Vector4 Normalize(Vector4 vector);

	static float Magnitude(Vector4 vector);
	static float SqrtMagnitude(Vector4 vector);


	static const Vector4 MakeVector4(const float* pointer); // Create vector from pointer


	// Operators

	Vector4 operator * (float scalar) const
	{
		return { x * scalar, y * scalar, z * scalar, w * scalar };
	}




	Vector4 operator * (const Vector4& other) const
	{

		float resultX = x * other.x;
		float resultY = y * other.y;
		float resultZ = z * other.z;
		float resultW = w * other.w;
		return Vector4(resultX, resultY, resultZ, resultW);
	}

	Vector4 operator / (const Vector4& other) const
	{

		float resultX = x / other.x;
		float resultY = y / other.y;
		float resultZ = z / other.z;
		float resultW = w / other.w;
		return Vector4(resultX, resultY, resultZ, resultW);

	}

	Vector4 operator + (const Vector4& other) const
	{

		float resultX = x + other.x;
		float resultY = y + other.y;
		float resultZ = z + other.z;
		float resultW = w + other.w;
		return Vector4(resultX, resultY, resultZ, resultW);

	}

	Vector4& operator+=(const Vector4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}
	Vector4& operator-=(const Vector4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}


	Vector4 operator-() const
	{
		return { -x, -y, -z, -w };
	}

	Vector4 operator - (const Vector4& other) const
	{

		float resultX = x - other.x;
		float resultY = y - other.y;
		float resultZ = z - other.z;
		float resultW = w - other.w;

		return Vector4(resultX, resultY, resultZ, resultW);

	}

	bool operator == (const Vector4& other) const
	{

		return x == other.x && y == other.y && z == other.z && w == other.w;

	}

	bool operator != (const Vector4& other) const
	{

		return x != other.x || y != other.y || z != other.z || w != other.w;

	}



};

Vector4 operator*(float scalar, const Vector4& vector);


#endif