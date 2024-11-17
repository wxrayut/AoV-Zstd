

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "args.h"
#include "message.h"
#include "utils.h"
#include "version.h"
#include "zstandard.h"


/**
 * Initializes the arguments structure with default values.
 *
 * @param args: A pointer to the arguments structure to be initialized.
 */
extern void args_init(arguments *args) {
    /* Initialize all options to their default states. */
    args->compress = false;          /* Compression is off by default. */
    args->decompress = false;        /* Decompression is off by default. */
    args->compressionlevel = 0;      /* Compression level defaults to 0. */
    args->dir = NULL;                /* Directory is NULL by default. */
    args->file = NULL;               /* File is NULL by default. */
    args->output = NULL;             /* Output file path is NULL by default. */
    args->verbose = false;           /* Verbose output is off by default. */
    args->version = false;           /* Version flag is off by default. */
}


/**
 * Parses command-line arguments and updates the arguments structure.
 *
 * @param argc: The number of command-line arguments.
 * @param argv: The array of command-line arguments.
 * @param args: A pointer to the arguments structure to store the parsed options.
 */
extern void args_parse(int argc, char *argv[], arguments *args) {

    /* The short options string. */
    static const char *options = "cdl:D:f:o:Vhv";

    /* The long options structure. */
    static const struct option long_options[] = {
        { "compress",         no_argument,       NULL, OPT_COMPRESS }, 
        { "decompress",       no_argument,       NULL, OPT_DECOMPRESS }, 
        { "clevel",           required_argument, NULL, OPT_COMPRESSIONLEVEL }, 
        { "dir",              required_argument, NULL, OPT_DIR }, 
        { "file",             required_argument, NULL, OPT_FILE }, 
        { "output",           required_argument, NULL, OPT_OUTPUT }, 
        { "verbose",          no_argument,       NULL, OPT_VERBOSE }, 
        { "version",          no_argument,       NULL, OPT_VERSION }, 
        { "help",             no_argument,       NULL, OPT_HELP }, 
        { NULL,               0,                 NULL, OPT_NONE }
    };

    /* Program name for error and help messages. */ 
    char *program_name = argv[0];

    /* The current option parsed by getopt. */
    int option;

    /* The compression level. */
    int clevel;

    /* Track the position of options for conflict detection. */
    int pos = 1;
    option_position optpos[] = {{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}};

    while ((option = getopt_long(argc, argv, options, long_options, NULL)) != -1) {
        switch (option) {
            case OPT_COMPRESS:
                args->compress = true;
                optpos->compress = pos++;
                break;

            case OPT_COMPRESSIONLEVEL:

                clevel = atoi(optarg);

                if (ZSTD_checkCLevel(clevel)) {
                    args->compressionlevel = clevel;
                }

                optpos->compressionlevel = pos++;
                break;

            case OPT_DECOMPRESS:
                args->decompress = true;
                optpos->decompress = pos++;
                break;

            case OPT_DIR:
                args->dir = optarg;

                // if (!isdir(args->dir)) {
                //     printf("[%-7s] The specified directory '%s' does not exist or\n          is not a directory.\n", "ERROR", args->dir);
                //     exit(EXIT_FAILURE);
                // }

                optpos->dir = pos++;
                break;

            case OPT_FILE:
                args->file = optarg;

                // if (!isfile(args->file)) {
                //     printf("[%-7s] The specified file '%s' does not exist or\n          is not a regular file.\n", "ERROR", args->file);
                //     exit(EXIT_FAILURE);
                // }

                optpos->file = pos++;
                break;

            case OPT_OUTPUT:
                args->output = optarg;
                optpos->output = pos++;
                break;

            case OPT_VERBOSE:
                args->verbose = true;
                optpos->verbose = pos++;
                break;

            case OPT_VERSION:
                version();
                exit(EXIT_SUCCESS);

            case OPT_HELP:
                usage(program_name);
                exit(EXIT_SUCCESS);

            default:
                simple(program_name);
                break;
        }
    }

    /**
     * Handle conflicts between incompatible command-line options.
     */ 
    option_error *opterr = (option_error *)malloc(sizeof(option_error));

    if (args->compress && args->decompress) {

        opterr->pos_1 = optpos->compress;
        opterr->pos_2 = optpos->decompress;

        opterr->opt_1 = OPT_COMPRESS;
        opterr->opt_2 = OPT_DECOMPRESS;

        opt_error(opterr);

        printf("          cannot use both -c (--compress) and -d (--decompress) modes together.\n");
        printf("          choose either -c (--compress) or -d (--decompress), but not both.\n");

        exit(EXIT_FAILURE);
    }
    
    if (args->decompress && args->compressionlevel) {

        opterr->pos_1 = optpos->compress;
        opterr->pos_2 = optpos->compressionlevel;

        opterr->opt_1 = OPT_DECOMPRESS;
        opterr->opt_2 = OPT_COMPRESSIONLEVEL;

        opt_error(opterr);

        printf("          compression level is not applicable for decompression.\n");

        exit(EXIT_FAILURE);
    }

    if (args->dir && args->file) {

        opterr->pos_1 = optpos->dir;
        opterr->pos_2 = optpos->file;

        opterr->opt_1 = OPT_DIR;
        opterr->opt_2 = OPT_FILE;

        opt_error(opterr);

        printf("          cannot use both -f (--file) and -D (--dir) options at the same time.\n");
        printf("          choose either -f (--file) or -D (--dir), but not both.\n");

        exit(EXIT_FAILURE);
    }

    /* Free memory allocated for option error tracking. */ 
    free(opterr);
}
