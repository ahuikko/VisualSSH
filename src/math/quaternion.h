#ifndef QUATERNION_CLASS_H
#define QUATERNION_CLASS_H


#include "../math/vector3.h"



class Matrix4x4;


class Quaternion
{

public:
	float w, x, y, z;


	Quaternion();

	Quaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}


	Matrix4x4 ToMatrix4x4() const;


	static const Quaternion MakeQuaternion(const float* pointer); // Create vector from pointer


	const float* value_ptr() const
	{
		return &w;
	}



	//Static methods reflecting Unity 

	static double Angle(Quaternion a, Quaternion b);

	static double AngleAxis(Quaternion a, Quaternion b);


	static Quaternion FromAxisAngle(const Vector3& axis, float angle);



	static double Dot(Quaternion a, Quaternion b);

	static Quaternion Euler(float x, float y, float z);

	static Quaternion FromToRotation(Quaternion a, Quaternion b);

	static Quaternion Inverse(const Quaternion& rotation);

	static Quaternion Lerp(Quaternion a, Quaternion b, float t);

	static double LerpUnclamped(Quaternion a, Quaternion b, float t);

	static Quaternion LookRotation(Vector3 forward, Vector3 up);

	static Quaternion Normalize(Quaternion quat);

	Quaternion Conjugate() const;


	Quaternion& operator*=(const Quaternion& rhs)
	{
		// Assuming you have a * operator defined, you can implement *= like this
		*this = *this * rhs; // Where *this * rhs is your quaternion multiplication
		return *this;
	}

	// Your quaternion multiplication method
	Quaternion operator*(const Quaternion& rhs) const
	{
		// Quaternion multiplication logic
		return Quaternion(
			w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
			w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
			w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
			w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w
		);
	}

		
	static const Quaternion Identity;

	static Vector3 Rotate(const Quaternion& q, const Vector3& v)
	{
		// Convert vector to quaternion with w = 0
		Quaternion vecQuat(0, v.x, v.y, v.z);

		// Rotate the vector (convert it to a quaternion, multiply, and convert back)
		Quaternion rotatedQuat = q * vecQuat * q.Conjugate();

		// Return the rotated vector, dropping the w component
		return Vector3(rotatedQuat.x, rotatedQuat.y, rotatedQuat.z);
	}

		

};

	


#endif
