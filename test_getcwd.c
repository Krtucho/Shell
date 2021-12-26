#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef MAX_BUF
#define MAX_BUF 200
#endif

int main(void) {
    char path[MAX_BUF];

    getcwd(path, MAX_BUF);
    printf("Current working directory: %s\n", path);

    exit(EXIT_SUCCESS);
}