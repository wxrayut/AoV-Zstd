

#include <stdio.h>
#include <stdlib.h>

#include "args.h"


/**
 * Prints information about the command-line options used.
 * 
 * @param argc: The number of arguments passed.
 * @param argv: The array of arguments.
 * @param args: The structure containing parsed arguments (not used in this 
 *              function but can be expanded for more options).
 */
extern void opt_info(int argc, char *argv[], arguments *args) {

    printf("[%-7s] Used command line options: %s ", "INFO", argv[0]);
    
    for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }

    printf("\n");

    if (!args->verbose) {
        printf("\n");
    }
}


/**
 * Displays a warning message regarding a specific option.
 *
 * @param option: The command-line option that caused the warning.
 * @param message: The warning message to display.
 */
extern void opt_warn(const char *option, const char *message) {

    printf("[%-7s] option \e[0;33m'%s' \033[0;37m%s.\n", "WARN", option, message);
}


/**
 * Displays an error message about conflicting or incorrect options.
 *
 * @param opterr: A pointer to an `option_error` structure that 
 *                contains information about the conflicting options.
 */
extern void opt_error(option_error *opterr) {

    printf("[%-7s] command line options: ", "ERROR");

    /* Depending on the positions of the conflicting options, print them in order. */ 
    if (opterr->pos_1 > opterr->pos_2) {
        printf("\033[0;31m-%c -%c\n", opterr->opt_2, opterr->opt_1);
    } else {
        printf("\033[0;31m-%c -%c\n", opterr->opt_1, opterr->opt_2);
    }

    printf("\033[0;37m");
}

/**
 * Prints a simple usage guide.
 *
 * @param program_name: The name of the program, typically argv[0].
 */
extern void simple(const char *program_name) {

    /* Print a basic usage guide with an example of the command options. */ 
    printf("Usage: %s [-c | -d] [-f FILE | -D DIRECTORY]\n", program_name);
}


/**
 * Displays detailed usage instructions for the program.
 *
 * @param program_name: The name of the program, typically argv[0].
 */
extern void usage(const char *program_name) {
    printf("\nUsage: %s [MODE] [OPTIONS] [-f FILE | -D DIRECTORY]\n", program_name);
    printf("\nModes:\n");
    printf("  -c, --compress                Compress the specified file or directory.\n");
    printf("  -d, --decompress              Decompress the specified file or directory.\n");
    
    printf("\nOptions:\n");
    printf("  -l, --clevel LEVEL            Set the compression level (e.g., 1-22, with 22 being the highest).\n");
    printf("                                Default level is based on preset configurations.\n");
    printf("  -D, --dir DIRECTORY           Specify a directory to compress or decompress.\n");
    printf("                                Recommended for handling multiple files in a directory.\n");
    printf("  -f, --file FILE               Specify a single file to compress or decompress.\n");
    printf("  -o, --output OUTPUT           Specify the output path for the result (file or directory).\n");
    printf("                                If not provided, the input file or directory will be used.\n");
    printf("  -V, --verbose                 Enable verbose output, showing detailed progress.\n");
    printf("  -v, --version                 Display the program version and exit. This option cannot be used\n");
    printf("                                with any other options.\n");
    printf("  -h, --help                    Display this help message and exit.\n");
    
    printf("\nRecommendation:\n");
    printf("  For processing multiple files, it is recommended to use the '-D' option to specify a directory\n");
    printf("  instead of using '-f' for individual files.\n");

    printf("\nExamples:\n");
    printf("  %s -c -f input.txt -o output.zst\n", program_name);
    printf("      Compress 'input.txt' and save as 'output.zst'.\n");
    printf("  %s -d -D /input/dir -o /output/dir\n", program_name);
    printf("      Decompress all files in '/input/dir' to '/output/dir'.\n");

    // printf("\nNotes:\n");
    // printf("  1. The '-v' (version) option cannot be used in conjunction with other options.\n");
    // printf("  2. When no output option is provided, the result will overwrite the input file or\n");
    // printf("     directory, so ensure to back up your data if necessary.\n");
    printf("\n");
}
