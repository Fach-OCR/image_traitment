#ifndef BLOB_H
#define BLOB_H

void free_blob_list(MyList *list);

Dot *find_corners(Blob *blob);

MyList find_blob(Image *image);

#endif
