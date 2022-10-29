/*
 * =====================================================================================
 *
 *       Filename:  houghtransform.c
 *
 *    Description: Perform hough transform algorithm on the image
 *
 *        Version:  1.0
 *        Created:  10/28/2022 03:38:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Fach'OCR
 *
 * =====================================================================================
 */
#include <math.h>
#include <stdlib.h>

#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/utilis_image.h"

#define DEG2RAD 0.017453293f

void draw_line(Image *image, int w, int h, Line *line);


MyList hough_transform(Image *image, Image *draw_image, int threshold)
{
    int width = image->width;
    int height = image->height;
    Pixel **pixels = image->pixels;

    double hough_h = (sqrt(2.0) * (double)(height > width ? height : width)) / 2;
    double _accu_h = hough_h * 2.0, _accu_w = 180;
    double center_x = width / 2.0, center_y = height / 2.0;

    unsigned int *_accu =
        (unsigned int *)calloc(_accu_h * _accu_w, sizeof(unsigned int));

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (pixels[i][j].r > 250)
            {
                for (int t = 0; t < 180; ++t)
                {
                    double r =
                        (((double)j - center_x) * cos((double)t * DEG2RAD)) +
                        (((double)i - center_y) * sin((double)t * DEG2RAD));
                    _accu[(int)((round(r + hough_h) * 180.0)) + t]++;
                }
            }
        }
    }

    MyList all_lines = {NULL, NULL, 0};

    for (int r = 0; r < _accu_h; r++)
    {
        for (int t = 0; t < _accu_w; t++)
        {
            if ((int)_accu[(int)(r * _accu_w) + t] >= threshold)
            {
                // Is this point a local maxima (9x9)
                int max = _accu[(int)(r * _accu_w) + t];
                for (int ly = -4; ly <= 4; ly++)
                {
                    for (int lx = -4; lx <= 4; lx++)
                    {
                        if ((ly + r >= 0 && ly + r < _accu_h) &&
                                (lx + t >= 0 && lx + t < _accu_w))
                        {
                            if ((int)_accu[(int)((r + ly) * _accu_w) + (t + lx)] > max)
                            {
                                max = _accu[(int)((r + ly) * _accu_w) + (t + lx)];
                                ly = lx = 5;
                            }
                        }
                    }
                }
                if (max > (int)_accu[(int)(r * _accu_w) + t])
                    continue;

                int x1, y1, x2, y2;
                x1 = y1 = x2 = y2 = 0;

                if (t >= 45 && t <= 135)
                {
                    // y = (r - x cos(t)) / sin(t)
                    x1 = 0;
                    y1 = ((double)(r - (_accu_h / 2)) -
                            ((x1 - (width / 2.0)) * cos(t * DEG2RAD))) /
                        sin(t * DEG2RAD) + (height / 2.0);
                    x2 = width - 0;
                    y2 = ((double)(r - (_accu_h / 2)) -
                            ((x2 - (width / 2.0)) * cos(t * DEG2RAD))) /
                        sin(t * DEG2RAD) + (height / 2.0);
                }
                else
                {
                    // x = (r - y sin(t)) / cos(t);
                    y1 = 0;
                    x1 = ((double)(r - (_accu_h / 2)) -
                            ((y1 - (height / 2.0)) * sin(t * DEG2RAD))) /
                        cos(t * DEG2RAD) + (width / 2.0);
                    y2 = height - 0;
                    x2 = ((double)(r - (_accu_h / 2)) -
                            ((y2 - (height / 2.0)) * sin(t * DEG2RAD))) /
                        cos(t * DEG2RAD) + (width / 2.0);
                }
                // append to list line
                Line line;
                line.xStart = x1;
                line.xEnd = x2;
                line.yStart = y1;
                line.yEnd = y2;

                draw_line(draw_image, width, height, &line);

                void *p = Line_tovptr(line);
                append(&all_lines, p);
            }
        }
    }

    return all_lines;
}


void draw_line(Image *image, int w, int h, Line *line)
{
    Pixel color = { .r = 40, .g = 40, .b = 200 };

    int x0 = line->xStart;
    int y0 = line->yStart;

    int x1 = line->xEnd;
    int y1 = line->yEnd;

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (1)
    {
        if (0 <= x0 && x0 < w && 0 <= y0 && y0 < h)
        {
            image->pixels[y0][x0].r = color.r;
            image->pixels[y0][x0].g = color.g;
            image->pixels[y0][x0].b = color.b;
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

