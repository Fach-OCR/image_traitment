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
#include "../../include/image_traitment/canny.h"

int Gx[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };

int Gy[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

void sobel(Image *image, int Gx, int Gy, int i, int j, double **theta, int *max)
{
    int n = sqrt(Gx * Gx + Gy * Gy);
    if (n > *max)
        *max = n;

    theta[i][j] = atan2(Gy, Gx);
    set_all_pixel(image, i, j, n);
}

double **sobel_filter(Image *image)
{
    int height = image->height;
    int width = image->width;

    int Gx_val;
    int Gy_val;
    double **theta = calloc(height, sizeof(*theta));
    for (int i = 0; i < height; ++i)
        theta[i] = calloc(width, sizeof(theta));

    int max = 0;
    Image sobel_image = copy_image(image);
    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < width - 1; ++j)
        {
            Gx_val = Gy_val = 0;
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    if (i + x < height && i + x > 0 && j + y < width
                        && j + y > 0)
                    {
                        int kx = x + 1;
                        int sy = y + 1;

                        Gx_val += (image->pixels[i + x][j + y].r * Gx[kx][sy]);
                        Gy_val += (image->pixels[i + x][j + y].r * Gy[kx][sy]);
                    }
                }
            }
            sobel(&sobel_image, Gx_val, Gy_val, i, j, theta, &max);
        }
    }
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            set_all_pixel(image, i, j,
                          (sobel_image.pixels[i][j].r * 255) / max);

    free_image(&sobel_image);
    return theta;
}

void hysteris(Image *image)
{
    Image tmp_image = copy_image(image);
    Pixel **pixels = tmp_image.pixels;
    int height = image->height;
    int width = image->width;

    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < width - 1; ++j)
        {
            if (pixels[i][j].r == 255)
                continue;
            else if (pixels[i][j].r == 100)
            {
                if (pixels[i][j + 1].r == 255 || pixels[i][j - 1].r == 255
                    || pixels[i + 1][j].r == 255 || pixels[i - 1][j].r == 255
                    || pixels[i + 1][j + 1].r == 255
                    || pixels[i - 1][j - 1].r == 255
                    || pixels[i + 1][j - 1].r == 255
                    || pixels[i - 1][j + 1].r == 255)
                    set_all_pixel(image, i, j, 255);
                else
                    set_all_pixel(image, i, j, 0);
            }
            else
                set_all_pixel(image, i, j, 0);
        }
    }

    free_image(&tmp_image);
}

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

            // angle ~= 0
            if ((0 <= curr_angle && curr_angle < 22.5)
                || (157.5 <= curr_angle && curr_angle <= 180))
            {
                q = pixels[i][j + 1].r;
                r = pixels[i][j - 1].r;
            }

            // angle ~= 45
            else if (22.5 <= curr_angle && curr_angle < 67.5)
            {
                q = pixels[i + 1][j - 1].r;
                r = pixels[i - 1][j + 1].r;
            }

            // angle ~= 90
            else if (67.5 <= curr_angle && curr_angle < 112.5)
            {
                q = pixels[i + 1][j].r;
                r = pixels[i - 1][j].r;
            }

            // angle ~= 135
            else if (112.5 <= curr_angle && curr_angle < 157.5)
            {
                q = pixels[i - 1][j - 1].r;
                r = pixels[i + 1][j + 1].r;
            }

            if (pixels[i][j].r >= q && pixels[i][j].r >= r)
                set_all_pixel(image, i, j, pixels[i][j].r);
            else
                set_all_pixel(image, i, j, 0);
        }
    }
    free_image(&cimage);
    for (int i = 0; i < height; ++i)
        free(angle[i]);
    free(angle);
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

    double high_tresh = max * HIGH_RATIO;
    double low_tresh = high_tresh * LOW_RATIO;
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            if (pixels[i][j].r > high_tresh)
                set_all_pixel(image, i, j, 255);
            else if (pixels[i][j].r > low_tresh)
                set_all_pixel(image, i, j, 100);
            else
                set_all_pixel(image, i, j, 0);
        }
    }
}

void canny_edge_detection(Image *image)
{
    double **theta = sobel_filter(image);
    save_image(image, "sobel_");

    non_max_suppression(image, theta);
    save_image(image, "non_max_");

    double_threshold(image);
    save_image(image, "double_tresh_");

    hysteris(image);
    save_image(image, "hysteris_");

    for (unsigned int i = 0; i < image->height; ++i)
        free(theta[i]);
    free(theta);
}
