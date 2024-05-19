#include "matrix3x3.h"

#include "../math/hmath_utils.h"

#include "matrix4x4.h"
#include <algorithm>





Matrix3x3::Matrix3x3(const Matrix4x4& mat4)
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            data[i][j] = mat4[i][j];
        }
    }
}

Matrix3x3 Matrix3x3::Scale(const Matrix3x3& matrix, const Vector3& vector)
{
    Matrix3x3 scaleMatrix(1.0f); // Initialize to identity matrix

    // Set the diagonal elements to the scale factors
    scaleMatrix[0][0] = vector.x;
    scaleMatrix[1][1] = vector.y;
    scaleMatrix[2][2] = vector.z;

    return scaleMatrix;
}

void Matrix3x3::Transposed()
{
    // A simple in-place transpose for a 3x3 matrix
    std::swap(data[0][1], data[1][0]);
    std::swap(data[0][2], data[2][0]);
    std::swap(data[1][2], data[2][1]);

}


