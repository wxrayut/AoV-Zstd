

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "aes.h"
#include "args.h"
#include "io.h"

#ifdef _WIN32
#   include "dirent.h"
#elif __linux__
#   include <dirent.h>
#endif

#include "message.h"
#include "types.h"
#include "utils.h"
#include "version.h"
#include "zstandard.h"


int main(int argc, char *argv[]) {

    /* Clear screen. */ 
    #ifdef _WIN32 
        system("cls");
    #else
        system("clear");
    #endif

    arguments args;

    /* Extract the program name from the full path. */
    argv[0] = basename(argv[0]);

    /* Initialize argument structure. */
    args_init(&args);

    /* This header is used to identify the uncompressed data.*/
    const byte AES_HEADER[HEADER_SIZE] = {0x22, 0x4A, 0x67, 0x00};

    /* Load the compression dictionary from the specified file. */
    bytes *dict = ZSTD_loadDictionary("./bin/dict.zst");

    if (argc > 1) {

        clock_t start = clock();

        args_parse(argc, argv, &args);
        
        if (!args.version) {
            version();
        }

        /* Display information about the options used. */ 
        opt_info(argc, argv, &args);

        /**
         * Requires the user to specify one of the following modes: 
         *      `--compress`   (-c) for compression
         *      `--decompress` (-d) for decompression
         */
        if (!(args.compress || args.decompress) || (args.compress && args.decompress)) {
            usage(argv[0]);
            return EXIT_FAILURE;
        }

        /* Default compression level of Arena Of Valor. */
        if (args.compress && !args.compressionlevel) {
            args.compressionlevel = ZSTD_aov_compressionlevel;
        }

        if (args.dir) {

            struct dirent *entry;

            DIR *dir = opendir(args.dir);

            if (dir == NULL) {
                return EXIT_FAILURE;
            }

            /* Determine output path if specified. */ 
            if (args.output) {
                if (/* If the specified output path does not exist. */ 
                    !isdir(args.output)) {

                    #ifdef _WIN32
                        mkdir(args.output);
                    #elif __linux__
                        mkdir(args.output, 0700);
                    #endif
                }
            }

            while ((entry = readdir(dir)) != NULL) {
                /* Skip the current directory (.) and parent directory (..). */ 
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                    continue;
                }
                
                char *path = path_join(args.dir, entry->d_name);
                bytes *b = read_file(path);
 
                /* Perform compression or decompression based on the flags. */ 
                if (args.compress) {

                    if (ZSTD_isNotDecompressedData(b->data, AES_HEADER)) {
                        continue;
                    }

                    /* Compress the data. */
                    b = ZSTD_aov_compress(b, dict, args.compressionlevel);

                } else if (args.decompress) {
                    /* Decompress the data. */ 
                    b = ZSTD_aov_decompress(b, dict);
                }

                /* Determine output path if specified. */ 
                if (args.output) {
                    path = path_join(args.output, entry->d_name);
                }

                if (args.verbose) {

                    printf("\n[%-7s] %s: %s\n", "INFO", "File", entry->d_name);
                    printf("[%-7s] %s: %s\n\n", "INFO", "Mode", args.compress ? "compression": "decompression");

                    preview(b, 0, 128, 16);

                    if (args.compress) {
                        printf("\n[%-7s] %s: %d\n", "INFO", "compression level", args.compressionlevel);
                    }

                    printf(args.decompress ? "\n" : "");
                    printf("[%-7s] %s: %zu bytes\n", "INFO", "Size", b->size);
                    printf("[%-7s] Output written to: %s\n", "INFO", path);

                }

                write_file(path, b);

                /* Free the allocated memory for the bytes. */
                bytes_free(b);
            }

            /* Close the directory stream. */ 
            closedir(dir);

        } else if (args.file) {

            char *path = args.file;
            bytes *b = read_file(path);

            /* Perform compression or decompression based on the flags. */ 
            if (args.compress) {

                if (ZSTD_isNotDecompressedData(b->data, AES_HEADER)) {
                    /* Pass. */
                } else {
                    /* Compress the data. */ 
                    b = ZSTD_aov_compress(b, dict, args.compressionlevel);
                }

            } else if (args.decompress) {
                /* Decompress the data. */ 
                b = ZSTD_aov_decompress(b, dict);
            }

            /* Determine output path if specified. */ 
            if (args.output) {
                /* If output is a directory, join it with the file's base name. */ 
                if (isdir(args.output)) {
                    path = path_join(args.output, basename(args.file));
                } else {
                    /* Use the specified output path. */ 
                    path = args.output;
                }
            }

            if (args.verbose) {

                printf("\n[%-7s] %s: %s\n", "INFO", "File", basename(args.file));
                printf("[%-7s] %s: %s\n\n", "INFO", "Mode", args.compress ? "compression": "decompression");

                preview(b, 0, 128, 16);

                if (args.compress) {
                    printf("\n[%-7s] %s: %d\n", "INFO", "compression level", args.compressionlevel);
                }

                printf(args.decompress ? "\n" : "");
                printf("[%-7s] %s: %zu bytes\n", "INFO", "Size", b->size);
                printf("[%-7s] Output written to: %s\n", "INFO", path);
            }

            write_file(path, b);

            /* Free the allocated memory for the bytes. */
            bytes_free(b);
        }

        clock_t end = clock();

        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

        if (args.verbose) {
            printf("\n[%-7s] Execution time: %f seconds\n\n", "INFO", time_spent);
        }

    } else {
        /* Handle case where no arguments are provided (optional). */ 
    }

    /* Free the loaded dictionary memory. */ 
    bytes_free(dict);

    return EXIT_SUCCESS;
}
