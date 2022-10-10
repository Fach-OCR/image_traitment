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
#include <SDL2/SDL2.h>
#include <SDL2/SDL2_image.h>
#include <err.h>

typedef struct Pixel
{
    unsigned int r, g, b;
} Pixel;


typedef struct Image
{
    unsigned int width;
    unsigned int height;
    double averageColor;
    Pixel **pixels;
    char *path;
} Image;


Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

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
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

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


Image create_image(SDL_Surface *surface, int width, int height)
{
    Image image;

    image.width = width;
    image.height = height;

    // Initializes the matrix of the image
    image.pixels = calloc(width, sizeof(Pixel *));
    for (int x = 0; x < width; x++)
    {
        image.pixels[x] = calloc(height, sizeof(Pixel));
        if (image.pixels[x] == NULL)
        {
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image");
        }
    }

    SDL_Color rgb;
    Uint32 pixel;
    double averageColor = 0;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            // Get pixel from surface
            pixel = get_pixel(surface, x, y);

            // Get RGB values from pixel
            SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);

            image.pixels[x][y].r = rgb.r;
            image.pixels[x][y].g = rgb.g;
            image.pixels[x][y].b = rgb.b;
            averageColor += ((rgb.r + rgb.g + rgb.b) / 3);

        }
    }
    averageColor /= (width * height);
    image.averageColor = averageColor;

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
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
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


void freeImage(Image *image)
{
    for (unsigned int x = 0; x < image->width; x++)
        free(image->pixels[x]);

    free(image->pixels);
}















