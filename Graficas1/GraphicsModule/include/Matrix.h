#pragma once
class Matrix
{
public:
	Matrix();
	~Matrix();

	static float* MatrixMultiplication(const float* _MatrixA, const float* _MatrixB);
};

