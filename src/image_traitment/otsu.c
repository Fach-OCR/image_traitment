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
#include "../../include/image_traitment/otsu.h"

float compute_zero_order(float *hist, int k)
{
    float res = 0;
    for (int i = 0; i < k; ++i)
        res += hist[i];

    return res;
}

float compute_first_order(float *hist, int k)
{
    float res = 0;
    for (int i = 0; i < k; i++)
        res += i * hist[i];

    return res;
}

float compute_variance_class(float uT, float wk, float uk)
{
    return pow((uT * wk - uk), 2) / (wk * (1 - wk));
}

int otsu(Image *image)
{
    float hist[256];
    int width = image->width;
    int height = image->height;
    Pixel **pixels = image->pixels;
    int max_intensity = 256;

    // init histo with 0
    for (int i = 0; i < 256; ++i)
        hist[i] = 0;

    // assign values to the histo
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            hist[pixels[i][j].r] += 1;

    // normalisation of the histogran
    int N = width * height;
    for (int i = 0; i < max_intensity; ++i)
        hist[i] = hist[i] / N;

    float w[256], u[256], uT;
    for (int i = 0; i < max_intensity; ++i)
    {
        w[i] = compute_zero_order(hist, i);
        u[i] = compute_first_order(hist, i);
    }

    uT = compute_first_order(hist, 256);
    float variance_class_max = -1;
    float threshold = 0;

    for (int i = 0; i < max_intensity; ++i)
    {
        int vk = compute_variance_class(uT, w[i], u[i]);
        if (vk > variance_class_max)
        {
            variance_class_max = vk;
            threshold = i;
        }
    }

    return (int)threshold;
}

void apply_threshold(Image *image, unsigned int threshold)
{
    Pixel **pixels = image->pixels;
    for (unsigned int i = 0; i < image->height; ++i)
        for (unsigned int j = 0; j < image->width; ++j)
            set_all_pixel(image, i, j, pixels[i][j].r > threshold ? 255 : 0);
}
