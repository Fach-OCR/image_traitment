#include <err.h>

#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/otsu.h"
#include "../../include/image_traitment/preprocess.h"
#include "../../include/image_traitment/sobel.h"
#include "../../include/image_traitment/utilis_image.h"
#include "../../include/image_traitment/gaussian_filter.h"
#include "../../include/image_traitment/houghtransform.h"
#include "../../include/image_traitment/grid_detection.h"

int main(int argc, char **argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Import image
    SDL_Surface *surface = IMG_Load(argv[1]);
    Image image = create_image(surface, surface->w, surface->h);

    // Create the name to save image
    image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
    image.path[0] = 'r';
    image.path[1] = 'e';
    image.path[2] = 's';
    image.path[3] = '_';
    strcat(image.path, argv[1]);

    // Preprocess
    surface_to_grayscale(&image);
    image_contrast(&image, 10);
    image_normalize_brightness(&image);
    invert(&image);
    gaussian_blur(&image, 3);

    // Binarisation
    int otsuthresh = otsu(&image);
    apply_threshold(&image, otsuthresh);
    hysteris(&image);
    edges(&image);

    Image draw_image = copy_image(&image);
    int w = image.width;
    int h = image.height;
    int thresh = w > h ? w / 4 : h / 4;

    MyList all_lines = hough_transform(&image, thresh);
    MyList simplified_lines = simplify_lines(&all_lines, 90);

  //  printf("len all %zu\n", all_lines.length);
//    printf("len sim %zu\n", simplified_lines.length);
    for (size_t i = 0; i < simplified_lines.length; ++i)
    {
        Line *l = get_value(&simplified_lines, i);
   //     printf("xstart: %i  ystart: %i  xend: %i, yend %i\n", l->xStart,
    //           l->yStart, l->xEnd, l->yEnd);
        draw_line(&draw_image, l);
    }

    // Save the image
    SDL_Surface *final_surface = create_surface(&draw_image);
    SDL_SaveBMP(final_surface, image.path);

    // Free image and surface
    SDL_FreeSurface(final_surface);
    freeImage(&image);

    SDL_Quit();

    return EXIT_SUCCESS;
}
