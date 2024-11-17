

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "args.h"
#include "io.h"
#include "types.h"
#include "utils.h"
#include "zstandard.h"


/** 
 * The first 4 bytes of the compressed data. If they do not match, the data 
 * will not be decompressed. 
 */
const byte HEADER[HEADER_SIZE] = {0x22, 0x4A, 0x00, 0xEF};

/**
 * Frame header data used in the Zstandard compression algorithm. This data 
 * can be used to locate the start of compressed data within a file and use 
 * for decompressed.
 */ 
const byte FRAME_HEADER[FRAME_HEADER_SIZE] = {0x28, 0xB5, 0x2F, 0xFD};


/**
 * Returns the minimum compression level for Zstandard.
 * 
 * Returns a fixed integer value that represents the minimum 
 * allowable compression level when using the Zstandard 
 * compression algorithm.
 * 
 * @return: The minimum compression level (1).
 */
int __zstandard_ZSTD_minCLevel(void) {

    return 1;
}


/**
 * Returns the maximum compression level for Zstandard.
 * 
 * Retrieves the maximum allowable compression level for the 
 * Zstandard compression algorithm by calling the Zstandard
 * library's `ZSTD_maxCLevel` function.
 * 
 * @return: The maximum compression level, as defined by the Zstandard library.
 */
int __zstandard_ZSTD_maxCLevel(void) {

    return ZSTD_maxCLevel();
}

/**
 * Checks if the specified compression level is within the allowable range.
 *
 * Compares the provided compression level against the
 * defined minimum and maximum compression levels for Zstandard. 
 * 
 * @param clevel: The compression level to check.
 * @return: `true` if the compression level is valid, `false` otherwise.
 */
extern bool ZSTD_checkCLevel(const int clevel) {

    int min, max;
    
    min = __zstandard_ZSTD_minCLevel();
    max = __zstandard_ZSTD_maxCLevel();

    bool isCLevel = true;

    if (clevel < min || clevel > max) {
        isCLevel = false;
    }
    
    return isCLevel;
}


typedef void (*cleanup_context_fn)(void *);
typedef void (*cleanup_dict_fn)(void *);

/**
 * Cleans up allocated resources for context, dictionary, and result buffer.
 * 
 * @param ctx: Pointer to a context (ZSTD_CCtx or ZSTD_DCtx).
 * @param ctx_cleanup_fn: Function to clean up the context, if provided.
 * @param dict: Pointer to a dictionary (ZSTD_CDict or ZSTD_DDict).
 * @param dict_cleanup_fn: Function to clean up the dictionary, if provided.
 * @param result: Pointer to a `bytes` buffer to be freed, if non-NULL.
 */
static void cleanup_resource(void *ctx, cleanup_context_fn ctx_cleanup_fn, 
                             void *dict, cleanup_dict_fn dict_cleanup_fn, 
                             bytes *result) {
    if (ctx && ctx_cleanup_fn) ctx_cleanup_fn(ctx);
    if (dict && dict_cleanup_fn) dict_cleanup_fn(dict);
    if (result) bytes_free(result);
}


/**
 * Checks if the provided data contains the correct Zstandard header.
 *
 * Compares the beginning of the provided data with the predefined HEADER to 
 * determine if it is valid Zstandard compressed data.
 * 
 * @param data: Pointer to the data to check.
 * @return: `true` if the data matches the HEADER, `false` otherwise.
 */
extern bool ZSTD_isHeader(const byte *data) {

    return memcmp(data, HEADER, HEADER_SIZE) == 0;
}


/** 
 * Checks if the provided data matches the specified header, indicating it is 
 * not decompressed data.
 * 
 * Compares the beginning of the provided data with the given header
 * to determine if the data is in a decompressed state. If the data matches the header,
 * it suggests that the data may still be in its compressed form and has not been
 * fully decompressed.
 * 
 * @param data: Pointer to the data to check.
 * @param header: Pointer to the expected header that indicates decompression status.
 * @return: `true` if the data matches the header, indicating it is not decompressed, 
 *          `false` otherwise.
 */ 
extern bool ZSTD_isNotDecompressedData(byte *data, const byte *header) {

    return memcmp(data, header, HEADER_SIZE) == 0;
}


/**
 * Searches for the frame header in the provided data.
 *
 * Iterates over the provided data to find the index of the
 * FRAME_HEADER. If found, it returns the index; otherwise, it returns -1.
 * 
 * @param data: Pointer to the data to search.
 * @param size: Size of the data.
 * @return: The index of the frame header if found, `-1` otherwise.
 */
