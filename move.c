#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "helper.h"

/* move functions moves file given in argv[1] to dest in argv[2] */
int main(int argc, char *argv[]) {
    if (argc != 3) { // Check if both arguments are provided
        printf("Usage: %s <file> <directory>\n", argv[0]);
        return 1;
    }
    
    if (!is_path( argv[1]))
    {
        perror(argv[1]);
        exit(2);
    }

    if (!is_path( argv[2]))
    {
        perror(argv[2]);
        exit(2);
    }

    // Construct the full path of the destination file
    char dest[256];
    snprintf(dest, sizeof(dest), "%s/%s", argv[2], argv[1]);
    
    // Move the file to the destination directory
    if (rename(argv[1], dest) < 0) {
        perror("Error moving file");
        return 1;
    }
    
    printf("%s successfully moved to %s\n", argv[1], argv[2]);
    return 0;
}
