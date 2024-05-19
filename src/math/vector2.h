#ifndef VECTOR2_CLASS_H
#define VECTOR2_CLASS_H


#include "glm/glm.hpp"


class Vector2
{
public:
	union
	{
		struct { float x, y; };
		struct { float r, g; };
		struct { float u, v; };
	};

	Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	operator glm::vec2() const
	{
		return glm::vec2(x, y);
	}


	const float* value_ptr() const
	{
		return &x;
	}

	static Vector2 Cross(Vector2 a, Vector2 b);
	static Vector2 Normalize(Vector2 vector);
	static float Dot(Vector2 a, Vector2 b);

	static float Magnitude(Vector2 vector);

	static float Angle(Vector2 from, Vector2 to);


	static Vector2 Rotate(const Vector2& vec, float angleRadians);


	static const Vector2 MakeVector2(const float* pointer); // Create vector from pointer


	// Operators

	Vector2 operator * (float scalar) const
	{
		return { x * scalar, y * scalar};
	}




	Vector2 operator * (const Vector2& other) const
	{

		float resultX = x * other.x;
		float resultY = y * other.y;

		return Vector2(resultX, resultY);
	}

	Vector2 operator / (const Vector2& other) const
	{

		float resultX = x / other.x;
		float resultY = y / other.y;

		return Vector2(resultX, resultY);

	}

	Vector2 operator + (const Vector2& other) const
	{

		float resultX = x + other.x;
		float resultY = y + other.y;

		return Vector2(resultX, resultY);

	}

	Vector2& operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	Vector2& operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}


	Vector2 operator-() const
	{
		return { -x, -y};
	}

	Vector2 operator - (const Vector2& other) const
	{

		float resultX = x - other.x;
		float resultY = y - other.y;

		return Vector2(resultX, resultY);

	}

	bool operator == (const Vector2& other) const
	{

		return x == other.x && y == other.y;

	}

	bool operator != (const Vector2& other) const
	{

		return x != other.x || y != other.y;

	}

};

Vector2 operator*(float scalar, const Vector2& vector);


#endif