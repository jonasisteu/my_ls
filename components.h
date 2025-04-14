#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <linux/limits.h>
#include <sys/types.h>

void sort(char **directories, char **flags, int argc, char *argv[]);

void flag_check(char **directories, char **flags);

int compare_mtime(const void *a, const void *b);

void print_file_details(char *filename, int tagLUsed);

void list_directory(const char *directory, int show_hidden, int long_format);

void print_directory_total(const char *directory) ;

int check(char *entries[], char *flags[]);

#endif