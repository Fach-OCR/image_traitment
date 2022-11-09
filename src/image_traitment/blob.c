#include "../../include/image_traitment/blob.h"

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

int manhattan_distance(Dot *d1, Dot *d2)
{
    int x1 = d1->X;
    int x2 = d2->X;
    int y1 = d1->Y;
    int y2 = d2->Y;
    return abs(x2 - x1) + abs(y2 - y1);
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

MyList clear_blob(MyList *all_blob, int *index_max)
{
    MyList cleared_blob = { NULL, NULL, 0 };
    Node *n = all_blob->head;
    // Get average length of each blob
    int average = 0;
    for (int i = 0; n != NULL; n = n->next, ++i)
    {
        Blob *tmp_blob = (Blob *)n->value;
        average += tmp_blob->length;
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

    int max = 0;
    for (size_t i = 0; i < cleared_blob.length; ++i)
    {
        Blob *tmp_blob = get_value(&cleared_blob, i);
        if (tmp_blob->length > max)
        {
            max = tmp_blob->length;
            *index_max = (int)i;
        }
    }

    return cleared_blob;
}

/*
 *res[0]: top left A
 *res[1]: top right B
 *res[2]: bot left D
 *res[3]: bot right C
 */
Dot *find_corners(Blob *blob, Image *image)
{
    int len = blob->length;
    int w = image->width;
    int h = image->height;
    Dot top_left, top_right, bot_left, bot_right;

    int dist_A = INT_MAX;
    Dot dot_A = { .X = 0, .Y = h };
    int dist_B = INT_MAX;
    Dot dot_B = { .X = w, .Y = h };
    int dist_C = INT_MAX;
    Dot dot_C = { .X = w, .Y = 0 };
    int dist_D = INT_MAX;
    Dot dot_D = { .X = 0, .Y = 0 };
    int tmp_dist;
    for (int i = 0; i < len; ++i)
    {
        Dot d = blob->dots[i];
        tmp_dist = manhattan_distance(&d, &dot_A);
        if (tmp_dist < dist_A)
        {
            top_left.X = d.X;
            top_left.Y = d.Y;
            dist_A = tmp_dist;
        }
        tmp_dist = manhattan_distance(&d, &dot_B);
        if (tmp_dist < dist_B)
        {
            top_right.X = d.X;
            top_right.Y = d.Y;
            dist_B = tmp_dist;
        }
        tmp_dist = manhattan_distance(&d, &dot_C);
        if (tmp_dist < dist_C)
        {
            bot_right.X = d.X;
            bot_right.Y = d.Y;
            dist_C = tmp_dist;
        }
        tmp_dist = manhattan_distance(&d, &dot_D);
        if (tmp_dist < dist_D)
        {
            bot_left.X = d.X;
            bot_left.Y = d.Y;
            dist_C = tmp_dist;
        }
    }
    Dot *res = (Dot *)calloc(4, sizeof(Dot));
    res[0] = top_left;
    res[1] = top_right;
    res[2] = bot_right;
    res[3] = bot_left;

    return res;
}

void draw_blob(Image *image, MyList *all_blob)
{
    Node *n = all_blob->head;
    int size = 1;
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
    int index_max;
    MyList cleared_blob = clear_blob(&all_blob, &index_max);
    free_blob_list(&all_blob);
    free_image(&cimage);
    draw_blob(image, &cleared_blob);
    printf("%zu\n", cleared_blob.length);

    Blob *biggest_blob = get_value(&cleared_blob, index_max);
    printf("%d\n", biggest_blob->length);
    Dot *corners = find_corners(biggest_blob, image);
    for (int i = 0; i < 4; ++i)
    {
        draw_dot(image, &corners[i], 6);
    }

    free(corners);

    return cleared_blob;
}
