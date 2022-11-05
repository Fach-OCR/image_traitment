/*
 * =====================================================================================
 *
 *       Filename:  grid_detection.c
 *
 *    Description:  All the function to detect the grid of the sudoku
 *
 *        Version:  1.0
 *        Created:  10/31/2022 10:44:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Fach'OCR
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/utilis_image.h"

MyList simplify_lines(MyList *all_lines, int gap)
{
    MyList new_lines = { NULL, NULL, 0 };
    for (size_t i = 0; i < all_lines->length; ++i)
    {
        for (size_t j = 1 + i; j < all_lines->length; ++j)
        {
            Line *l1 = get_value(all_lines, i);
            Line *l2 = get_value(all_lines, j);
            int x1d, x2d, y1d, y2d, x1f, x2f, y1f, y2f;
            x1d = l1->xStart;
            x2d = l2->xStart;
            y1d = l1->yStart;
            y2d = l2->yStart;

            x1f = l1->xEnd;
            x2f = l2->xEnd;
            y1f = l1->yEnd;
            y2f = l2->yEnd;

            // Line almost equals
            if (abs(x1d - x2d) <= gap && abs(x1f - x2f) <= gap
                && abs(y1d - y2d) <= gap && abs(y1f - y2f) <= gap)
                append(&new_lines, l1);
        }
    }

    return new_lines;
}

MyList find_intersections(MyList *lines)
{
    MyList res = { NULL, NULL, 0 };
    for (size_t i = 0; i < lines->length; ++i)
    {
        for (size_t j = 1; j < lines->length; ++j)
        {
            Line *l1 = get_value(lines, i);
            Line *l2 = get_value(lines, j);
            int x1, x2, x3, x4, y1, y2, y3, y4;
            x1 = l1->xStart;
            x2 = l1->xEnd;
            y1 = l1->yStart;
            y2 = l1->yEnd;

            x3 = l2->xStart;
            x4 = l2->xEnd;
            y3 = l2->yStart;
            y4 = l2->yEnd;

            int x12 = x1 - x2;
            int y34 = y3 - y4;
            int y12 = y1 - y2;
            int x34 = x3 - x4;

            int c = x12 * y34 - y12 * x34;

            if (c > 0)
            {
                Dot dot;
                int a = x1 * y2 - y1 * x2;
                int b = x3 * y4 - y3 * x4;
                dot.X = (a * x34 - b * x12) / c;
                dot.Y = (a * y34 - b * y12) / c;

                void *p = Dot_tovptr(dot);
                append(&res, p);
            }
        }
    }

    return res;
}
