#include "error.h"

void error(char *msg) {
    if (errno)
        fprintf(stderr, "Error: %s: %s\n", msg, strerror(errno));
    else
        fprintf(stderr, "Error: %s\n", msg);
        
    exit(EXIT_FAILURE);
}
