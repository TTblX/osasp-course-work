#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "helper.h"

/* change_mode function sets file's permission mode given in argv[1] to mode given argv[2] */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <mode>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    const char *mode_str = argv[2];
    __mode_t mode = strtol(mode_str, NULL, 8);

    if (chmod(filename, mode) == -1) {
        perror("chmod");
        return EXIT_FAILURE;
    }

    printf("Changed mode of file '%s' to %04o\n", filename, mode);
    return EXIT_SUCCESS;
}
