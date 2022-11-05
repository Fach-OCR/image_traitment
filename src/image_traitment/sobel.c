/* *
 * =====================================================================================
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
#include "../../include/image_traitment/utilis_image.h"
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

int Gx[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };

int Gy[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

void sobel(Image *image, int Gx, int Gy, int i, int j)
{
    int n = sqrt(Gx * Gx + Gy * Gy);
    if (n > 255)
        n = 255;

    set_all_pixel(image, i, j, n);
}

void non_max_suppresion(Image *sobel_image, Image *image)
{
    int height = image->height;
    int width = image->width;

    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < width - 1; ++j)
        {
            int Gx_val = 0;
            int Gy_val = 0;
            // 3x3 kernal around [i][j]
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

            double theta = atan2(Gx_val, Gy_val) * (360.0 / (2.0 * M_PI));

            if ((theta <= 22.5 && theta >= -22.5) || (theta <= -157.5)
                || (theta >= 157.5))
            {
                if (sobel_image->pixels[i][j - 1].r
                        >= sobel_image->pixels[i][j].r
                    || (sobel_image->pixels[i][j + 1].r
                        >= sobel_image->pixels[i][j].r))
                    set_all_pixel(image, i, j, 0);
            }
            else if ((theta > 22.5 && theta <= 67.5)
                     || (theta > -157.5 && theta <= -112.5))
            {
                if (sobel_image->pixels[i - 1][j].r
                        >= sobel_image->pixels[i][j].r
                    || (sobel_image->pixels[i + 1][j].r
                        >= sobel_image->pixels[i][j].r))
                    set_all_pixel(image, i, j, 0);
            }
            else if ((theta > 67.5 && theta <= 112.5)
                     || (theta >= -112.5 && theta < -67.5))
            {
                if (sobel_image->pixels[i + 1][j - 1].r
                        >= sobel_image->pixels[i][j].r
                    || (sobel_image->pixels[i - 1][j + 1].r
                        >= sobel_image->pixels[i][j].r))
                    set_all_pixel(image, i, j, 0);
            }
            else if ((theta >= -67.5 && theta < -22.5)
                     || (theta > 112.5 && theta < 157.5))
            {
                if (sobel_image->pixels[i - 1][j - 1].r
                        >= sobel_image->pixels[i][j].r
                    || (sobel_image->pixels[i + 1][j + 1].r
                        >= sobel_image->pixels[i][j].r))
                    set_all_pixel(image, i, j, 0);
            }
            else
                return;
        }
    }
}


void double_threshold(Image *image, unsigned int lowthresh,
                      unsigned int hightresh)
{
    Pixel **pixels = image->pixels;
    int height = image->height;
    int width = image->width;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (pixels[i][j].r > hightresh)
                set_all_pixel(image, i, j, 255);
            else if (pixels[i][j].r > lowthresh)
                set_all_pixel(image, i, j, 100);
            else
                set_all_pixel(image, i, j, 0);
        }
    }
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

    freeImage(&tmp_image);
}

void edges(Image *image)
{
    int height = image->height;
    int width = image->width;

    int Gx_val;
    int Gy_val;

    // define temporary image
    Image sobel_image = copy_image(image);
    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < width - 1; ++j)
        {
            Gx_val = Gy_val = 0;
            // 3x3 kernal around [i][j]
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    // Multiply each channel by corresponding value in
                    // convolutional array
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
            // Perform sobel operatation
            sobel(&sobel_image, Gx_val, Gy_val, i, j);
        }
    }

    // Perform non_max_suppresion operatation
    non_max_suppresion(&sobel_image, image);

    freeImage(&sobel_image);
}
