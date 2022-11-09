#ifndef BLOB_H
#define BLOB_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wctype.h>

#include "linkedlist.h"
#include "struct.h"
#include "utilis_image.h"

void free_blob_list(MyList *list);

Dot *find_corners(Blob *blob, Image *image);

MyList find_blob(Image *image);

#endif
