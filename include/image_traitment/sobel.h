#ifndef SOBEL_H
#define SOBEL_H

void sobel(Image *image, int Gx, int Gy, int i, int j);

void non_max_suppresion(Image *sobel_image, Image *image);

void double_threshold(Image *image, unsigned int lowthresh,
                      unsigned int hightresh);

void hysteris(Image *image);

void edges(Image *image);

#endif
