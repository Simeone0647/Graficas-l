#include "Matrix.h"

Matrix::Matrix()
{

}

Matrix::~Matrix()
{

}

void Matrix::MatrixMultiplication(const float* _MatrixA, const float* _MatrixB, float* _Result)
{
    int Counter = 0;
    int NumberOfRow = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            _Result[Counter] = (_MatrixA[NumberOfRow] * _MatrixB[j])
                               + (_MatrixA[NumberOfRow + 1] * _MatrixB[j + 4])
                               + (_MatrixA[NumberOfRow + 2] * _MatrixB[j + 8])
                               + (_MatrixA[NumberOfRow + 3] * _MatrixB[j + 12]);
            Counter++;
        }
        NumberOfRow += 4;
    }
}

void Matrix::SetIdentityMatrix(float* _Matrix)
{
	_Matrix[0] = 1.0f;
	_Matrix[1] = 0.0f;
	_Matrix[2] = 0.0f;
	_Matrix[3] = 0.0f;

	_Matrix[4] = 0.0f;
	_Matrix[5] = 1.0f;
	_Matrix[6] = 0.0f;
	_Matrix[7] = 0.0f;

	_Matrix[8] = 0.0f;
	_Matrix[9] = 0.0f;
	_Matrix[10] = 1.0f;
	_Matrix[11] = 0.0f;

	_Matrix[12] = 0.0f;
	_Matrix[13] = 0.0f;
	_Matrix[14] = 0.0f;
	_Matrix[15] = 1.0f;
}
