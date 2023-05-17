#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

/* change_mode function sets file's permission mode given in argv[1] to mode given argv[2] */
int change_mode(char* filepath, char* mode_str) {

    if (!is_path( filepath))
    {
        perror(filepath);
        exit(2);
    }

    __mode_t mode = strtol(mode_str, NULL, 8);

    if (chmod(filepath, mode) == -1) {
        perror("chmod");
        return EXIT_FAILURE;
    }

    printf("Changed mode of file '%s' to %04o\n", filepath, mode);
    return EXIT_SUCCESS;
}


/* create function creates file with name given in argv[1] including file extension */
int create(char* filename, char* directory) {

    if (!is_path(directory))
    {
        perror(directory);
        exit(2);
    }

    char filepath[strlen(directory) + strlen(filename) + 2];
    sprintf(filepath, "%s/%s", directory, filename);

    FILE* fp = fopen(filepath, "w");
    if(fp == NULL) {
        printf("Error creating %s.\n", filename);
        return 1;
    }

    printf("%s created successfully in %s.\n", filename, directory);

    fclose(fp);
    return 0;
}


/* remove_file function deletes file given in argv[1] */
int remove_file(char* filepath) {
    if (!is_path( filepath))
    {
        perror(filepath);
        exit(2);
    }

    if(remove(filepath) == 0) {
        printf("%s deleted successfully.\n", filepath);
    } else {
        printf("Error deleting %s.\n", filepath);
    }
    return 0;
}

/* function is_path checks whether given char* is correct path or not. Return 1 if it is, 0 if it isn't , otherwise it's error*/
int is_path(char* path) {
    struct stat st;
    if (stat(path, &st) == 0) return 1;
    return 0;
}


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
void listdir(char * directory)
{
    DIR *d;
    struct dirent * info;
    // if(argc != 2){
    //     fprintf(stderr, "usage: listdir dirname\n");
    //     exit(1);
    // }
    
    if (!is_path( directory))
    {
        perror(directory);
        exit(2);
    }
    
    chdir(directory);
    d = opendir(directory);

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


/* move functions moves file given in argv[1] to dest in argv[2] */
int move(char* filepath, char* destination) {
    
    if (!is_path( filepath))
    {
        perror(filepath);
        exit(2);
    }

    if (!is_path( destination))
    {
        perror(destination);
        exit(2);
    }

    // Construct the full path of the destination file
    char dest[256];
    snprintf(dest, sizeof(dest), "%s/%s", destination, filepath);
    
    // Move the file to the destination directory
    if (rename(filepath, dest) < 0) {
        perror("Error moving file");
        return 1;
    }
    
    printf("%s successfully moved to %s\n",filepath, destination);
    return 0;
}


void recWalk(const char* dir_name)
{
    DIR *directory;
    /* namelist изначально NULL, тк scandir сам выделяет память, */
    struct dirent **namelist = NULL;
    int numOfFiles = 0;

    if ((directory = opendir(dir_name)) == NULL)
    {
        perror(dir_name);
        printf("opendir error\n");

        exit(1);
    }
    /* смена рабочей директории на ту, которую проходим, чтобы в дальнейшем можно было
    открывать директорию с помощью только ее имени*/
    chdir(dir_name);

   
    numOfFiles = scandir(".", &namelist, NULL, alphasort);
    if(numOfFiles == -1)
    {
        perror("Scandir error");
        exit(1);
    }

    while (numOfFiles--)
    {
        printf("%s\n", namelist[numOfFiles]->d_name);

        if (namelist[numOfFiles]->d_type == DT_DIR
        && namelist[numOfFiles]->d_name[0] != '.')
        {
            recWalk((const char*)namelist[numOfFiles]->d_name);
        }
    }

    free(namelist);

    closedir(directory);
    chdir("..");
}

/* dirwalk function recursively traverses files in given directory*/
int dirwalk(char* directory)
{
    if (!is_path( directory))
    {
        perror(directory);
        exit(2);
    }

    recWalk(directory);

}