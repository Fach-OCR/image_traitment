#ifndef STRUCT_H
#define STRUCT_H

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Line {
  int xStart;
  int yStart;
  int xEnd;
  int yEnd;
} Line;

typedef struct LineList {
  Line *lines;
  int len;
  double maxTheta;

} LineList;

typedef struct Dot {
  int X;
  int Y;
} Dot;

typedef struct Square {
  Line top;
  Line bottom;
  Line left;
  Line right;
} Square;

typedef struct SquareList {
  Square *squares;
  int len;
} SquareList;

typedef struct Blob {
  int length;
  Dot *dots;
} Blob;

#endif
