#include "Matrix.h"

Matrix::Matrix()
{

}

Matrix::~Matrix()
{

}

void Matrix::Multiplication(const float* _MatrixA, const float* _MatrixB, float* _Result)
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

void Matrix::SetIdentity(float* _Matrix)
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

void Matrix::Copy(float* _Source, float* _Destination)
{
	_Destination[0] = _Source[0];
	_Destination[1] = _Source[1];
	_Destination[2] = _Source[2];
	_Destination[3] = _Source[3];
	_Destination[4] = _Source[4];
	_Destination[5] = _Source[5];
	_Destination[6] = _Source[6];
	_Destination[7] = _Source[7];
	_Destination[8] = _Source[8];
	_Destination[9] = _Source[9];
	_Destination[10] = _Source[10];
	_Destination[11] = _Source[11];
	_Destination[12] = _Source[12];
	_Destination[13] = _Source[13];
	_Destination[14] = _Source[14];
	_Destination[15] = _Source[15];
}
