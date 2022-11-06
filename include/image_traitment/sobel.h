#ifndef SOBEL_H
#define SOBEL_H

void sobel(Image *image, int Gx, int Gy, int i, int j);

void hysteris(Image *image);

void edges(Image *image);

#endif
