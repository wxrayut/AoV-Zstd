

#include <stdio.h>

#include "io.h"
#include "types.h"


/**
 * Reads the contents of a binary file and returns it as a `bytes` structure.
 *
 * @param path: The path to the file to be read.
 * @return: A pointer to a `bytes` structure containing the file data, 
 *          or `NULL` if the file could not be read or an error occurred.
 */
extern bytes *read_file(const char *path) {

    FILE *fptr = fopen(path, "rb");

    if (fptr == NULL) {
        return NULL;
    }
    
    fseek(fptr, 0, SEEK_END);

    long size = ftell(fptr);

    bytes *result = bytes_init((size_t)size);

    if (result == NULL || result->data == NULL) {
        fclose(fptr);
        return NULL;
    }

    fseek(fptr, 0, SEEK_SET);

    size_t rsize = fread(result->data, 1, result->size, fptr);

    if (rsize != result->size) {
        bytes_free(result);
        fclose(fptr);
        return NULL;
    }

    fclose(fptr);

    return result;
}


/**
 * Writes the contents of a `bytes` structure to a binary file.
 *
 * @param path: The path to the file where the data should be written.
 * @param b: The `bytes` structure containing the data to write.
 */
extern void write_file(const char *path, bytes *b) {

    FILE *fptr = fopen(path, "wb");

    if (fptr == NULL || b->data == NULL) {
        return;
    }

    fwrite(b->data, 1, b->size, fptr);

    fclose(fptr);
}