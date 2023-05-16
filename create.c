#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

/* create function creates file with name given in argv[1] including file extension */
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <filename> <directory>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    const char* directory = argv[2];

    char filepath[strlen(directory) + strlen(filename) + 2];
    sprintf(filepath, "%s/%s", directory, filename);

    FILE* fp = fopen(filepath, "w");
    if(fp == NULL) {
        printf("Error creating %s.\n", filepath);
        return 1;
    }

    printf("%s created successfully in %s.\n", filename, directory);

    fclose(fp);
    return 0;
}