extern int ZSTD_getFrameHeaderIndex(const byte *data, size_t size) {
    for (size_t i = 0; i < size - FRAME_HEADER_SIZE; i++) {
        if (memcmp(data + i, FRAME_HEADER, FRAME_HEADER_SIZE) == 0) {
            return (int)i;
        }
    }

    return -1;
}


/**
 * Prepares compressed data with a custom header and decompressed size.
 * 
 * Adds a custom header to the compressed data, and the size of the original 
 * uncompressed data. The resulting `bytes` structure contains the header 
 * followed by the compressed data.
 * 
 * @param b: Pointer to the `bytes` structure containing the compressed data.
 * @param dsize: The size of the uncompressed data.
 * @return: A pointer to a new `bytes` structure with the header prepended to the compressed data, 
 *          or NULL on failure.
 */
extern bytes *ZSTD_setHeader(bytes *b, uint32_t dsize) {

    bytes *out_buffer = bytes_init(HEADER_SIZE + FRAME_HEADER_SIZE + b->size);

    if (out_buffer == NULL) {
        return NULL;
    }

    memcpy(out_buffer->data, HEADER, HEADER_SIZE);

    for (int i = 0; i < HEADER_SIZE; i++) {
        out_buffer->data[HEADER_SIZE + i] = (dsize >> (8 * i)) & 0xFF;
    }
    
    memcpy(out_buffer->data + HEADER_SIZE + FRAME_HEADER_SIZE, b->data, b->size);

    cleanup_resource(NULL, NULL, NULL, NULL, b);

    return out_buffer;
}


/**
 * Loads a dictionary from the specified file path.
 *
 * Reads the contents of a file and returns it as a `bytes` structure, 
 * which can be used for decompression.
 * 
 * @param path: The path to the dictionary file.
 * @return: A pointer to the loaded dictionary as a `bytes` 
 *          structure, or NULL on failure.
 */
extern bytes *ZSTD_loadDictionary(const char *path) {

    return read_file(path);
}


/**
 * Extracts compressed data from the provided `bytes` structure.
 *
 * If the input data has the required size, extracts the decompressed size, 
 * finds the frame header, and allocates memory for the compressed data. If 
 * successful, it returns a new `bytes` structure containing the compressed data; 
 * otherwise, it returns NULL.
 * 
 * @param b: Pointer to the `bytes` structure containing the 
 *           compressed data.
 * @return: A pointer to a new `bytes` structure with the 
 *          compressed data, or NULL on failure.
 */
extern bytes *ZSTD_extract_CompressData(bytes *b) {

    /* Ensure the input data has at least the header, decompressed size, and frame header. */
    if (b->size < 12) {
        return NULL;
    }

    /* Find the frame header (which should be 4 bytes after the decompressed size). */ 
    int fh = ZSTD_getFrameHeaderIndex(b->data, b->size);
    if (fh == -1) {
        return NULL;
    }

    /* The size of the compressed data. */
    size_t csize = b->size - fh;

    bytes *result = bytes_init(csize);
    if (result == NULL) {
        return NULL;
    }

    /* Copy the compressed data into the result structure. */ 
    memcpy(result->data, b->data + fh, csize);

    return result;
}


/**
 * Compresses the given data using the Zstandard algorithm.
 * 
 * @param b: Pointer to the `bytes` structure containing the data to be compressed.
 * @param dict: Pointer to the `bytes` structure containing the dictionary used for compression.
 * @param compressionlevel: Compression level to be used, between the minimum and maximum 
 *                          allowable Zstandard compression levels.
 * @return: A pointer to a new `bytes` structure containing the compressed data, or NULL on failure.
 */
