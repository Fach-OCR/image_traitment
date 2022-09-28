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
#include <string.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>



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
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
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

void resize_image(SDL_Surface* surface)
{
    double w = surface->w;
    double h = surface->h;
    if (w > 800)
        return;

    double ratio = (w >= h ? w / h : h / w);


    double n_w = (h * 800) / w;
}

void change_res_name(char[] dest, char[] arg)
{
    dest[0] = 'r';
    dest[1] = 'e';
    dest[2] = 's';
    dest[3] = '_';
    strcat(dest, arg);
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

    //resize_image(surface);
    // Convert the surface into grayscale
    surface_to_grayscale(surface);

    //change the name of the file where the image will be saved
    char dest[sizeof(argv[1]) + 5];
    change_res_name(dest, argv[1]);

    //save the image
    SDL_SaveBMP(surface, dest);

    // Destroy the objects to free memory
    SDL_FreeSurface(surface);
    SDL_Quit();

    return EXIT_SUCCESS;
}
