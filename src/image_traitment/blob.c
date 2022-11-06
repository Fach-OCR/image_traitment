#include <stdio.h>
#include <stdlib.h>

#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/utilis_image.h"


void rec_blob(Image *cimage, int i, int j,  MyList *current_blob)
{
    int w = cimage->width;
    int h = cimage->height;
    Pixel** pixels = cimage->pixels;

    set_all_pixel(cimage, i, j, 420);
    Dot dot = {.X = i, .Y = j};
    append(current_blob, Dot_tovptr(dot));
    for (int k = -1; k < 2; ++k)
    {
        for (int l = -1; l < 2; ++l)
        {
            if (i + k < 0 || i + k >= h || j + l < 0 || j + l >= w)
                continue;
            if (pixels[i + k][j + l].r != 255)
                continue;
            rec_blob(cimage, i + k, j + l, current_blob);
        }
    }
}


MyList find_blob(Image *image)
{
    int w = image->width;
    int h = image->height;
    Image cimage = copy_image(image);
    Pixel **pixels = cimage.pixels;
    MyList all_blob =  { NULL, NULL, 0};

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            if (pixels[i][j].r != 255)
                continue;
            MyList blob_list = { NULL, NULL, 0};
            rec_blob(&cimage, i, j, &blob_list);

            Blob final_blob;
            final_blob.length = blob_list.length;
            final_blob.dots = (Dot*)calloc(final_blob.length, sizeof(Dot));

            Node *n = blob_list.head;
            for (size_t k = 0; n != NULL; ++k, n = n->next)
            {
                Dot *tmp_dot = (Dot*)n->value;
                Dot new_dot = {.X = tmp_dot->X, .Y = tmp_dot->Y};
                final_blob.dots[k] = new_dot;
            }

            free_list(&blob_list);
            append(&all_blob, Blob_tovptr(final_blob));
        }
    }

    return all_blob;
}

