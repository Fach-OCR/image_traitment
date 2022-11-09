#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/otsu.h"
#include "../../include/image_traitment/preprocess.h"
#include "../../include/image_traitment/utilis_image.h"
#include "../../include/image_traitment/gaussian_filter.h"
#include "../../include/image_traitment/grid_detection.h"
#include "../../include/image_traitment/hough_transform.h"
#include "../../include/image_traitment/blob.h"
#include "../../include/image_traitment/canny.h"

void compute_hough(Image *image)
{
    Image draw_image_hough = copy_image(image);
    int hough_threshold =
        image->width > image->height ? image->width / 4 : image->height / 4;

    MyList lines = hough_transform(image, hough_threshold);
    MyList simplified_lines = simplify_lines(&lines, 40);

    for (size_t i = 0; i < simplified_lines.length; ++i)
    {
        Line *l = get_value(&simplified_lines, i);
        draw_line(&draw_image_hough, l);
    }

    free_list(&lines);
    free_list(&simplified_lines);
    save_image(&draw_image_hough, "res_hough_");
    free_image(&draw_image_hough);
}

void compute_blob(Image *image)
{
    Image draw_image_blob = copy_image(image);
    MyList allblob = find_blob(&draw_image_blob);
    printf("nb bolb = %lu\n", allblob.length);
    free_blob_list(&allblob);

    save_image(&draw_image_blob, "res_blob_");
    free_image(&draw_image_blob);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Import image
    SDL_Surface *surface = IMG_Load(argv[1]);
    printf("Imported image of size %ix%i\n", surface->w, surface->h);
    Image tmp_image = create_image(surface, surface->w, surface->h);
    SDL_FreeSurface(surface);

    // Create the name to save image
    tmp_image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
    tmp_image.path[0] = 'r';
    tmp_image.path[1] = 'e';
    tmp_image.path[2] = 's';
    tmp_image.path[3] = '_';
    strcat(tmp_image.path, argv[1]);

    // Resize the image and free the others
    Image image = resize_image(&tmp_image, 800);
    free_image(&tmp_image);
    printf("new h = %i , new w = %i\n", image.height, image.width);

    // Preprocess
    surface_to_grayscale(&image);
    save_image(&image, "gray_scale_");
    image_contrast(&image, 12);
    save_image(&image, "contrast_");
    image_normalize_brightness(&image);
    save_image(&image, "brightness_");
    gaussian_blur(&image, 4);
    save_image(&image, "gaussian_blur_");

    canny_edge_detection(&image);

    compute_blob(&image);
    //    compute_hough(&image);

    save_image(&image, "");
    free_image(&image);

    SDL_Quit();

    return EXIT_SUCCESS;
}
