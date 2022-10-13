/* * =====================================================================================
 *    Filename:  gaussian_filter.c *
 *    Description: Perform gaussian filter on an image
 *
 *        Version:  1.0
 *        Created:  09/28/2022 07:40:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo.Wattebled
 *   Organization: Fach'OCR
 *
 * =====================================================================================
 */
#include "utilis_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>


void surface_to_grayscale(Image *image)
{
    Pixel **pixels = image->pixels;
    unsigned int w = image->width;
    unsigned int h = image->height;

    unsigned int r, g, b;
    for (unsigned int i = 0; i < h; ++i)
    {
        for (unsigned int j = 0; j < w; ++j)
        {
            r = pixels[i][j].r;
            g = pixels[i][j].g;
            b = pixels[i][j].b;
            unsigned int average = 0.3*r + 0.59*g + 0.11*b;
            pixels[i][j].r = average;
            pixels[i][j].g = average;
            pixels[i][j].b = average;
        }
    }
}


int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Impot the surface from the image in argument
    SDL_Surface* surface = IMG_Load(argv[1]);

    // Create an Image struct from the input surface
    Image image = create_image(surface, surface->w, surface->h);

    // Free the surface
    SDL_FreeSurface(surface);

    image.path = (char *)calloc(strlen(argv[1]) + 4, sizeof(char));
    image.path[0] = 'r';
    image.path[1] = 'e';
    image.path[2] = 's';
    image.path[3] = '_';
    strcat(image.path, argv[1]);

    // Convert the image into grayscale
    surface_to_grayscale(&image);

    // Save the image
    SDL_Surface* final_surface = create_surface(&image);
    SDL_SaveBMP(final_surface, image.path);

    // Destroy the objects to free memory
    SDL_FreeSurface(final_surface);
    freeImage(&image);

    SDL_Quit();

    return EXIT_SUCCESS;
}
