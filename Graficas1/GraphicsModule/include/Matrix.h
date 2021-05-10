#pragma once
class Matrix
{
public:
	Matrix();
	~Matrix();

	static void SetIdentity(float* _Matrix);
	static Matrix Transpose(const Matrix _Matrix);
	static void MatrixTo1D(const Matrix _Matrix, float* _Arr);

	void operator=(const Matrix& _MatrixA);

	void operator=(const float* _MatrixA);

	Matrix& operator*=(const float* _MatrixA);

	Matrix& operator*(const float* _MatrixA);

	Matrix& operator*=(const Matrix& _MatrixA);

	Matrix& operator*(const Matrix& _MatrixA);

	float m_Matrix[4][4];
};

