#pragma once
class Matrix
{
public:
	Matrix();
	~Matrix();

	static void Multiplication(const float* _MatrixA, const float* _MatrixB, float* _Result);
	static void SetIdentity(float* _Matrix);
	static void Copy(float* _Source, float* _Destination);
	static void Transpose(float* _Matrix);

	Matrix operator*(const Matrix& _MatrixA)
	{
		Matrix MatrixB;
		Matrix Result;
		int Row = 0;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Result.m_Matrix[Row][i] = 
					(_MatrixA.m_Matrix[0][i] * MatrixB.m_Matrix[Row][0])
					+ (_MatrixA.m_Matrix[1][i] * MatrixB.m_Matrix[Row][1])
					+ (_MatrixA.m_Matrix[2][i] * MatrixB.m_Matrix[Row][2])
					+ (_MatrixA.m_Matrix[3][i] * MatrixB.m_Matrix[Row][3]);
				Row++;
			}
			Row = 0;
		}
		return Result;
	}

private:
	float m_Matrix[4][4];
};

