

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <libgen.h>

#ifdef _WIN32
    #define SEPARATOR "\\"
#else 
    #define SEPARATOR "/"
#endif


#define BLACK     "\033[0;30m"
#define RED       "\033[0;31m"
#define GREEN     "\033[0;32m"
#define YELLOW    "\033[0;33m"
#define BLUE      "\033[0;34m"
#define PURPLE    "\033[0;35m"
#define CYAN      "\033[0;36m"
#define WHITE     "\033[0;37m"
#define RESET     "\033[0m"


extern char *dirname(char *path);
extern char *basename(char *path);
extern char *path_join(const char *dir, const char *file);

extern bool isdir(const char *path);
extern bool isfile(const char *path);

extern void preview(const bytes *b, int start, int stop, int column);

#endif