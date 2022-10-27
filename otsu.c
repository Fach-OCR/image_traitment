/*
 * =====================================================================================
 *
 *       Filename:  otsu.c
 *
 *    Description: Find the otsu threshold for the image
 *
 *        Version:  1.0
 *        Created:  10/28/2022 12:02:18 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Fach'OCR
 *
 * =====================================================================================
 */
#include "utilis_image.h"

int hist[255];

int otsu(Image *image)
{
    int width = image->width;
    int height = image->height;
    Pixel **pixels = image->pixels;
    unsigned int threshold = 0;
    int var_max = 0, sum = 0, sum_b = 0, q1 = 0, q2 = 0, mu1 = 0, mu2 = 0;
    int max_intensity = 255;

    for (unsigned int k = 0; k <= 255; ++k)
    {
        int count = 0;
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                if (pixels[i][j].r == k)
                {
                    count++;
                    hist[k] = count;
                }
            }
        }
    }

    for (int i = 0; i < max_intensity; ++i)
        sum += i * hist[i];

    for (int i = 0; i < max_intensity; ++i)
    {
        q1 += hist[i];
        if (q1 == 0)
            continue;
        q2 = width * height - q1;
        if (q2 == 0)
            continue;

        sum_b += i * hist[i];
        mu1 = sum_b / q1;
        mu2 = (sum - sum_b) / q2;

        int sigma = q1 * q2 * ((mu1 - mu2) * (mu1 - mu2));
        if (sigma > var_max)
        {
            threshold = i;
            var_max = sigma;
        }
    }

    return threshold;
}

void apply_threshold(Image *image, unsigned int threshold)
{
    Pixel **pixels = image->pixels;
    for (unsigned int i = 0; i < image->height; ++i)
    {
        for (unsigned int j = 0; j < image->width; ++j)
        {
            set_all_pixel(image, i, j, pixels[i][j].r > threshold ? 0 : 255);
        }
    }
}












