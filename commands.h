#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include "builtins.h"
#include "io_helpers.h"

ssize_t cat_func( char **tokens);
ssize_t wc_func(char **tokens);
ssize_t cd_func(char **tokens);
ssize_t ls_func(char ** tokens);
int identify_rec(char **tokens, int tokens_size, int *pos);
int identify_substring(char **tokens, int size, char *sub);
int identify_path(char **tokens, int size, int rec_pos, char *path, int has_rec);
int recursive_ls(char **tokens, char *sub, int d, int size, int has_sub, char *path);