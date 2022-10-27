#ifndef OTSU_H
#define OTSU_H

#include "utilis_image.h"

int otsu(Image *image);

void apply_threshold(Image *image, int threshold);

#endif
