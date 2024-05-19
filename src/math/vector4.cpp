#include "vector4.h"
#include <iostream>
#include "../math/hmath_utils.h"
#include <cmath>



Vector4 operator*(float scalar, const Vector4& vector)
{
	return vector * scalar; // This calls the member operator* defined in the class.
}

Vector4 Vector4::Normalize(Vector4 vector)
{
	float mag = Magnitude(vector);
	if (mag == 0)
	{
		std::cerr << "Error: Cannot normalize a zero-magnitude vector." << std::endl;
		return Vector4(0, 0, 0); // Or handle the zero vector case as you see fit
	}
	return Vector4(vector.x / mag, vector.y / mag, vector.z / mag);
}

float Vector4::Magnitude(Vector4 vector)
{
	return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;

}

float Vector4::SqrtMagnitude(Vector4 vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);;
}




const Vector4 Vector4::MakeVector4(const float* pointer)
{
	Vector4 result;
	memcpy(const_cast<float*>(result.value_ptr()), pointer, sizeof(Vector4));
	return result;
}





