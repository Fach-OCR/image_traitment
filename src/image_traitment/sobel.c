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
#include <SDL2/SDL_video.h>
#include <math.h>
#include "../../include/image_traitment/utilis_image.h"

int Gx[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };

int Gy[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

void sobel(Image *image, int Gx, int Gy, int i, int j, double **theta)
{
    int n = sqrt(Gx * Gx + Gy * Gy);
    if (n > 255)
        n = 255;

    theta[i][j] = atan2(Gx, Gy);
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
            sobel(&sobel_image, Gx_val, Gy_val, i, j, theta);
        }
    }
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            set_all_pixel(image, i, j, sobel_image.pixels[i][j].r);

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
