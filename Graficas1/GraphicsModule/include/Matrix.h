#pragma once
class Matrix
{
public:
	Matrix();
	~Matrix();

	static void SetIdentity(float* _Matrix);
	static Matrix Transpose(const Matrix _Matrix);

	void operator=(const Matrix& _MatrixA);

	Matrix& operator*=(const Matrix& _MatrixA);

	Matrix& operator*(const Matrix& _MatrixA);

	float m_Matrix[4][4];
};

