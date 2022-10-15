/* * =====================================================================================
 *    Filename:  gaussian_filter.c *
 *    Description: Perform gaussian filter on an image
 *
 *        Version:  1.0
 *        Created:  09/28/2022 07:40:29 PM
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


#define PI 3.142857

void surface_to_grayscale(Image *image)
{
    Pixel **pixels = image->pixels;
    unsigned int w = image->width;
    unsigned int h = image->height;

    unsigned int r, g, b;
    for (unsigned int i = 0; i < h; ++i)
    {
        for (unsigned int j = 0; j < w; ++j)
        {
            r = pixels[i][j].r;
            g = pixels[i][j].g;
            b = pixels[i][j].b;
            unsigned int average = 0.3*r + 0.59*g + 0.11*b;
            pixels[i][j].r = average;
            pixels[i][j].g = average;
            pixels[i][j].b = average;
        }
    }
}

void gaussian_blur(Image *image, int radius)
{
     double sigma = radius / 2.0 > 1.0 ? radius / 2.0 : 1.0;
     int kwidht = (2 * round(radius)) + 1;

     double **kernel = calloc(kwidht, sizeof(double *));
     for (int i = 0; i < kwidht; ++i)
         kernel[i] = calloc(kwidht, sizeof(double));
     double sum = 0.0;

     for (double x = -radius; x < radius; ++x)
     {
         for (double y = -radius; y < radius; ++y)
         {
             double exponum = -(x * x + y * y);
             double expodeno = 2 * sigma * sigma;
             double expression = exp(exponum / expodeno);
             double kvalue = expression / (2 * PI * sigma * sigma);

             kernel[(int)x + radius][(int)y + radius] = kvalue;
             sum += kvalue;
         }
     }

     for (int x = 0; x < kwidht; ++x)
         for (int y = 0; y < kwidht; ++y)
             kernel[x][y] /= sum;

    Image tmp_image = copy_image(image);
    for (unsigned int x = radius; x < image->height - radius; ++x)
    {
        for (unsigned int y = radius; y < image->width - radius; ++y)
        {
            double r = 0.0;
            double g = 0.0;
            double b = 0.0;

            for (int _x = -radius; _x < radius; ++_x)
            {
                for (int _y = -radius; _y < radius; ++_y)
                {
                    double kvalue = kernel[_x + radius][_y + radius];

                    r += tmp_image.pixels[x - _x][y - _y].r * kvalue;
                    g += tmp_image.pixels[x - _x][y - _y].g * kvalue;
                    b += tmp_image.pixels[x - _x][y - _y].b * kvalue;
                }
            }
            image->pixels[x][y].r = r;
            image->pixels[x][y].g = g;
            image->pixels[x][y].b = b;
        }
    }

    for (int i = 0; i < kwidht; ++i)
        free(kernel[i]);
    free(kernel);

    freeImage(&tmp_image);
}



