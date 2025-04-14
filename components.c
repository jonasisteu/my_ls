#include "utils.h"
#include "components.h"
// ------- variable globale ----------
int reverse = 0;
typedef struct {
    char *name;
    time_t mtime;
} FileInfo;


// -------- tri ----------------
int compare_mtime(const void *a, const void *b) {
    FileInfo *fileA = (FileInfo *)a;
    FileInfo *fileB = (FileInfo *)b;
    int result = (fileB->mtime - fileA->mtime);
    return reverse ? -result : result;  // inverse si -r est activé
}

int compare_alpha(const void *a, const void *b) {
    FileInfo *fileA = (FileInfo *)a;
    FileInfo *fileB = (FileInfo *)b;
    int result = strcasecmp(fileA->name, fileB->name);
    return reverse ? -result : result;  // inverse si -r est activé
}

//éviter les doubles 
void build_path(char *dest, size_t size, const char *dir, const char *file) {
    if (dir[strlen(dir) - 1] == '/') {
        snprintf(dest, size, "%s%s", dir, file);
    } else {
        snprintf(dest, size, "%s/%s", dir, file);
    }
}

// séparer les flag et les directories
void sort(char **directories, char **flags, int argc, char *argv[]) {
    int flagCount = 0;
    int directCount = 0;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')  //  vérifie si c'est un flag
        {
            if (directCount > 0) {
                // Force le fait de devoir mettre des flags avant les directories
                directories[directCount] = argv[i];
                directCount++;
                continue;
            }

            //  Décomposer les flags collés
            for (int j = 1; argv[i][j] != '\0'; j++) {
                char *flag = (char *) malloc(3 * sizeof(char));
                if (!flag) {
                    perror("Erreur d'allocation mémoire");
                    exit(1);
                }
                flag[0] = '-';
                flag[1] = argv[i][j];  // Ajoute une seule lettre
                flag[2] = '\0';

                flags[flagCount] = flag;  //  Ajoute le flag séparé
                flagCount++;
            }
        }
        else
        {
            // ajoute les directories dans un array
            directories[directCount] = argv[i];
            directCount++;
        }
    }

    //  ajout d'un NULL de fin pour éviter les accès non valides
    directories[directCount] = NULL;
    flags[flagCount] = NULL;
}

// affichage de -l
void print_file_details(char *filename, int tagLUsed) {
    struct stat file_stat;

    // Choisir entre stat() et lstat() selon si -L est activé
    if (tagLUsed) {
        if (stat(filename, &file_stat) == -1) {
            perror("stat");
            return;
        }
    } else {
        if (lstat(filename, &file_stat) == -1) {
            perror("lstat");
            return;
        }
    }

    char permissions[11];

    // Déterminer le type de fichier
    permissions[0] = S_ISDIR(file_stat.st_mode) ? 'd' :
                     S_ISLNK(file_stat.st_mode) ? 'l' : '-';

    // Droits utilisateur
    permissions[1] = file_stat.st_mode & S_IRUSR ? 'r' : '-';
    permissions[2] = file_stat.st_mode & S_IWUSR ? 'w' : '-';
    permissions[3] = file_stat.st_mode & S_IXUSR ? 'x' : '-';

    // Droits groupe
    permissions[4] = file_stat.st_mode & S_IRGRP ? 'r' : '-';
    permissions[5] = file_stat.st_mode & S_IWGRP ? 'w' : '-';
    permissions[6] = file_stat.st_mode & S_IXGRP ? 'x' : '-';

    // Droits autres
    permissions[7] = file_stat.st_mode & S_IROTH ? 'r' : '-';
    permissions[8] = file_stat.st_mode & S_IWOTH ? 'w' : '-';
    permissions[9] = file_stat.st_mode & S_IXOTH ? 'x' : '-';
    permissions[10] = '\0';

    // récupération des infos utilisateur et groupe
    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);

    // récupération de la date de modification
    char time_str[20];
    struct tm timeinfo;
    localtime_r(&file_stat.st_mtime, &timeinfo);
    strftime(time_str, sizeof(time_str), "%b %d %H:%M", &timeinfo);

    // affichage des détails
    printf("%s %ld %s %s %ld %s %s",
           permissions,
           file_stat.st_nlink,
           pw ? pw->pw_name : "?",
           gr ? gr->gr_name : "?",
           file_stat.st_size,
           time_str,
           basename(filename));

    // affiche un flèche pour les liens symboliques
    if (S_ISLNK(file_stat.st_mode) && !tagLUsed) {
        char target[1024];
        ssize_t len = readlink(filename, target, sizeof(target) - 1);
        if (len != -1) {
            target[len] = '\0';
            printf(" -> %s", target);
        }
    }

    printf("\n");
}