extern bytes *ZSTD_aov_compress(bytes *b, bytes *dict, int compressionlevel) {

    ZSTD_CCtx *cctx = ZSTD_createCCtx();
    if (cctx == NULL) {
        return NULL;
    }

    ZSTD_CCtx_reset(cctx, ZSTD_reset_session_only);

    ZSTD_CDict *cdict = ZSTD_createCDict(dict->data, dict->size, compressionlevel);
    if (cdict == NULL) {
        cleanup_resource(cctx, (cleanup_context_fn)ZSTD_freeCCtx, NULL, NULL, NULL);
        return NULL;
    }

    size_t code = ZSTD_CCtx_refCDict(cctx, cdict);

    if (ZSTD_isError(code)) {
        cleanup_resource(cctx, (cleanup_context_fn)ZSTD_freeCCtx, cdict, (cleanup_dict_fn)ZSTD_freeCDict, NULL);
        return NULL;
    }

    bytes *result = bytes_init(ZSTD_compressBound(b->size));

    if (result == NULL) {
        cleanup_resource(cctx, (cleanup_context_fn)ZSTD_freeCCtx, cdict, (cleanup_dict_fn)ZSTD_freeCDict, NULL);
        return NULL;
    }

    code = ZSTD_CCtx_setPledgedSrcSize(cctx, b->size);

    if (ZSTD_isError(code)) {
        cleanup_resource(cctx, (cleanup_context_fn)ZSTD_freeCCtx, cdict, (cleanup_dict_fn)ZSTD_freeCDict, result);
        return NULL;
    }

    ZSTD_inBuffer in_buffer = { b->data, b->size, 0 };
    ZSTD_outBuffer out_buffer = { result->data, result->size, 0 };

    code = ZSTD_compressStream2(cctx, &out_buffer, &in_buffer, ZSTD_e_end);

    if (ZSTD_isError(code) || code) {
        cleanup_resource(cctx, (cleanup_context_fn)ZSTD_freeCCtx, cdict, (cleanup_dict_fn)ZSTD_freeCDict, result);
        return NULL;
    }

    result->size = out_buffer.pos;

    cleanup_resource(cctx, (cleanup_context_fn)ZSTD_freeCCtx, cdict, (cleanup_dict_fn)ZSTD_freeCDict, NULL);

    return ZSTD_setHeader(result, (uint32_t)b->size);
}


/**
 * Decompresses the provided data using the Zstandard algorithm.
 * 
 * @param b: Pointer to the `bytes` structure containing the compressed data.
 * @param dict: Pointer to the `bytes` structure containing 
 *              the dictionary used for decompression.
 * @return: A pointer to a new `bytes` structure containing 
 *          the decompressed data, or NULL on failure.
 */
extern bytes *ZSTD_aov_decompress(bytes *b, bytes *dict) {

    if (!ZSTD_isHeader(b->data)) {
        return b;
    }

    b = ZSTD_extract_CompressData(b);
    if (b == NULL) {
        return NULL;
    }

    ZSTD_DCtx *dctx = ZSTD_createDCtx();
    if (dctx == NULL) {
        return NULL;
    }

    ZSTD_DDict *ddict = ZSTD_createDDict(dict->data, dict->size);
    if (ddict == NULL) {
        cleanup_resource(dctx, (cleanup_context_fn)ZSTD_freeDCtx, NULL, NULL, NULL);
        return NULL;
    }

    size_t code = ZSTD_DCtx_refDDict(dctx, ddict);
    if (ZSTD_isError(code)) {
        cleanup_resource(dctx, (cleanup_context_fn)ZSTD_freeDCtx, ddict, (cleanup_dict_fn)ZSTD_freeDDict, NULL);
        return NULL;
    }

    size_t output_size = ZSTD_getFrameContentSize(b->data, b->size);

    if (output_size == ZSTD_CONTENTSIZE_ERROR || output_size == ZSTD_CONTENTSIZE_UNKNOWN) {
        cleanup_resource(dctx, (cleanup_context_fn)ZSTD_freeDCtx, ddict, (cleanup_dict_fn)ZSTD_freeDDict, NULL);
        return NULL;
    }

    bytes *result = bytes_init(output_size);

    if (result == NULL) {
        cleanup_resource(dctx, (cleanup_context_fn)ZSTD_freeDCtx, ddict, (cleanup_dict_fn)ZSTD_freeDDict, NULL);
        return NULL;
    }

    ZSTD_inBuffer in_buffer = { b->data, b->size, 0 };
    ZSTD_outBuffer out_buffer = { result->data, result->size, 0 };

    code = ZSTD_decompressStream(dctx, &out_buffer, &in_buffer);

    if (ZSTD_isError(code)) {
        cleanup_resource(dctx, (cleanup_context_fn)ZSTD_freeDCtx, ddict, (cleanup_dict_fn)ZSTD_freeDDict, result);
        return NULL;
    }

    cleanup_resource(dctx, (cleanup_context_fn)ZSTD_freeDCtx, ddict, (cleanup_dict_fn)ZSTD_freeDDict, NULL);

    return result;

}