/*
 * =====================================================================================
 *
 *       Filename:  homographic_transform.c
 *
 *    Description:  Perform homographic transform resizing / rotating and
 *    correction the perspective
 *
 *        Version:  1.0
 *        Created:  11/09/2022 08:25:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled, Mathieu Pastre, Xavier de Place
 *   Organization:  0x12R
 *
 * =====================================================================================
 */
#include "../../include/image_traitment/homographic_transform.h"
#define FILL(mat, x, y) mat[x * 8 + y]

// Function : Fill_matrix
// Description : Computes the value for the homographic transphormation matrix
// input : orderedPoints *points the points

// return : **float the matrix
double *Fill_matrix(int corTLx, int corTLy, int corTRx, int corTRy, int corBLx,
                    int corBLy, int corBRx, int corBRy, int size)
{
    double *mat = calloc(8 * 8, sizeof(double));
    // 1st ROW
    FILL(mat, 0, 2) = 1;
    FILL(mat, 1, 5) = 1;
    // 2nd ROW
    FILL(mat, 2, 0) = size;
    FILL(mat, 2, 2) = 1;
    FILL(mat, 2, 6) = -corTRx * size;
    FILL(mat, 3, 3) = size;
    FILL(mat, 3, 5) = 1;
    FILL(mat, 3, 6) = -corTRy * size;
    // 3rd ROW
    FILL(mat, 4, 1) = size;
    FILL(mat, 4, 2) = 1;
    FILL(mat, 4, 7) = -corBLx * size;
    FILL(mat, 5, 4) = size;
    FILL(mat, 5, 5) = 1;
    FILL(mat, 5, 7) = -corBLy * size;
    // 4th ROW
    FILL(mat, 6, 0) = size;
    FILL(mat, 6, 1) = size;
    FILL(mat, 6, 2) = 1;
    FILL(mat, 6, 6) = -corBRx * size;
    FILL(mat, 6, 7) = -corBRy * size;
    FILL(mat, 7, 3) = size;
    FILL(mat, 7, 4) = size;
    FILL(mat, 7, 5) = 1;
    FILL(mat, 7, 6) = -corBRx * size;
    FILL(mat, 7, 7) = -corBRy * size;

    double *res = calloc(8, sizeof(double));
    double b[] = { (double)corTLx, (double)corTLy, (double)corTRx,
                   (double)corTRy, (double)corBLx, (double)corBLy,
                   (double)corBRx, (double)corBRy };

    // Create a Matrix to store the transpose of the matrix mat
    double *mat_transpose = calloc(8 * 8, sizeof(double));
    matTranspose(mat, mat_transpose, 8, 8);
    // Create a Matrix to store the multiplication of the matrix mat_transpose
    // and mat
    double *res_mult = calloc(8 * 8, sizeof(double));
    matMult(mat_transpose, mat, 8, 8, 8, res_mult);
    // Free mat we don't need it anymore
    free(mat);
    double *res_inv = calloc(8 * 8, sizeof(double));
    if (!inverse(res_mult, res_inv, 8))
    {
        errx(EXIT_FAILURE, "Matrix is singular");
    }

    free(res_mult);
    double *res_mult2 = calloc(8 * 8, sizeof(double));
    matMult(res_inv, mat_transpose, 8, 8, 8, res_mult2);
    free(res_inv);
    free(mat_transpose);
    matMult(res_mult2, b, 8, 8, 1, res);
    free(res_mult2);
    return res;
}

// Function : HomographicTransphorm
// Description : Computes the homographic transphormation of an image
// input : SDL_Surface *src the source image
//         orderedPoints points the points
// return : SDL_Surface *the image after the transphormation

// Image HomographicTransform(Image *image, int corTLx, int corTLy, int corTRx,
// int corTRy, int corBLx, int corBLy, int corBRx, int corBRy, int size)
Image HomographicTransform(Image *image, Dot *TL_dot, Dot *TR_dot, Dot *BL_dot,
                           Dot *BR_dot, int size)
{
    // Create a matrix to store the homographic transphormation matrix
    double *mat = Fill_matrix(TL_dot->X, TL_dot->Y, TR_dot->X, TR_dot->Y,
                              BL_dot->X, BL_dot->Y, BR_dot->X, BR_dot->Y, size);

    Image new_image = {
        .height = size, .width = size, .pixels = NULL, .path = NULL
    };

    new_image.pixels = (Pixel **)calloc(size, sizeof(Pixel *));
    for (int x = 0; x < size; ++x)
    {
        new_image.pixels[x] = (Pixel *)calloc(size, sizeof(Pixel));
        if (new_image.pixels[x] == NULL)
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image");
    }

    new_image.path = calloc(strlen(image->path) + 1, sizeof(char));
    strcpy(new_image.path, image->path);

    // Create a matrix to store the transphormation matrix
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            double x = (mat[0] * i + mat[1] * j + mat[2])
                / (mat[6] * i + mat[7] * j + 1);
            double y = (mat[3] * i + mat[4] * j + mat[5])
                / (mat[6] * i + mat[7] * j + 1);

            if (x >= 0 && x < image->width && y >= 0 && y < image->height)
            {
                new_image.pixels[i][j].r = image->pixels[(int)x][(int)y].r;
                new_image.pixels[i][j].g = image->pixels[(int)x][(int)y].g;
                new_image.pixels[i][j].b = image->pixels[(int)x][(int)y].b;
            }
        }
    }

    free(mat);
    return new_image;
}
