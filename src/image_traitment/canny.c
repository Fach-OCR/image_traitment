/*
 * =====================================================================================
 *
 *       Filename:  canny.c
 *
 *    Description: Do the canny thing
 *
 *        Version:  1.0
 *        Created:  11/07/2022 11:20:07 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  O12R
 *
 * =====================================================================================
 */
#include <SDL2/SDL_audio.h>
#include <math.h>
#include <stdlib.h>
#include "../../include/image_traitment/utilis_image.h"
#include "../../include/image_traitment/sobel.h"

#ifndef M_PI
#    define M_PI 3.14159265359
#endif

void non_max_suppression(Image *image, double **D)
{
    int height = image->height;
    int width = image->width;
    Image cimage = copy_image(image);
    Pixel **pixels = cimage.pixels;

    double **angle = calloc(height, sizeof(*angle));
    for (int i = 0; i < height; ++i)
        angle[i] = calloc(width, sizeof(angle));

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            angle[i][j] = D[i][j] * 180 / M_PI;
            if (angle[i][j] < 0)
                angle[i][j] += 180;
        }
    }

    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < width - 1; ++j)
        {
            unsigned int q = 255;
            unsigned int r = 255;
            double curr_angle = angle[i][j];

            if ((0 <= curr_angle && curr_angle < 22.5)
                || (157.5 <= curr_angle && curr_angle <= 180))
            {
                q = pixels[i][j + 1].r;
                r = pixels[i][j - 1].r;
            }
            if (22.5 <= curr_angle && curr_angle < 67.5)
            {
                q = pixels[i + 1][j - 1].r;
                r = pixels[i - 1][j + 1].r;
            }

            // angle ~= 90
            if (67.5 <= curr_angle && curr_angle < 112.5)
            {
                q = pixels[i + 1][j].r;
                r = pixels[i - 1][j].r;
            }

            // angle ~= 135
            if (112.5 <= curr_angle && curr_angle < 157.5)
            {
                q = pixels[i - 1][j - 1].r;
                r = pixels[i + 1][j + 1].r;
            }
            if (pixels[i][j].r >= q && pixels[i][j].r >= r)
                set_all_pixel(image, i, j, pixels[i][j].r);
        }
    }
}

void double_threshold(Image *image)
{
    double max = 0;
    int h = image->height;
    int w = image->width;
    Pixel **pixels = image->pixels;

    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            if (pixels[i][j].r > max)
                max = pixels[i][j].r;

    double high_tresh = max * 0.09;
    double low_tresh = high_tresh * 0.05;

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            if (pixels[i][j].r > high_tresh)
                set_all_pixel(image, i, j, 255);
            else if (pixels[i][j].r > low_tresh)
                set_all_pixel(image, i, j, 25);
            else
                set_all_pixel(image, i, j, 0);
        }
    }
}

void canny_edge_detection(Image *image)
{
    double **theta = sobel_filter(image);
    non_max_suppression(image, theta);
    double_threshold(image);
    hysteris(image);

    free(theta);
}
