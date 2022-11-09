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

#include "../../include/image_traitment/grid_detection.h"

int line_equal(Line *l1, Line *l2)
{
    return l1->xStart == l2->xStart && l1->xEnd == l2->xEnd
        && l1->yStart == l2->yStart && l1->yEnd == l2->yEnd;
}

MyList simplify_lines(MyList *all_lines, int gap)
{
    MyList new_lines = { NULL, NULL, 0 };
    for (size_t i = 0; i < all_lines->length; ++i)
    {
        for (size_t j = 1 + i; j < all_lines->length; ++j)
        {
            if (i == j)
                continue;

            Line *l1 = get_value(all_lines, i);
            Line *l2 = get_value(all_lines, j);
            int x1, x2, x3, x4, y1, y2, y3, y4;
            x1 = l1->xStart;
            x2 = l1->xEnd;
            y1 = l1->yStart;
            y2 = l1->yEnd;

            x3 = l2->xStart;
            x4 = l2->xEnd;
            y3 = l2->yStart;
            y4 = l2->yEnd;

            // Line almost equals
            if (abs(x1 - x3) <= gap && abs(x2 - x4) <= gap
                && abs(y1 - y3) <= gap && abs(y2 - y4) <= gap)
            {
                Line l = { .xStart = (x1 + x3) / 2,
                           .yStart = (y1 + y3) / 2,
                           .xEnd = (x2 + x4) / 2,
                           .yEnd = (y2 + y4) / 2 };

                int test_in = 0;
                for (size_t k = 0; k < new_lines.length; ++k)
                {
                    test_in = line_equal(get_value(&new_lines, k), &l);
                    if (test_in == 1)
                        break;
                }
                if (test_in == 0)
                {
                    append(&new_lines, Line_tovptr(l));
                }
            }
        }
    }

    return new_lines;
}

Dot find_inter_line1(Line *line1, Line *line2, int width, int height)
{
    Dot dot;
    if ((line1->xEnd - line1->xStart) != 0
        && (line2->xEnd - line2->xStart) != 0)
    {
        double directCoeff1 = ((double)line1->yEnd - (double)line1->yStart)
            / ((double)line1->xEnd - (double)line1->xStart);
        double directCoeff2 = ((double)line2->yEnd - (double)line2->yStart)
            / ((double)line2->xEnd - (double)line2->xStart);

        double ordOrigin1 = (double)line1->yStart
            - (double)directCoeff1 * (double)line1->xStart;
        double ordOrigin2 = (double)line2->yStart
            - (double)directCoeff2 * (double)line2->xStart;

        if (((int)directCoeff1 - (int)directCoeff2) != 0)
        {
            int x = (ordOrigin1 - ordOrigin2) / (directCoeff2 - directCoeff1);
            int y = directCoeff1 * (ordOrigin2 - ordOrigin1)
                    / (directCoeff1 - directCoeff2)
                + ordOrigin1;

            if (x >= 0 && x < width && y >= 0 && y < height)
            {
                dot.X = x;
                dot.Y = y;
                return dot;
            }
        }
    }
    dot.X = -1;
    dot.Y = -1;
    return dot;
}

Dot find_inter_line(Line *l1, Line *l2, int width, int height)
{
    Dot dot;
    dot.X = -1;
    dot.Y = -1;

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
        int a = x1 * y2 - y1 * x2;
        int b = x3 * y4 - y3 * x4;
        int X = (a * x34 - b * x12) / c;
        int Y = (a * y34 - b * y12) / c;
        if (X >= 0 && X <= height && Y >= 0 && Y <= width)
        {
            dot.X = X;
            dot.Y = Y;
        }
    }

    return dot;
}

MyList find_squares(MyList *all_lines, Image *image)
{
    MyList square_list = { NULL, NULL, 0 };
    int w = image->width;
    int h = image->height;
    int len = all_lines->length;
    int i = 0;
    for (; i < len; ++i)
    {
        Line *l1 = get_value(all_lines, i);
        int j = 0;
        for (; j < len; ++j)
        {
            if (j == i)
                continue;
            Line *l2 = get_value(all_lines, j);
            Dot dot1 = find_inter_line(l1, l2, w, h);

            if (dot1.X == -1)
                continue;

            int k = 0;
            for (; k < len; ++k)
            {
                if (k == j)
                    continue;
                //                printf("ta grosse race1\n");
                Line *l3 = get_value(all_lines, k);
                Dot dot2 = find_inter_line(l2, l3, w, h);

                if (dot2.X == -1)
                    continue;
                int l = 0;
                for (; l < len; ++l)
                {
                    printf("ta grosse race2\n");
                    if (k == l)
                        continue;
                    Line *l4 = get_value(all_lines, l);
                    Dot dot3 = find_inter_line(l3, l4, w, h);

                    if (dot3.X == -1)
                        continue;

                    Dot dot4 = find_inter_line(l4, l1, w, h);

                    if (dot4.X == -1)
                        continue;
                    // Found 4 lines that form a square

                    Square square;
                    Line firstLine = { .xStart = dot1.X,
                                       .yStart = dot1.Y,
                                       .xEnd = dot2.X,
                                       .yEnd = dot2.Y };
                    square.top = firstLine;

                    Line secondLine = { .xStart = dot2.X,
                                        .yStart = dot2.Y,
                                        .xEnd = dot3.X,
                                        .yEnd = dot3.Y };
                    square.right = secondLine;

                    Line thirdLine = { .xStart = dot3.X,
                                       .yStart = dot3.Y,
                                       .xEnd = dot4.X,
                                       .yEnd = dot4.Y };
                    square.bottom = thirdLine;

                    Line fourthLine = { .xStart = dot4.X,
                                        .yStart = dot4.Y,
                                        .xEnd = dot1.X,
                                        .yEnd = dot1.Y };
                    square.left = fourthLine;

                    printf("we did it");
                    append(&square_list, Square_tovptr(square));
                }
            }
        }
    }

    return square_list;
}
