

#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <stddef.h>


/**
 * Defines a mutable string (null-terminated char array).
 * Use this for strings that you intend to modify.
 */
typedef char* str;

/**
 * Defines an immutable string (null-terminated constant char array).
 * Use this for strings that should not be modified.
 */
typedef const char* cstr;

/**
 * Defines an array of mutable strings (array of null-terminated char arrays).
 * This is useful for handling arrays of strings like command-line arguments.
 */
typedef char* strarray[];

/**
 * Defines an unsigned byte type (0-255).
 * This is useful for handling raw binary data.
 */
typedef unsigned char byte;

/**
 * Represents a dynamic byte array with its size.
 * Use this struct to manage binary data buffers.
 */
struct bytes
{
    /* Pointer to the raw byte data. */ 
    byte *data;
    
    /* Number of bytes in the data array. */ 
    size_t size;
};

typedef struct bytes bytes;


/**
 * Initializes a bytes structure.
 * Allocates memory for the byte buffer and sets the size.
 * 
 * @param size: The number of bytes to allocate.
 * 
 * @return: A `bytes` struct with allocated memory, or {NULL, 0} if allocation fails.
 */
static inline bytes *bytes_init(size_t size) {
    bytes *b = (bytes *)malloc(sizeof(bytes));
    if (b == NULL) {
        return NULL;
    }
    b->data = (byte *)malloc(size);
    if (b->data != NULL) {
        b->size = size;
    } else {
        b->size = 0;
    }

    return b;
}


/**
 * Frees the memory allocated for the byte array in a `bytes` structure.
 * 
 * @param b: A pointer to the `bytes` structure whose memory should be freed.
 */
static inline void bytes_free(bytes *b) {
    if (b && b->data) {
        free(b->data);
        b->data = NULL;
        b->size = 0;
    }
    
    free(b);
}

#endif