// gestion des flag
void flag_check(char **directories, char **flags) {
    reverse = includes(flags, arrlength(flags), "-r"); // Initialisation du flag -r
    int follow_symlinks = (flags != NULL) ? includes(flags, arrlength(flags), "-L") : 0;
    int recursive = (flags != NULL) ? includes(flags, arrlength(flags), "-R") : 0;

    for (int i = 0; i < arrlength(directories); i++) {
        DIR *directory = opendir(directories[i]);
        if (!directory) {
            perror("Erreur d'ouverture du répertoire");
            continue;
        }

        struct dirent *content;
        FileInfo files[1024];
        char *subdirs[1024];
        int file_count = 0, subdir_count = 0;

        blkcnt_t total_blocks = 0; // Initialisation du compteur de blocs

        // Première boucle : Stocker les fichiers et répertoires et calculer les blocs
        while ((content = readdir(directory)) != NULL) {
            struct stat file_stat;
            char filepath[1024];

            build_path(filepath, sizeof(filepath), directories[i], content->d_name);
            if (stat(filepath, &file_stat) == -1) continue;

            if (flags != NULL && !includes(flags, arrlength(flags), "-a") && content->d_name[0] == '.') {
                if (includes(flags, arrlength(flags), "-A") &&
                    (strcmp(content->d_name, ".") == 0 || strcmp(content->d_name, "..") == 0))
                    continue;
                if (!includes(flags, arrlength(flags), "-A")) continue;
            }

            files[file_count].name = strdup(content->d_name);
            files[file_count].mtime = file_stat.st_mtime;
            file_count++;

            total_blocks += file_stat.st_blocks; // Ajouter les blocs au total

            if (recursive && S_ISDIR(file_stat.st_mode) && 
                strcmp(content->d_name, ".") != 0 && strcmp(content->d_name, "..") != 0) {
                subdirs[subdir_count] = strdup(filepath);
                subdir_count++;
            }
        }
        closedir(directory);

        if (includes(flags, arrlength(flags), "-t")) {
            qsort(files, file_count, sizeof(FileInfo), compare_mtime);
        } else {
            qsort(files, file_count, sizeof(FileInfo), compare_alpha);
        }

        // Affichage du total des blocs AVANT l'affichage des fichiers
        if (includes(flags, arrlength(flags), "-l")) {
            printf("total %ld\n", total_blocks / 2); // Division par 2 car st_blocks est en 512B
        }

        // Affichage des fichiers
        for (int j = 0; j < file_count; j++) {
            struct stat file_stat;
            char filepath[1024];

            build_path(filepath, sizeof(filepath), directories[i], files[j].name);
            if (stat(filepath, &file_stat) == -1) continue;

            if (includes(flags, arrlength(flags), "-l")) {
                print_file_details(filepath, follow_symlinks);
            } else {
                printf("%s  ", files[j].name);
            }
        }
        printf("\n");

        // Explorer les sous-répertoires après affichage des fichiers
        for (int j = 0; j < subdir_count; j++) {
            printf("\n%s:\n", subdirs[j]);
            char *subdir[] = {subdirs[j], NULL};
            flag_check(subdir, flags);
            free(subdirs[j]);
        }
    }
}
