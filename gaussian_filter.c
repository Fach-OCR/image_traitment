/*
 * =====================================================================================
 *
 *       Filename:  gaussian_filter.c
 *
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

#include <string.h>
#include <err.h>


void surface_to_grayscale(Image *image)
{
    Pixel **pixels = image->pixels;
    const unsigned int w = image->width;
    const unsigned int h = image->height;

<<<<<<< HEAD
    unsigned int r, g, b;
    for (unsigned int i = 0; i < h; ++i)
    {
        for (unsigned int j = 0; j < w; ++j)
        {
            r = pixels[i][j].r;
            g = pixels[i][j].g;
            b = pixels[i][j].b;
            int average = 0.3*r + 0.59*g + 0.11*b;
            r = g = b = average;
        }
    }
=======
    return res_surface;
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat *format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    int average = 0.3*r + 0.59*g + 0.11*b;

    r = g = b = average;

    Uint32 color = SDL_MapRGB(format, r, g, b);
    return color;
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;

    if (SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (int i = 0; i < len; i++)
        pixels[i] = pixel_to_grayscale(pixels[i], format);

    SDL_UnlockSurface(surface);
}

Uint32 pixelmatrix_to_medpixel(Uint32 **mat, SDL_PixelFormat* format, int ratio)
{
    int r = 0, g = 0, b = 0;
    for (int i = 0; i < ratio; ++i)
    {
        for (int j = 0; j < ratio; ++j)
        {
            Uint8 tmp_r = 0, tmp_g = 0, tmp_b = 0;
            SDL_GetRGB(mat[i][j], format, &tmp_r, &tmp_g, &tmp_b);
            r += tmp_r;
            g += tmp_g;
            b += tmp_b;
        }
    }

    int a = ratio * ratio;
    return SDL_MapRGB(format, r / a, g / a, b / a);
}


void resize_image(SDL_Surface* surface)
{
    int h = surface->h;
    int w = surface->w;
    int ratio = round((h > w ? h : w) / 300);

    printf("ratio = %i\n", ratio);

    if (ratio <= 1) //Don't do anything if the image is already the bound
        return;

    SDL_Surface* new_surface = SDL_CreateRGBSurfaceWithFormat(0, w/ratio, h/ratio, 32, SDL_PIXELFORMAT_RGBA32);
    printf("dim new surface w = %i, h = %i\n", new_surface->w, new_surface->h);
    SDL_LockSurface(new_surface);
    SDL_LockSurface(surface);

    Uint32* prev_pixels = surface->pixels;
    Uint32* new_pixels = new_surface->pixels;

    Uint32 **tmp_mat = (Uint32**)malloc(ratio * sizeof(Uint32));
    for (int i = 0; i < ratio; i++)
        tmp_mat[i] = (Uint32*)malloc(ratio * sizeof(Uint32));

    for (int i = 0; i < w - ratio; i += ratio)
    {
        for (int j = 0; j < h - ratio; j += ratio)
        {
            for (int _i = 0; _i < ratio; ++_i)
            {
                for (int _j = 0; _j < ratio; ++_j)
                {
                    printf("Here i = %i   j = %i   _i = %i   _j = %i   \n", i, j, _i, _j);
                    tmp_mat[_i][_j] = prev_pixels[(i + _i) * h + (j + _j)];
                }
            }
            printf("Here new i = %i   new j = %i \n", i/ratio, j/ratio);
            new_pixels[(int)(i / ratio) * new_surface->w + (int)(j / ratio)] = pixelmatrix_to_medpixel(tmp_mat, new_surface->format, ratio);
        }
    }

    SDL_UnlockSurface(new_surface);
    SDL_UnlockSurface(surface);

    *surface = *new_surface;

    SDL_FreeSurface(surface);
//for (int i = 0; i < ratio; ++i)
//        free(tmp_mat[i]);
//    free(tmp_mat);
>>>>>>> 72574103ec6aa8c15a1b0f65f637a75d7ba317b3
}


int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Impot the surface from the image in argument
    SDL_Surface* surface = IMG_Load(argv[1]);
    Image image = create_image(surface, surface->w, surface->h);
    image.path = argv[1];

    SDL_FreeSurface(surface);


    // Convert the image into grayscale
    surface_to_grayscale(&image);


    //change the name of the file where the image will be saved
    char dest[40] = "res_";
    strcat(dest, argv[1]);

    //save the image
    SDL_Surface* final_surface = create_surface(&image);
    SDL_SaveBMP(final_surface, dest);

    // Destroy the objects to free memory
    SDL_FreeSurface(final_surface);
    freeImage(&image);

    SDL_Quit();

    return EXIT_SUCCESS;
}
