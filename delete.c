#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

/* delete function deletes file given in argv[1] */
int main(int argc, char* argv[]) {
    if (!is_path( argv[1]))
    {
        perror(argv[1]);
        exit(2);
    }

    if(remove(argv[1]) == 0) {
        printf("%s deleted successfully.\n", argv[1]);
    } else {
        printf("Error deleting %s.\n", argv[1]);
    }
    return 0;
}
