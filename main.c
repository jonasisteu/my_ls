#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "components.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    // Stocker les directories ou le contenu d'un directory dans le même array, afin de pouvoir faire la même fonction check mais juste avec du contenu différent si l'option -d est effectuée ou pas

    char **directories = (char **) malloc((argc - 1) * sizeof(char *));
    char **flags = (char **) malloc((argc - 1) * sizeof(char *));

    if (!directories || !flags)
    {
        perror("Erreur d'allocation mémoire\n");
        free(directories);
        free(flags);
        return 1;
    }
    
    sort(directories, flags, argc, argv);

    if (directories[0] == NULL)
    {
        directories[0] = ".";
    }
    
    flag_check(directories, flags);

    free(directories);
    free(flags);

    return 0;
}