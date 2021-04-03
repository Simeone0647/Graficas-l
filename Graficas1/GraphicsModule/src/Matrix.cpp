#include "Matrix.h"

float* Matrix::MatrixMultiplication(const float* _MatrixA, const float* _MatrixB)
{
    float Product[16];
    int Counter = 0;
    int NumberOfRow = 0;
    //Product[0] = (_MatrixA[0] * _MatrixB[0]) + (_MatrixA[1] * _MatrixB[4]) + (_MatrixA[2] * _MatrixB[8]) + (_MatrixA[3] * _MatrixB[12]);
    //Product[1] = (_MatrixA[0] * _MatrixB[1]) + (_MatrixA[1] * _MatrixB[5]) + (_MatrixA[2] * _MatrixB[9]) + (_MatrixA[3] * _MatrixB[13]);
    //Product[2] = (_MatrixA[0] * _MatrixB[2]) + (_MatrixA[1] * _MatrixB[6]) + (_MatrixA[2] * _MatrixB[10]) + (_MatrixA[3] * _MatrixB[14]);
    //Product[3] = (_MatrixA[0] * _MatrixB[3]) + (_MatrixA[1] * _MatrixB[7]) + (_MatrixA[2] * _MatrixB[11]) + (_MatrixA[3] * _MatrixB[15]);
    //
    //Product[4] = (_MatrixA[4] * _MatrixB[0]) + (_MatrixA[5] * _MatrixB[4]) + (_MatrixA[6] * _MatrixB[8]) + (_MatrixA[7] * _MatrixB[12]);
    //Product[5] = (_MatrixA[4] * _MatrixB[1]) + (_MatrixA[5] * _MatrixB[4]) + (_MatrixA[6] * _MatrixB[8]) + (_MatrixA[7] * _MatrixB[12]);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Product[Counter] = (_MatrixA[NumberOfRow] * _MatrixB[j])
                               + (_MatrixA[NumberOfRow + 1] * _MatrixB[j + 4])
                               + (_MatrixA[NumberOfRow + 2] * _MatrixB[j + 8])
                               + (_MatrixA[NumberOfRow + 3] * _MatrixB[j + 12]);
            Counter++;
        }
        NumberOfRow += 4;
    }
    return &Product[0];
}
