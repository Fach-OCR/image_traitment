#include "../../include/image_traitment/utilis_image.h"
#include "../../include/image_traitment/otsu.h"
#include "../../include/image_traitment/sobel.h"
#include "../../include/image_traitment/gaussian_filter.h"
#include "../../include/image_traitment/preprocess.h"
#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/houghtransform.h"

#include <err.h>

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Import image
    SDL_Surface* surface = IMG_Load(argv[1]);
    Image image = create_image(surface, surface->w, surface->h);

    // Create the name to save image
    image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
    image.path[0] = 'r';
    image.path[1] = 'e';
    image.path[2] = 's';
    image.path[3] = '_';
    strcat(image.path, argv[1]);

    // Perform canny on the image
    surface_to_grayscale(&image);
    image_contrast(&image, 10);
    invert(&image);
    image_normalize_brightness(&image);
    gaussian_blur(&image, 3);
    apply_threshold(&image, otsu(&image));
    hysteris(&image);
    edges(&image);
    int w = image.width;
    int h = image.height;
    int thresh = w>h?w/4:h/4;
    hough_transform(&image, thresh);


    // Save the image
    SDL_Surface* final_surface = create_surface(&image);
    SDL_SaveBMP(final_surface, image.path);

    // Free image and surface
    SDL_FreeSurface(final_surface);
    freeImage(&image);

    SDL_Quit();

    return EXIT_SUCCESS;
}
