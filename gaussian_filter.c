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
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <err.h>
#include <math.h>
// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* tmp_surface = IMG_Load(path);
    SDL_Surface* res_surface = SDL_ConvertSurfaceFormat(tmp_surface, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp_surface);

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

Uint32 pixelmatrix_to_medpixel(Uint32* mat, SDL_PixelFormat* format, size_t size)
{
    int r = 0, g = 0, b = 0;
    int a = size * size;
    for (int i = 0; i < a; ++i)
    {
        Uint8 tmp_r = 0, tmp_g = 0, tmp_b = 0;
        SDL_GetRGB(mat[i], format, &tmp_r, &tmp_g, &tmp_b);
        r += tmp_r;
        g += tmp_g;
        b += tmp_b;
    }

    return SDL_MapRGB(format, r / a, g / a, b / a);
}


void resize_image(SDL_Surface* surface)
{
    SDL_LockSurface(surface);
    int h = surface->h;
    int w = surface->w;
    Uint32* prev_pixels = surface->pixels;
    int ratio = round((h > w ? h : w) / 300);
    printf("ratio = %i\n", ratio);
    if (ratio <= 1) //Don't do anything if the image is already the bound
        return;

    SDL_Surface* new_surface = SDL_CreateRGBSurfaceWithFormat(0, w/ratio, h/ratio, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_LockSurface(new_surface);
    Uint32* new_pixels = new_surface->pixels;

    Uint32 *tmp_mat = (Uint32 *)malloc(ratio * ratio * sizeof(Uint32));

    for (int i = 0; i < w - ratio; i += ratio)
    {
        for (int j = 0; j < h - ratio; j += ratio)
        {
            for (int _i = 0; _i < ratio; ++_i)
            {
                for (int _j = 0; _j < ratio; ++_j)
                {
                    printf("Here\n");
                    tmp_mat[_i * ratio + _j] = prev_pixels[(_i - j) * h + (_j - j)];
                }
            }

            new_pixels[(i / ratio) * new_surface->w + (j / ratio)] = pixelmatrix_to_medpixel(tmp_mat, new_surface->format, ratio); //todo
        }
    }
    SDL_UnlockSurface(new_surface);
    SDL_UnlockSurface(surface);
    *surface = *new_surface;
    SDL_FreeSurface(surface);
    free(tmp_mat);
}


int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Impot the surface from the image in argument
    SDL_Surface* surface = load_image(argv[1]);

    resize_image(surface);


    // Convert the surface into grayscale
    surface_to_grayscale(surface);


    //change the name of the file where the image will be saved
    char dest[40] = "res_";
    strcat(dest, argv[1]);

    //save the image
    SDL_SaveBMP(surface, dest);

    // Destroy the objects to free memory
    SDL_FreeSurface(surface);
    SDL_Quit();

    return EXIT_SUCCESS;
}
