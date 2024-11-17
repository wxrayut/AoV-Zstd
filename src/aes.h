

#ifndef AES_H
#define AES_H

#include "types.h"


#define HEADER_SIZE               4

/** 
 * The first 4 bytes of encrypted data, used to identify data encrypted with AES.
 * 
 * If the data's header matches the AES_HEADER, it indicates that the data 
 * has not been compressed using the Zstandard algorithm and likely uses AES 
 * encryption.
 */ 
extern const byte AES_HEADER[HEADER_SIZE];

#endif