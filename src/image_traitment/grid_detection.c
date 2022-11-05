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
            {
                printf("xstart: %i  ystart: %i  xend: %i, yend %i\n",
                       l1->xStart, l1->yStart, l1->xEnd, l1->yEnd);
                printf("xstart: %i  ystart: %i  xend: %i, yend %i\n",
                       l2->xStart, l2->yStart, l2->xEnd, l2->yEnd);
                Line new_line;
                new_line.xStart = (x1d + x2d) / 2;
                new_line.yStart = (y1d + y2d) / 2;
                new_line.xEnd = (x1f + x2f) / 2;
                new_line.yEnd = (y1f + y2f) / 2;
                removeAt(all_lines, i);
                removeAt(all_lines, j);
                i = 0;
                j = 1;

                printf("xstart: %i  ystart: %i  xend: %i, yend %i\n\n\n",
                       new_line.xStart, new_line.yStart, new_line.xEnd,
                       new_line.yEnd);
                void *p = Line_tovptr(new_line);
                append(&new_lines, p);
            }
        }
    }

    return new_lines;
}
