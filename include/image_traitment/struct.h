#ifndef STRUCT_H
#define STRUCT_H

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "utilis_image.h"

/**
 * @brief
 *
 */
typedef struct Line {
  int xStart;
  int yStart;
  int xEnd;
  int yEnd;
} Line;

/**
 * @brief
 *
 */
typedef struct LineList {
  Line *lines;
  int len;
  double maxTheta;

} LineList;

/**
 * @brief
 *
 */
typedef struct Dot {
  int X;
  int Y;
} Dot;

/**
 * @brief
 *
 */
typedef struct Square {
  Line top;
  Line bottom;
  Line left;
  Line right;
} Square;

/**
 * @brief
 *
 */
typedef struct SquareList {
  Square *squares;
  int len;
} SquareList;

/**
 * @brief
 *
 * @param x
 * @param y
 * @return unsigned int**
 */
unsigned int **initMatrice(unsigned int x, unsigned int y);

/**
 * @brief
 *
 * @param matrice
 * @param height
 */
void freeMatrice(unsigned int **matrice, size_t height);

#endif
