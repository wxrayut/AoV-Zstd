

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "args.h"
#include "types.h"
#include "utils.h"
#include "zstandard.h"


/**
 * Joins a directory and file name into a single path.
 *
 * @param dir: The directory path.
 * @param file: The file name.
 * @return: string that represents the full path, or NULL 
 *         if allocation fails or input is invalid.
 */
extern char *path_join(const char *dir, const char *file) {
    
    if (!dir || !file) {
        return NULL;
    }

    char *result = (char *)malloc(
        strlen(dir) + strlen(SEPARATOR) + strlen(file) + 1
    );
    
    if (!result) {
        return NULL;
    }

    strcpy(result, dir);
    
    if (dir[strlen(dir) - 1] != SEPARATOR[0]) {
        strcat(result, SEPARATOR);
    }

    strcat(result, file);

    return result;
}


/**
 * Checks if the given path is a directory.
 *
 * @param path: The path to check.
 * @return: true if the path is a directory, false otherwise.
 */
extern bool isdir(const char *path) {

    if (path == NULL) {
        return false;
    }

    struct stat st;

    if (stat(path, &st) != 0) {
        return false;
    }

    return S_ISDIR(st.st_mode);
}


/**
 * Checks if the given path is a regular file.
 *
 * @param path: The path to check.
 * @return: true if the path is a regular file, false otherwise.
 */
extern bool isfile(const char *path) {

    if (path == NULL) {
        return false;
    }

    struct stat st;

    if (stat(path, &st) != 0) {
        return false;
    }

    return S_ISREG(st.st_mode);
}


extern void preview(const bytes *b, int start, int stop, int column) {

    /* Default to the beginning of the byte array. */ 
    if (start == 0) {
        start = 0;
    }

    /* Default to the size of the byte array. */ 
    if (stop == 0 || stop > b->size) {
        stop = (int)b->size;
    }

    /* Default to printing 16 bytes per line. */ 
    if (column == 0) {
        column = 16;
    }

    /* Ensure that the start and stop are within valid bounds. */
    if (start >= b->size) {
        return;
    }

    for (size_t i = start; i < stop; i += column) {        
        for (size_t j = 0; j < column; j++) {
            if (i + j < stop) {
                printf("%02X ", b->data[i + j]);
            } else {
                printf("   ");
            }
        }
        
        printf("| ");

        for (size_t j = 0; j < column; j++) {
            if (i + j < stop) {
                byte chr = b->data[i + j];
                if (chr >= 32 && chr <= 126) {
                    printf("%c", chr);
                } else {
                    printf(".");
                }
            }
        }

        printf("\n");
    }
}