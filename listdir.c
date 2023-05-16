#define _DEFAULT_SOURCE

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include "helper.h"

/* listfile lists given file like ls -la */
void listfile(char* name)
{
    struct stat sb;
    char *modtime;
    

    char *filetype[] = {"?", "p", "c", "?", "d", "?", "b", "?", ".", "?", "l", "?", "s"};

    if(stat(name, &sb) < 0)
    {
        perror(name);
        exit(2);
    }
    

    printf("%s", filetype[(sb.st_mode >> 12) & 017]);


    printf("%c%c%c%c%c%c%c%c%c", 
            (sb.st_mode & S_IRUSR) ? 'r' : '-',
            (sb.st_mode & S_IWUSR) ? 'w' : '-',
            (sb.st_mode & S_IXUSR) ? 'x' : '-',
            (sb.st_mode & S_IRGRP) ? 'r' : '-',
            (sb.st_mode & S_IWGRP) ? 'w' : '-',
            (sb.st_mode & S_IXGRP) ? 'x' : '-',
            (sb.st_mode & S_IROTH) ? 'r' : '-',
            (sb.st_mode & S_IWOTH) ? 'w' : '-',
            (sb.st_mode & S_IXOTH) ? 'x' : '-');

    printf("%8ld", sb.st_size);

    modtime = ctime(&sb.st_mtime);
    modtime[strlen(modtime) - 1] = '\0';
    printf(" %s ", modtime);
    printf("%s\n", name);
    
}

/* lists all files in given directory like ls -la */
void main(int argc, char * argv[])
{
    DIR *d;
    struct dirent * info;
    if(argc != 2){
        fprintf(stderr, "usage: listdir dirname\n");
        exit(1);
    }
    
    if (!is_path( argv[1]))
    {
        perror(argv[1]);
        exit(2);
    }
    
    chdir(argv[1]);
    d = opendir(argv[1]);

    while((info = readdir(d)) != NULL)
    {
        if (info->d_type == DT_LNK)
        {
            continue;
        }
        
        listfile(info->d_name);
    }

    closedir(d);  
}