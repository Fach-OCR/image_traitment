#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H


MyList hough_transform(Image *image, int threshold);


void draw_line(Image *image, int w, int h, Line *line);
#endif
