#pragma once
class Matrix
{
public:
	Matrix();
	~Matrix();

	static void MatrixMultiplication(const float* _MatrixA, const float* _MatrixB, float* _Result);
};

