/* * =====================================================================================
 *    Filename:  sobel.c
 *    Description: Perform sobel function on the image
 *
 *        Version:  1.0
 *        Created:  10/15/2022 14:40:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo.Wattebled
 *   Organization: Fach'OCR
 *
 * =====================================================================================
 */
#include "utilis_image.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int sobel(int Gx, int Gy)
{
    int n = sqrt(Gx * Gx + Gy * Gy);
    if (n > 255)
        n = 255;

    return n;

}

void edges(Image *image)
{
    // define kernals
    int Gx[3][3];
    Gx[0][0] = -1;
    Gx[0][1] = 0;
    Gx[0][2] = 1;
    Gx[1][0] = -2;
    Gx[1][1] = 0;
    Gx[1][2] = 2;
    Gx[2][0] = -1;
    Gx[2][1] = 0;
    Gx[2][2] = 1;

    int Gy[3][3];
    Gy[0][0] = -1;
    Gy[0][1] = -2;
    Gy[0][2] = -1;
    Gy[1][0] = 0;
    Gy[1][1] = 0;
    Gy[1][2] = 0;
    Gy[2][0] = 1;
    Gy[2][1] = 2;
    Gy[2][2] = 1;

    // define variables
    int Gx_red, Gx_green, Gx_blue;
    int Gy_red, Gy_green, Gy_blue;

    // define temporary array
    Image edge_image = copy_image(image);

    int height = image->height;
    int width = image->width;
    // loop through columns
    for (int i = 0; i < height; ++i)
    {
        // loop through rows
        for (int j = 0; j < width; ++j)
        {
            // set/reset sobel values for each colour channel
            Gx_red = Gx_green = Gx_blue = 0;
            Gy_red = Gy_green = Gy_blue = 0;

            // 3x3 kernal around [i][j]
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    // Multiply each channel by corresponding value in convolutional array
                    if (i + x < height && i + x > 0 && j + y < width && j + y > 0)
                    {
                        int kx = x + 1;
                        int sy = y + 1;

                        // Gx kernal
                        Gx_red += (image->pixels[i + x][j + y].r * Gx[kx][sy]);
                        Gx_green += (image->pixels[i + x][j + y].g * Gx[kx][sy]);
                        Gx_blue += (image->pixels[i + x][j + y].b * Gx[kx][sy]);

                        // Gy kernal
                        Gy_red += (image->pixels[i + x][j + y].r * Gy[kx][sy]);
                        Gy_green += (image->pixels[i + x][j + y].g * Gy[kx][sy]);
                        Gy_blue += (image->pixels[i + x][j + y].b * Gy[kx][sy]);
                    }
                }
            }
            // Perform sobel operatation and assign each colour channel value to new array
            edge_image.pixels[i][j].r = sobel(Gx_red, Gy_red);
            edge_image.pixels[i][j].g = sobel(Gx_green, Gy_green);
            edge_image.pixels[i][j].b = sobel(Gx_blue, Gy_blue);
        }
    }
    // assign temp array to origional array for output
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            image->pixels[i][j] = edge_image.pixels[i][j];

    freeImage(&edge_image);
}



