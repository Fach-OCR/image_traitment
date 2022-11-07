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
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pixel
{
    unsigned int r, g, b;
} Pixel;

typedef struct Image
{
    unsigned int width;
    unsigned int height;
    Pixel **pixels;
    char *path;
} Image;

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);

Image create_image(SDL_Surface *surface, int width, int height)
{
    Image image = {
        .width = width, .height = height, .pixels = NULL, .path = NULL
    };

    // Initializes the matrix of the image
    image.pixels = (Pixel **)calloc(height, sizeof(Pixel *));
    for (int x = 0; x < height; ++x)
    {
        image.pixels[x] = (Pixel *)calloc(width, sizeof(Pixel));
        if (image.pixels[x] == NULL)
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image");
    }

    SDL_Color rgb;
    Uint32 tmp_pixel;

    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            // Get pixel from surface
            tmp_pixel = get_pixel(surface, x, y);

            // Get RGB values from pixel
            SDL_GetRGB(tmp_pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);

            image.pixels[x][y].r = rgb.r;
            image.pixels[x][y].g = rgb.g;
            image.pixels[x][y].b = rgb.b;
        }
    }

    return image;
}

SDL_Surface *create_surface(Image *image)
{
    const unsigned int width = image->width;
    const unsigned int height = image->height;

    // Create rgb surface from image
    SDL_Surface *surface =
        SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    // For each pixel in the source image
    for (unsigned int x = 0; x < height; ++x)
    {
        for (unsigned int y = 0; y < width; ++y)
        {
            // Get pixel from image
            Pixel _pixel = image->pixels[x][y];

            // Get pixel value for SDL
            Uint32 pixel =
                SDL_MapRGB(surface->format, _pixel.r, _pixel.g, _pixel.b);

            // Put pixel in img
            put_pixel(surface, x, y, pixel);
        }
    }

    return surface;
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

Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + x * surface->pitch + y * bpp;

    switch (bpp)
    {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;
    }
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + x * surface->pitch + y * bpp;

    switch (surface->format->BytesPerPixel)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
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

Image resize_image(Image *image, int new_width, int new_height)
{
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
