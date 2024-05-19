#ifndef MATRIX3X3_CLASS_H
#define MATRIX3X3_CLASS_H

#include "../math/vector3.h"



class Matrix4x4;

class Matrix3x3
{
public:
    float data[3][3];

    // Default constructor initializes to the identity matrix
    explicit Matrix3x3(float diagonalValue = 1.0f)
    {
        for (int column = 0; column < 3; ++column)
        {
            for (int row = 0; row < 3; ++row)
            {
                data[column][row] = (row == column) ? diagonalValue : 0.0f;
            }
        }
    }

    explicit Matrix3x3(const Matrix4x4& mat4);

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
    Matrix3x3 operator*(const Matrix3x3& other) const
    {
        Matrix3x3 result(0.0f);

        for (int column = 0; column < 3; ++column)
        {
            for (int row = 0; row < 3; ++row)
            {
                result.data[column][row] = 0.0f;
                for (int k = 0; k < 3; ++k)
                {
                    result.data[column][row] += data[k][row] * other.data[column][k];
                }
            }
        }

        return result;
    }

 
    static Matrix3x3 Scale(const Matrix3x3& matrix, const Vector3& vector);

    void Transposed();
    //void Inverted();

};


#endif // MATRIX3X3_CLASS_H
