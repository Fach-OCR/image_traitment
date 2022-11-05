#ifndef PREPROCESS_H
#define PREPROCESS_H

void image_contrast(Image *image, size_t n);

float max_color(Image *image);

void image_normalize_brightness(Image *image);

void invert(Image *image);
#endif
