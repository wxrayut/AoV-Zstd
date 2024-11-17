

#ifndef ARGS_H
#define ARGS_H 

#include <getopt.h>
#include <stdbool.h>
#include <limits.h>


struct arguments {
    /* Flag to indicate whether to compress the data. */
    bool compress;

    /* Flag to indicate whether to decompress the data. */
    bool decompress;

    /* Compression level to be used for compression. */
    int compressionlevel;

    /* Directory path where the file is located. */
    char *dir;

    /* File path to be compress or decompress. */
    char *file;

    /* Path for the output file or directory after compression or decompression. */
    char *output;

    /* Flag to indicate whether to enabling verbose output. */
    bool verbose;

    /* Flag to indicate whether to display the version information of the program. */ 
    bool version;
};

typedef struct arguments arguments;


struct option_position {
    /* Position of the compress option in the argument list. */
    int compress;

    /* Position of the decompress option in the argument list. */
    int decompress;

    /* Position of the compression level option in the argument list. */
    int compressionlevel;

    /* Position of the directory option in the argument list. */
    int dir;

    /* Position of the file option in the argument list. */
    int file;

    /* Position of the output option in the argument list. */
    int output;

    /* Position of the verbose option in the argument list. */ 
    int verbose;
};

typedef struct option_position option_position;


struct option_error {
    /* Position of the first conflicting option in the argument list. */
    int pos_1;

    /* Position of the second conflicting option in the argument list. */
    int pos_2;
 
    /* Character representing the first conflicting option. */
    char opt_1;

    /* Character representing the second conflicting option. */
    char opt_2;
};

typedef struct option_error option_error;


enum {
    /* Default value for no option. */
    OPT_NONE                  = 0, 

    /* Option to specify compression. */
    OPT_COMPRESS              = 99, 

    /* Option to specify decompression. */
    OPT_DECOMPRESS            = 100, 

    /* Option to specify the compression level. */
    OPT_COMPRESSIONLEVEL      = 108, 

    /* Option to specify the directory path of the input file. */
    OPT_DIR                   = 68, 

    /* Option to specify the input file name. */
    OPT_FILE                  = 102, 

    /* Option to specify the output file or directory path. */
    OPT_OUTPUT                = 111, 

    /* Option to enable verbose output. */ 
    OPT_VERBOSE               = 86,

    /* Option to display help information. */
    OPT_HELP                  = 104, 

    /* Option to display version information. */
    OPT_VERSION               = 118
};


extern void args_init(arguments *args);
extern void args_parse(int argc, char *argv[], arguments *args);

#endif
