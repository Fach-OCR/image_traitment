#ifndef GRID_DETECTION_H
#define GRID_DETECTION_H

#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "utilis_image.h"

MyList simplify_lines(MyList *all_lines, int gap);

MyList find_intersections(MyList *lines);

MyList find_squares(MyList *all_lines, Image *image);
#endif
