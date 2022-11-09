#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include <math.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "utilis_image.h"

#define DEG2RAD 0.017453293f

MyList hough_transform(Image *image, int threshold);

void draw_line(Image *image, Line *line);

#endif
