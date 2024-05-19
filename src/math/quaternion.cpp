#include "quaternion.h"
#include <iostream>
#include "../math/hmath_utils.h"
#include "matrix4x4.h"


const Quaternion Quaternion::Identity = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);



Quaternion::Quaternion()
{
}

const Quaternion Quaternion::MakeQuaternion(const float* pointer)
{
    assert(pointer != nullptr); 
    Quaternion result;

    memcpy(const_cast<float*>(result.value_ptr()), pointer, sizeof(Quaternion));
    return result;

}

Matrix4x4 Quaternion::ToMatrix4x4() const
{
        
    // Ensure the quaternion is normalized
    Quaternion q = Normalize(*this);

    Matrix4x4 result;

    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;
    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;

    // First row
    result[0][0] = 1.0f - 2.0f * (yy + zz);
    result[1][0] = 2.0f * (xy - wz);
    result[2][0] = 2.0f * (xz + wy);
    result[3][0] = 0.0f;

    // Second row
    result[0][1] = 2.0f * (xy + wz);
    result[1][1] = 1.0f - 2.0f * (xx + zz);
    result[2][1] = 2.0f * (yz - wx);
    result[3][1] = 0.0f;

    // Third row
    result[0][2] = 2.0f * (xz - wy);
    result[1][2] = 2.0f * (yz + wx);
    result[2][2] = 1.0f - 2.0f * (xx + yy);
    result[3][2] = 0.0f;

    // Fourth row
    result[0][3] = 0.0f;
    result[1][3] = 0.0f;
    result[2][3] = 0.0f;
    result[3][3] = 1.0f;

    return result;

}

double Quaternion::Angle(Quaternion a, Quaternion b)
{
    return 0.0;
}

double Quaternion::AngleAxis(Quaternion a, Quaternion b)
{
    return 0.0;
}

Quaternion Quaternion::FromAxisAngle(const Vector3& axis, float angle)
{
    // Normalize the axis
    Vector3 normalizedAxis = Vector3::Normalize(axis);

    // Convert the angle from degrees to radians
    float radians = angle * (PI / 180.0f);

    // Compute the sin and cos of half the angle
    float sinHalfAngle = sin(radians / 2.0f);
    float cosHalfAngle = cos(radians / 2.0f);

    // Create the quaternion
    return Quaternion(
        cosHalfAngle,
        normalizedAxis.x * sinHalfAngle,
        normalizedAxis.y * sinHalfAngle,
        normalizedAxis.z * sinHalfAngle
    );
}


double Quaternion::Dot(Quaternion a, Quaternion b)
{
    return 0.0;
}

Quaternion Quaternion::Euler(float x, float y, float z)
{
    return Quaternion::Identity;
}

Quaternion Quaternion::FromToRotation(Quaternion a, Quaternion b)
{
    return Quaternion::Identity;
}

Quaternion Quaternion::Inverse(const Quaternion& rotation)
{


    double norm = rotation.w * rotation.w + rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z;

    if (norm == 0.0f) {

        return Quaternion::Identity;  // Return an identity quaternion
    }

    double invNorm = 1.0f / norm;
    return Quaternion(rotation.w * invNorm, -rotation.x * invNorm, -rotation.y * invNorm, -rotation.z * invNorm);


}

Quaternion Quaternion::Lerp(Quaternion a, Quaternion b, float t)
{
    return Quaternion::Identity;
}

double Quaternion::LerpUnclamped(Quaternion a, Quaternion b, float t)
{
    return 0.0;
}

Quaternion Quaternion::LookRotation(Vector3 forward, Vector3 up)
{

        

    Vector3 z = Vector3::Normalize(forward);
    Vector3 x = Vector3::Normalize(Vector3::Cross(up, z));
    Vector3 y = Vector3::Cross(z, x);


    //Quaternion lookRot;


    return Quaternion::Identity;

}

Quaternion Quaternion::Normalize(Quaternion quat)
{
    double norm = sqrt(quat.w * quat.w + quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);

    if (norm == 0.0) {
        std::cerr << "Error: Cannot normalize a zero-magnitude quaternion." << std::endl;
        return Quaternion::Identity;  // Return an identity quaternion in case of error
    }

    return Quaternion(quat.w / norm, quat.x / norm, quat.y / norm, quat.z / norm);

}

Quaternion Quaternion::Conjugate() const
{
    return Quaternion(w, -x, -y, -z);
}



