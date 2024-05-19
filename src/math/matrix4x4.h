#ifndef MATRIX4X4_CLASS_H
#define MATRIX4X4_CLASS_H

#include "../math/vector3.h"
#include "Matrix3x3.h"

class Matrix4x4 {
public:
    float data[4][4];

    // Default constructor initializes to the identity matrix
    explicit Matrix4x4(float diagonalValue = 1.0f)
    {
        for (int column = 0; column < 4; ++column)
        {
            for (int row = 0; row < 4; ++row)
            {
                data[column][row] = (row == column) ? diagonalValue : 0.0f;
            }
        }
    }

    explicit Matrix4x4(const Matrix3x3& mat3)
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                data[i][j] = mat3[i][j];
            }
            data[i][3] = 0;
            data[3][i] = 0;
        }
        data[3][3] = 1.0f;
    }


    // Access elements as matrix[column][row]
    float* operator[](int column)
    {
        return data[column];
    }

    const float* operator[](int column) const
    {
        return data[column];
    }

    // Return a pointer to the matrix data in column-major order
    const float* value_ptr() const
    {
        return &data[0][0];
    }

    // Matrix multiplication assuming column-major order
    Matrix4x4 operator*(const Matrix4x4& other) const
    {
        Matrix4x4 result(0.0f);

        for (int column = 0; column < 4; ++column)
        {
            for (int row = 0; row < 4; ++row)
            {
                result.data[column][row] = 0.0f;
                for (int k = 0; k < 4; ++k)
                {
                    result.data[column][row] += data[k][row] * other.data[column][k];
                }
            }
        }

        return result;
    }

    operator glm::mat4() const
    {
        glm::mat4 glmMatrix;
        for (int column = 0; column < 4; ++column)
        {
            for (int row = 0; row < 4; ++row)
            {
                glmMatrix[column][row] = data[column][row];
            }
        }
        return glmMatrix;
    }



    Matrix3x3 ToMatrix3x3() const;

    static const Matrix4x4 MakeMatrix4x4(const float* pointer); // Create vector from pointer


    static Matrix4x4 RemoveTranslation(const Matrix4x4& viewMatrix);
        
    // Static functions for transformations
    static Matrix4x4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
    static Matrix4x4 Perspective(float FOVdeg, float aspectRatio, float nearPlane, float farPlane);

    static Matrix4x4 Scale(const Matrix4x4& matrix, const Vector3& vector);
    static Matrix4x4 Translate(const Matrix4x4& matrix, const Vector3& vector);


    void Transposed();

    void Inverted();





};



#endif // MATRIX4X4_CLASS_H
