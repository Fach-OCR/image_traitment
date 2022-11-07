#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/utilis_image.h"

void free_blob_list(MyList *list)
{
    Node *n = list->head;
    for (; n != NULL; n = n->next)
    {
        Blob *blob = (Blob *)n->value;
        free(blob->dots);
    }
    free_list(list);
}

Blob copy_blob(Blob *blob)
{
    Blob res = { .length = blob->length, .dots = NULL };
    res.dots = (Dot *)calloc(blob->length, sizeof(Dot));
    for (int i = 0; i < blob->length; ++i)
    {
        Dot tmp = { .X = blob->dots[i].X, .Y = blob->dots[i].Y };
        res.dots[i] = tmp;
    }
    return res;
}

void rec_blob(Image *cimage, int i, int j, MyList *current_blob)
{
    int w = cimage->width;
    int h = cimage->height;
    Pixel **pixels = cimage->pixels;

    set_all_pixel(cimage, i, j, 420);
    Dot dot = { .X = i, .Y = j };
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

MyList clear_blob(MyList *all_blob)
{
    MyList cleared_blob = { NULL, NULL, 0 };
    Node *n = all_blob->head;
    // Get average length of each blob
    int average = 0;
    for (int i = 0; n != NULL; n = n->next, ++i)
    {
        Blob *tmp_blob = (Blob *)n->value;
        average += tmp_blob->length;
        // printf("nb %i size = %i\n", i, tmp_blob->length);
    }
    average /= all_blob->length;

    // Remove blob that are under the average size
    n = all_blob->head;
    for (size_t i = 0; n != NULL; n = n->next, ++i)
    {
        Blob *tmp_blob = (Blob *)n->value;
        if (tmp_blob->length > average)
        {
            Blob new_blob = copy_blob(tmp_blob);
            append(&cleared_blob, Blob_tovptr(new_blob));
        }
    }

    return cleared_blob;
}

void draw_blob(Image *image, MyList *all_blob)
{
    Node *n = all_blob->head;
    int size = 2;
    int width = image->width;
    int height = image->height;
    for (; n != NULL; n = n->next)
    {
        Blob *tmp_blob = (Blob *)n->value;
        for (int b = 0; b < tmp_blob->length; ++b)
        {
            Dot dot = tmp_blob->dots[b];
            int x = dot.X;
            int y = dot.Y;
            for (int i = -size; i < size; ++i)
            {
                for (int j = -size; j < size; ++j)
                {
                    if (x + i >= 0 && x + i < height && j + y >= 0
                        && j + y < width)
                        image->pixels[x + i][y + j].b = 255;
                }
            }
        }
    }
}

MyList find_blob(Image *image)
{
    int w = image->width;
    int h = image->height;
    Image cimage = copy_image(image);
    Pixel **pixels = cimage.pixels;
    MyList all_blob = { NULL, NULL, 0 };

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            if (pixels[i][j].r != 255)
                continue;
            MyList blob_list = { NULL, NULL, 0 };
            rec_blob(&cimage, i, j, &blob_list);

            Blob final_blob;
            final_blob.length = blob_list.length;
            final_blob.dots = (Dot *)calloc(final_blob.length, sizeof(Dot));

            Node *n = blob_list.head;
            for (size_t k = 0; n != NULL; ++k, n = n->next)
            {
                Dot *tmp_dot = (Dot *)n->value;
                Dot new_dot = { .X = tmp_dot->X, .Y = tmp_dot->Y };
                final_blob.dots[k] = new_dot;
            }

            free_list(&blob_list);
            append(&all_blob, Blob_tovptr(final_blob));
        }
    }
    MyList cleared_blob = clear_blob(&all_blob);
    free_blob_list(&all_blob);
    free_image(&cimage);
    draw_blob(image, &cleared_blob);

    return cleared_blob;
}
