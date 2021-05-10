#include "Matrix.h"

Matrix::Matrix()
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			if (i == j)
			{
				m_Matrix[i][j] = 1.0f;
			}
			else
			{
				m_Matrix[i][j] = 0.0f;
			}
		}
	}
}

Matrix::~Matrix()
{
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

Matrix Matrix::Transpose(const Matrix _Matrix)
{
	Matrix Temp;
	Temp.m_Matrix[0][0] = _Matrix.m_Matrix[0][0];
	Temp.m_Matrix[0][1] = _Matrix.m_Matrix[1][0];
	Temp.m_Matrix[0][2] = _Matrix.m_Matrix[2][0];
	Temp.m_Matrix[0][3] = _Matrix.m_Matrix[3][0];

	Temp.m_Matrix[1][0] = _Matrix.m_Matrix[0][1];
	Temp.m_Matrix[1][1] = _Matrix.m_Matrix[1][1];
	Temp.m_Matrix[1][2] = _Matrix.m_Matrix[2][1];
	Temp.m_Matrix[1][3] = _Matrix.m_Matrix[3][1];

	Temp.m_Matrix[2][0] = _Matrix.m_Matrix[0][2];
	Temp.m_Matrix[2][1] = _Matrix.m_Matrix[1][2];
	Temp.m_Matrix[2][2] = _Matrix.m_Matrix[2][2];
	Temp.m_Matrix[2][3] = _Matrix.m_Matrix[3][2];

	Temp.m_Matrix[3][0] = _Matrix.m_Matrix[0][3];
	Temp.m_Matrix[3][1] = _Matrix.m_Matrix[1][3];
	Temp.m_Matrix[3][2] = _Matrix.m_Matrix[2][3];
	Temp.m_Matrix[3][3] = _Matrix.m_Matrix[3][3];

	return Temp;
}

void Matrix::MatrixTo1D(const Matrix _Matrix, float* _Arr)
{
	int Counter = 0;
	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			_Arr[Counter] = _Matrix.m_Matrix[i][j];
			Counter++;
		}
	}
}

void Matrix::operator=(const Matrix& _MatrixA)
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			m_Matrix[i][j] = _MatrixA.m_Matrix[i][j];
		}
	}
}

void Matrix::operator=(const float* _MatrixA)
{
	int Counter = 0;
	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			m_Matrix[i][j] = _MatrixA[Counter];
			Counter++;
		}
	}
}

Matrix& Matrix::operator*=(const float* _MatrixA)
{
	Matrix MatrixA;
	Matrix Result;
	unsigned int Counter = 0;
	int Row = 0;

	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			MatrixA.m_Matrix[i][j] = _MatrixA[Counter];
			Counter++;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Result.m_Matrix[Row][i] =
				(MatrixA.m_Matrix[0][i] * m_Matrix[Row][0])
				+ (MatrixA.m_Matrix[1][i] * m_Matrix[Row][1])
				+ (MatrixA.m_Matrix[2][i] * m_Matrix[Row][2])
				+ (MatrixA.m_Matrix[3][i] * m_Matrix[Row][3]);
			Row++;
		}
		Row = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_Matrix[i][j] = Result.m_Matrix[i][j];
		}
	}

	return Result;
}

Matrix& Matrix::operator*(const float* _MatrixA)
{
	Matrix Result(*this);
	return Result *= _MatrixA;
}

Matrix& Matrix::operator*=(const Matrix& _MatrixA)
{
	Matrix Result;
	int Row = 0;
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Result.m_Matrix[Row][i] =
				(_MatrixA.m_Matrix[0][i] * m_Matrix[Row][0])
				+ (_MatrixA.m_Matrix[1][i] * m_Matrix[Row][1])
				+ (_MatrixA.m_Matrix[2][i] * m_Matrix[Row][2])
				+ (_MatrixA.m_Matrix[3][i] * m_Matrix[Row][3]);
			Row++;
		}
		Row = 0;
	}
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_Matrix[i][j] = Result.m_Matrix[i][j];
		}
	}
	
	return Result;
}

Matrix& Matrix::operator*(const Matrix& _MatrixA)
{	
	Matrix Result (*this);
	return Result *= _MatrixA;
}
