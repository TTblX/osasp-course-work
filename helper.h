#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

/* function is_path checks whether given char* is correct path or not. Return 1 if it is, 0 if it isn't , otherwise it's error*/
int is_path(char* path) {
    struct stat st;
    if (stat(path, &st) == 0) return 1;
    return 0;
}