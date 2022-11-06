#include <SDL2/SDL_surface.h>
#include <err.h>

#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/otsu.h"
#include "../../include/image_traitment/preprocess.h"
#include "../../include/image_traitment/sobel.h"
#include "../../include/image_traitment/utilis_image.h"
#include "../../include/image_traitment/gaussian_filter.h"
#include "../../include/image_traitment/grid_detection.h"
#include "../../include/image_traitment/hough_transform.h"
#include "../../include/image_traitment/blob.h"


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
    printf("Imported image of size %ix%i\n", surface->w, surface->h);
    Image tmp_image = create_image(surface, surface->w, surface->h);

    // Create the name to save image
    tmp_image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
    tmp_image.path[0] = 'r';
    tmp_image.path[1] = 'e';
    tmp_image.path[2] = 's';
    tmp_image.path[3] = '_';
    strcat(tmp_image.path, argv[1]);

    // Resize the image and free the others
    int dim = 900;
    int w = tmp_image.width;
    int h = tmp_image.height;
    int rw = dim / w;
    int rh = dim / h;
    int new_h = rw > rh ? dim / rw : h;
    int new_w = rh > rw ? dim / rh : w;

    printf("new h = %i , new w = %i\n", new_h, new_w);
    Image image = resize_image(&tmp_image, new_w, new_h);
    SDL_FreeSurface(surface);
    free_image(&tmp_image);

    // Preprocess
    surface_to_grayscale(&image);
    image_contrast(&image, 10);
    image_normalize_brightness(&image);
    gaussian_blur(&image, 3);

    // Binarisation
    int otsuthresh = otsu(&image);
    apply_threshold(&image, otsuthresh);
    hysteris(&image);

    edges(&image);

    MyList allblob = find_blob(&image);
    printf("nb bolb = %lu\n", allblob.length);

/*     // Compute Hough transform
    w = image.width;
    h = image.height;
    int thresh = w > h ? w / 3 : h / 3;

    MyList all_lines = hough_transform(&image, thresh);
    MyList simplified_lines = simplify_lines(&all_lines, 60);
    printf("len simp = %li\n", simplified_lines.length);
    MyList squares = find_squares(&simplified_lines, &image);
    printf("square nb = %li", squares.length);

    Image draw_image = copy_image(&image);

    for (size_t i = 0; i < simplified_lines.length; ++i)
    {
        Line *l = get_value(&simplified_lines, i);
        draw_line(&draw_image, l);
    }

    SDL_Surface *final_surface = create_surface(&draw_image);
    SDL_SaveBMP(final_surface, image.path);

    // Free image and surface
    SDL_FreeSurface(final_surface);
    free_image(&image);
    free_image(&draw_image);

    //    free_list(&dots);
    free_list(&simplified_lines);
    free_list(&all_lines);
*/

    SDL_Surface *final_surface = create_surface(&image);
    SDL_SaveBMP(final_surface, image.path);

    SDL_Quit();

    return EXIT_SUCCESS;
}
