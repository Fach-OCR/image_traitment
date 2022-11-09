/*
 * =====================================================================================
 *
 *       Filename:  utilis_image.c
 *
 *    Description: This file will contain all the function to export and import
 *    an image from an SDL_Surface.
 *
 *        Version:  1.0
 *        Created:  10/10/2022 08:05:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Fach'OCR
 *
 * =====================================================================================
 */
#include "../../include/image_traitment/utilis_image.h"

void draw_dot(Image *image, Dot *dot, int size)
{
    int width = image->width;
    int height = image->height;
    int x = dot->X;
    int y = dot->Y;
    for (int i = -size; i < size; ++i)
    {
        for (int j = -size; j < size; ++j)
        {
            if (x + i >= 0 && x + i < height && j + y >= 0 && j + y < width)
            {
                image->pixels[x + i][y + j].r = 255;
            }
        }
    }
}

Image copy_image(Image *image)
{
    Image new_image = { .width = image->width,
                        .height = image->height,
                        .pixels = NULL,
                        .path = NULL };

    new_image.pixels = (Pixel **)calloc(new_image.height, sizeof(Pixel *));
    for (unsigned int x = 0; x < new_image.height; ++x)
    {
        new_image.pixels[x] = (Pixel *)calloc(new_image.width, sizeof(Pixel));
        if (new_image.pixels[x] == NULL)
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image");
    }

    new_image.path = calloc(strlen(image->path) + 1, sizeof(char));
    strcpy(new_image.path, image->path);

    for (unsigned int i = 0; i < image->height; ++i)
    {
        for (unsigned int j = 0; j < image->width; ++j)
        {
            Pixel p = { .r = image->pixels[i][j].r,
                        .g = image->pixels[i][j].g,
                        .b = image->pixels[i][j].b };
            new_image.pixels[i][j] = p;
        }
    }

    return new_image;
}

void set_all_pixel(Image *image, int i, int j, unsigned int val)
{
    image->pixels[i][j].r = val;
    image->pixels[i][j].g = val;
    image->pixels[i][j].b = val;
}

void free_image(Image *image)
{
    free(image->path);
    for (unsigned int x = 0; x < image->height; x++)
        free(image->pixels[x]);

    free(image->pixels);
}

Image resize_image(Image *image, int dimension)
{
    int new_width = 0, new_height = 0;
    if (image->height == image->width)
    {
        new_width = dimension;
        new_height = dimension;
    }
    else if (image->height > image->width)
    {
        new_width = (int)((float)dimension
                          * ((float)image->width / (float)image->height));
        new_height = dimension;
    }
    else
    {
        new_width = dimension;
        new_height = (int)((float)dimension
                           * ((float)image->height / (float)image->width));
    }
    // create a new image;
    Image new_image = {
        .height = new_height, .width = new_width, .pixels = NULL, .path = NULL
    };
    // assign memmory to the pixels
    new_image.pixels = (Pixel **)calloc(new_height, sizeof(Pixel *));
    for (int x = 0; x < new_height; ++x)
    {
        new_image.pixels[x] = (Pixel *)calloc(new_width, sizeof(Pixel));
        if (new_image.pixels[x] == NULL)
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image");
    }
    // copy the path of the previous image
    new_image.path = (char *)calloc(strlen(image->path) + 1, sizeof(char));
    strcpy(new_image.path, image->path);

    int w1 = image->width;
    int h1 = image->height;
    // calculate the ratio between the new size and the old size
    int j_ratio = (int)((w1 << 16) / new_width) + 1;
    int i_ratio = (int)((h1 << 16) / new_height) + 1;

    // calculate the new pixels
    int j2, i2;
    for (int i = 0; i < new_height; i++)
    {
        for (int j = 0; j < new_width; j++)
        {
            j2 = ((j * j_ratio) >> 16);
            i2 = ((i * i_ratio) >> 16);
            Pixel p = { .r = image->pixels[i2][j2].r,
                        .g = image->pixels[i2][j2].g,
                        .b = image->pixels[i2][j2].b };
            new_image.pixels[i][j] = p;
        }
    }

    // return the new image
    return new_image;
}

void save_image(Image *image, char *name)
{
    SDL_Surface *final_surface = create_surface(image);
    char *res = malloc(strlen(name) + strlen(image->path) + 1);
    strcpy(res, name);
    strcat(res, image->path);
    SDL_SaveBMP(final_surface, res);
    SDL_FreeSurface(final_surface);
    free(res);
}
