#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>

/* флаг типа файла.
l - link, d - directory
f - file, a - агрумент не прописан => все три типа подходят */
char type_flag = 'a';

/* флаг на сортировку */
bool is_sort = false;

bool IS_RTYPE(struct dirent *info)
{
    switch (type_flag)
    {
    case 'l':
        return DT_LNK == info->d_type;
        break;
    case 'd':
        return DT_DIR == info->d_type;
        break;
    case 'f':
        return DT_REG == info->d_type;
        break;
    case 'a':
        return DT_LNK == info->d_type
        || DT_REG == info->d_type
        || DT_DIR == info->d_type;
        break;
    default:
        printf("incorrect options: there's no key %c", type_flag);
        break;
    }
    return false;
}

void recWalk(const char* dir_name)
{
    DIR *directory;
    /* namelist изначально NULL, тк scandir сам выделяет память, */
    struct dirent **namelist = NULL;
    struct dirent *tmp;
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

    if (is_sort)
    {
        numOfFiles = scandir(".", &namelist, NULL, alphasort);
        if(numOfFiles == -1)
        {
            perror("Scandir error");
            exit(1);
        }
    }
    else
    {
        while ((tmp = readdir(directory)) != NULL)
        {
            namelist = realloc(namelist, sizeof(struct dirent*) * (numOfFiles + 1));
            if(namelist == NULL)
            {
                perror("realloc error");
                exit(1);
            }
            namelist[numOfFiles] = tmp;
            numOfFiles++;
        }
    }

    while (numOfFiles--)
    {
        if (IS_RTYPE(namelist[numOfFiles]))
        {
            printf("%s\n", namelist[numOfFiles]->d_name);
        }

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

int main(int argc, char* argv[])
{
    char c;
    char* dir_name;
    /* Проверяем, передали ли директорию. Если директорию  передали, 
    то запоминаем указатель на нее, тк getopt_long меняет порядок в argv*/
    if(argc == 1 || argv[1][0] == '-')
    {
        dir_name = ".";
    }   
    else
    {
        dir_name = argv[1];
    }

    int option_index = 0;
    struct option option_types[] = {
            {"type", required_argument, 0, 0}
    };

    while((c = getopt_long(argc, argv, "ldfs",
                           option_types, &option_index)) != EOF)
    {
        switch (c)
        {
            /* --type */
            case 0:
                if(optarg)
                {
                    /* проверяем на корректный аргумент */
                    if (strcmp(optarg, "l") || strcmp(optarg, "d") || strcmp(optarg, "f"))
                    {
                        type_flag = optarg[0];
                    }
                    else
                    {
                        perror("Incorrect type option value");
                        exit(1);
                    }
                }
                break;
            /* -l */
            case 'l':
                type_flag ='l';
                break;
            /* -d */
            case 'd':
                type_flag = 'd';
                break;
            /* -f */
            case 'f':
                type_flag = 'f';
                break;
            /* -s */
            case 's':
                is_sort = true;
                break;

            default:
                perror("Incorrect option");
                exit(1);
                break;
        }
    }

    recWalk(dir_name);

}