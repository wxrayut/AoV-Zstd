

#include <stdio.h>

#include "version.h"


/**
 * Displays version information, including the version number, 
 * build date, author, and license.
 */
extern void version(void) {
    /* Version. */ 
    printf("\nVersion: %s\n", VERSION);
    
    /* Build date. */
    printf("Build Date: %s\n", BUILD_DATE);

    /* Author. */ 
    printf("Author: %s\n", AUTHOR);

    /* License. */ 
    printf("License: %s\n", LICENSE);
    
    printf("\n");
}