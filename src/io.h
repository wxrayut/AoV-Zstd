

#ifndef IO_H
#define IO_H

#include "types.h"


extern bytes *read_file(const char *path);
extern void write_file(const char *path, bytes *b);

#endif
