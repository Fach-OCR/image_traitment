#ifndef CANNY_H
#define CANNY_H

#include "utilis_image.h"
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#define HIGH_RATIO 0.11
#define LOW_RATIO 0.05

void canny_edge_detection(Image *image);

#endif
