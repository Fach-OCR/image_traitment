#ifndef GAUSSIAN_FILTER_H
#define GAUSSIAN_FILTER_H

void surface_to_grayscale(Image *image);

double **build_gaussian_kernel(int radius);

void gaussian_blur(Image *image, int radius);

#endif
