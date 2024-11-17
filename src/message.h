

#ifndef MESSAGE_H
#define MESSAGE_H

#include "args.h"
#include "types.h"


extern void opt_info(int argc, char *argv[], arguments *args);
extern void opt_warn(const char *option, const char *message);
extern void opt_error(option_error *opterr);

extern void simple(const char *program_name);
extern void usage(const char *program_name);

#endif
