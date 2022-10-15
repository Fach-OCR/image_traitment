#include "utilis_image.h"
#include "sobel.h"
#include "gaussian_filter.h"

#include <err.h>

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

    image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
    image.path[0] = 'r';
    image.path[1] = 'e';
    image.path[2] = 's';
    image.path[3] = '_';
    strcat(image.path, argv[1]);

    surface_to_grayscale(&image);
    gaussian_blur(&image, 3);
    edges(&image);

    // Save the image
    SDL_Surface* final_surface = create_surface(&image);
    SDL_SaveBMP(final_surface, image.path);

    // Destroy the objects to free memory
    SDL_FreeSurface(final_surface);
    freeImage(&image);

    SDL_Quit();

    return EXIT_SUCCESS;
}
