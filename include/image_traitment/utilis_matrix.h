#ifndef UTILIS_MATRIX_H
#define UTILIS_MATRIX_H

void matMult(double *matA, double *matB, int heightA, int widthA, int widthB,
             double *result);
void matSub(double *matA, double *matB, double *matC, int height, int width);
void matAdd(double *matA, double *matB, int height, int width);
int inverse(double *matrix, double *inverse, int cols);
void matTranspose(double *mat, double *result, int height, int width);
void hadamardProduct(double *matA, double *matB, int height, int width);

#endif
