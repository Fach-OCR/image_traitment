#include <SDL2/SDL_surface.h>
#include <err.h>

#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/otsu.h"
#include "../../include/image_traitment/preprocess.h"
#include "../../include/image_traitment/sobel.h"
#include "../../include/image_traitment/utilis_image.h"
#include "../../include/image_traitment/gaussian_filter.h"
#include "../../include/image_traitment/grid_detection.h"
#include "../../include/image_traitment/houghtransform.h"

void draw_dot2(Image *image, Dot *dot, int size)
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
                image->pixels[x + i][y + j].b = 255;
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Import image
    SDL_Surface *surface = IMG_Load(argv[1]);
    SDL_Surface *surface2 =
        resize_surface(surface, surface->w / 2, surface->h / 2);
    Image image = create_image(surface2, surface2->w, surface2->h);
    SDL_FreeSurface(surface);

    SDL_Surface *final_surface = create_surface(&image);
    SDL_SaveBMP(final_surface, "lol.jpeg");
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

    // Compute Hough transform
    int w = image.width;
    int h = image.height;
    int thresh = w > h ? w / 4 : h / 4;

    MyList all_lines = hough_transform(&image, thresh);
    MyList simplified_lines = simplify_lines(&all_lines, 50);
    MyList dots = find_intersections(&simplified_lines);

    Image draw_image = copy_image(&image);

    for (size_t i = 0; i < simplified_lines.length; ++i)
    {
        Line *l = get_value(&simplified_lines, i);
        // printf("xstart: %i  ystart: %i  xend: %i, yend %i\n", l->xStart,
        //        l->yStart, l->xEnd, l->yEnd);
        draw_line(&draw_image, l);
    }

    for (size_t i = 0; i < dots.length; ++i)
    {
        Dot *dot = get_value(&dots, i);
        draw_dot2(&draw_image, dot, 6);
    }

    // Save the image
    // SDL_Surface *final_surface = create_surface(&draw_image);
    // SDL_SaveBMP(final_surface, image.path);

    // Free image and surface
    SDL_FreeSurface(final_surface);
    freeImage(&image);

    SDL_Quit();

    return EXIT_SUCCESS;
}
