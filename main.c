#define _DEFAULT_SOURCE

#include <getopt.h>
#include "utils.c"

int main(int argc, char* argv[])
{
    char c;
    int option_index = 0;
    struct option option_types[] = {
            {"create", required_argument, 0, 'c'},
            {"remove", required_argument, 0, 'r'},
            {"dirwalk", required_argument, 0, 'd'},
            {"listdir", required_argument, 0, 'l'},
            {"move", required_argument, 0, 'm'},
            {"chmod", required_argument, 0, 'h'}
    };

    while((c = getopt_long(argc, argv, "cr:d:l:mh",
                           option_types, &option_index)) != EOF)
    {
        switch (c)
        {
            case 'c':
                if (argc < 3) {
                   printf("Usage: %s <filename> <directory>\n", argv[0]);
                   return 1;
                }
                create(argv[argc - 2], argv[argc - 1]);
                break;
            case 'r':
                remove_file(optarg);
                break;
            case 'd':
                dirwalk(optarg);
                break;
            case 'l':
                listdir(optarg);
                break;
            case 'm':
                if (argc < 3) { // Check if both arguments are provided
                    printf("Usage: sudo %s <file> <directory>\n", argv[1]);
                    return 1;
                }
                move(argv[argc - 2], argv[argc - 1]);
                break;
            case 'h':
                if (argc < 3) {
                    fprintf(stderr, "Usage: sudo %s <filepath> <mode>\n", argv[1]);
                    return EXIT_FAILURE;
                }
                change_mode(argv[argc - 2], argv[argc - 1]);
                break;
            default:
                perror("Incorrect option");
                exit(1);
                break;
        }
    }
}