#include "matrix4x4.h"

#include "../math/hmath_utils.h"



Matrix3x3 Matrix4x4::ToMatrix3x3() const
{
    Matrix3x3 result;

    // Copy the upper-left 3x3 elements
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            result[i][j] = (*this)[i][j];
        }
    }

    return result;
}

const Matrix4x4 Matrix4x4::MakeMatrix4x4(const float* pointer)
{
    assert(pointer != nullptr); // Optional: Ensure the pointer is not null.
    Matrix4x4 result;

    memcpy(const_cast<float*>(result.value_ptr()), pointer, sizeof(Matrix4x4));
    return result;

}

Matrix4x4 Matrix4x4::RemoveTranslation(const Matrix4x4& viewMatrix)
{
        
    Matrix3x3 rotationScaleMat(viewMatrix);
    Matrix4x4 rotationMatrix(rotationScaleMat);
    return rotationMatrix;
        
}

Matrix4x4 Matrix4x4::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
        
    Vector3 f = Vector3::Normalize(center - eye);
    Vector3 s = Vector3::Normalize(Vector3::Cross(f, up));
    Vector3 u = Vector3::Cross(s, f);

    Matrix4x4 result(1.f);
    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;
    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;
    result[0][2] = -f.x;
    result[1][2] = -f.y;
    result[2][2] = -f.z;
    result[3][0] = -Vector3::Dot(s, eye);
    result[3][1] = -Vector3::Dot(u, eye);
    result[3][2] = Vector3::Dot(f, eye);

    return result;
        
}

Matrix4x4 Matrix4x4::Perspective(float FOVdeg, float aspectRatio, float nearPlane, float farPlane)
{
        
    // Calculate the necessary values for the perspective matrix
    float tanHalfFOV = tan(radians(FOVdeg / 2.0f));
    float zRange = farPlane - nearPlane;

    Matrix4x4 mat(0.0f);
    mat[0][0] = 1.0f / (aspectRatio * tanHalfFOV);
    mat[1][1] = 1.0f / tanHalfFOV;
    mat[2][2] = -(farPlane + nearPlane) / zRange;
    mat[2][3] = -1.0f;
    mat[3][2] = -(2.0f * farPlane * nearPlane) / zRange;
    return mat;
}

Matrix4x4 Matrix4x4::Scale(const Matrix4x4& matrix, const Vector3& vector)
{
    Matrix4x4 scaleMatrix(1.0f); // Initialize to identity matrix

    // Set the diagonal elements to the scale factors
    scaleMatrix[0][0] = vector.x;
    scaleMatrix[1][1] = vector.y;
    scaleMatrix[2][2] = vector.z;

    return scaleMatrix;
}

Matrix4x4 Matrix4x4::Translate(const Matrix4x4& matrix, const Vector3& vector)
{
    Matrix4x4 result(matrix); // Copy the input matrix

    // Apply the translation
    result[3][0] = matrix[0][0] * vector.x + matrix[1][0] * vector.y + matrix[2][0] * vector.z + matrix[3][0];
    result[3][1] = matrix[0][1] * vector.x + matrix[1][1] * vector.y + matrix[2][1] * vector.z + matrix[3][1];
    result[3][2] = matrix[0][2] * vector.x + matrix[1][2] * vector.y + matrix[2][2] * vector.z + matrix[3][2];
    result[3][3] = matrix[0][3] * vector.x + matrix[1][3] * vector.y + matrix[2][3] * vector.z + matrix[3][3];

    return result;
}

void Matrix4x4::Transposed()
{
    Matrix4x4 temp = *this;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            data[i][j] = temp.data[j][i];
        }
    }
}

void Matrix4x4::Inverted()
{
    // This is a simplified version of the inversion algorithm that only works
    // for affine transformation matrices (translation, rotation, scaling).
    // For a general 4x4 matrix, you would need to implement a full inversion algorithm.

    // Separate the matrix into its translation and rotation/scale components
    Vector3 translation(data[3][0], data[3][1], data[3][2]);
    Matrix3x3 rotationScale(*this); // Assuming you have a constructor of Matrix3x3 that takes a Matrix4x4

    // Invert the rotation/scale part by transposing it
    rotationScale.Transposed(); // Assuming you have a Transposed method for Matrix3x3

    // Invert the translation
    translation = -translation;

    // Reassemble the inverted matrix
    *this = Matrix4x4(rotationScale); // Convert back to 4x4 matrix
    this->Translate(*this, translation); // Apply the inverted translation
}
