

#ifndef ZSTANDARD_H
#define ZSTANDARD_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#   include "zstd.h"
#elif __linux__
#   include <zstd.h>
#endif

#include "types.h"


#define HEADER_SIZE               4
#define FRAME_HEADER_SIZE         4

/**
 * The compression level for Arena of Valor game files.
 * After testing, it was determined that the optimal compression level 
 * is set to 19, which balances compression efficiency and performance.
 */ 
#define ZSTD_aov_compressionlevel 19


extern const byte HEADER[HEADER_SIZE];
extern const byte FRAME_HEADER[FRAME_HEADER_SIZE];

extern bool ZSTD_checkCLevel(const int clevel);
extern bool ZSTD_isHeader(const byte *data);
extern bool ZSTD_isNotDecompressedData(byte *data, const byte *header);

extern int ZSTD_getFrameHeaderIndex(const byte *data, size_t size);

extern bytes *ZSTD_setHeader(bytes *b, uint32_t dsize);
extern bytes *ZSTD_loadDictionary(const char *path);
extern bytes *ZSTD_extract_CompressData(bytes *b);
extern bytes *ZSTD_aov_compress(bytes *b, bytes *dict, int compressionlevel);
extern bytes *ZSTD_aov_decompress(bytes *b, bytes *dict);

#endif
