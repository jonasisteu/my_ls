#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "utils.h"

int includes(char *array[], int arrlength, char value[])
{
    for (int i = 0; i < arrlength; i++)
    {
        if (strcmp(array[i], value) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int arrlength(char **array)
{
    int count = 0;
    while (array[count] != NULL)
    {
        count++;
    }
    return count;
}

int content_length(char *entry)
{
    DIR *directory;
    struct dirent *content;

    directory = opendir(entry);
    content = readdir(directory);

    int count = 0;
    while (content != NULL)
    {
        count++;
        content = readdir(directory);
    }

    closedir(directory);

    return count;
}

void arrcontent(char **array, int arrlength)
{
    for (int i = 0; i < arrlength; i++)
    {
        printf("Contenu n°%d : %s\n", i + 1, array[i]);
    }

    if(arrlength > 0)
    {
        printf("\n");
    }

}