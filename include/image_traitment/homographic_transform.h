#ifndef HOMOGRAPHIC_TRANSFORM_H
#define HOMOGRAPHIC_TRANSFORM_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wctype.h>

#include "linkedlist.h"
#include "struct.h"
#include "utilis_image.h"
#include "utilis_matrix.h"

#define FILL(mat, x, y) mat[x * 8 + y]

Image HomographicTransform(Image *image, Dot *TL_dot, Dot *TR_dot, Dot *BL_dot,
                           Dot *BR_dot, int size);

#endif
