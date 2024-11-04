#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include "builtins.h"
#include "io_helpers.h"


ssize_t cat_func(char **tokens){
    FILE *file;
    char line[251 + 1];
    if (tokens[1] == NULL){
        display_error("ERROR: No input source provided", "");
        return -1;
    }
    file = fopen(tokens[1], "r");
    if (file == NULL){
        display_error("ERROR: Cannot open file", "");
        return -1;
    }
    while(fgets(line, sizeof(line), file) != NULL){
        display_message(line);
    }
    display_message("\n");
    fclose(file);
    return 0;
}

ssize_t wc_func(char **tokens){
    FILE * file;
    char ch;
    int chars, words, lines;
    
    
    if (tokens[1] == NULL){
        display_error("ERROR: No input source provided", "");
        return -1;
    }
    file = fopen(tokens[1], "r");
    if (file == NULL){
        display_error("ERROR: Cannot open file", "");
        return -1;
    }
    chars = 0;
    words = 0;
    lines = 0;
    while ((ch = fgetc(file)) != EOF){
        if (ch == ' ' || ch == '\n' || ch == '\0' || ch == '\t'){
            words++;
        }
        if (ch == '\n' || ch == '\0'){
            lines++;
        }
        chars++;
    }
    if (chars > 0){
        words++;
        lines++;
    }

    printf("\n");
    printf("Total chars %d\n", chars);
    printf("word count %d\n", words);
    printf("newline count %d\n", lines);

    fclose(file);

    return 0;
}  


ssize_t cd_func(char **tokens){
    

    int err;
    int i = 0;
    char dots[200];
    int tokens_size = 0;
    while(tokens[tokens_size] != NULL){
        tokens_size ++;
    }
    if (tokens_size == 1){
        return 0;
    }
    strcpy(dots, tokens[1]);
    while (i < strlen(tokens[1])){
        if (dots[i]!= '.'){
            
            err = chdir(tokens[1]);
            if (err != 0){
                display_error("ERROR: Invalid path", "");
                return -1;
            }
            char q[100];
            display_message(getcwd(q, 100));
            display_message("\n");
            return 0;
        }
        i ++;
    }
    i = i - 1;
    int j = 0;
    while(j < i){
        err = chdir("../");
        if (err != 0){
            display_error("ERROR: Invalid path", "");
            
            return -1;
        }
        j++;
    }
    char q[100];
    display_message(getcwd(q, 100));
    display_message("\n");
    return 0;
}

ssize_t ls_func(char ** tokens){
    DIR *dp;
    struct dirent *dir;
    int tokens_size = 0;
    while(tokens[tokens_size] != NULL){
        tokens_size ++;
    }
    //If its just ls
    if (tokens_size == 1){
        //printf("Test1\n");
        dp = opendir("./");
        while ((dir = readdir(dp)) != NULL){
            //printf("%s\n", dir->d_name);
            display_message(dir->d_name);
            display_message("\n");
        }
        closedir(dp);
    }
    //If its only 2 arguements
    else if (tokens_size == 2){
        dp = opendir(tokens[1]);
        if (dp != NULL){
            while ((dir = readdir(dp)) != NULL){
                //printf("%s\n", dir->d_name);
                display_message(dir->d_name);
                display_message("\n");
            }
            closedir(dp);
        }
        else{
            display_error("ERROR: Invalid path", "");
            return -1;
        }
    }
    //Otherwise, identify all the arguements and then either recurse or just ls
    else{
        int pos[3]; //{--rec index, --d index, depth}
        char sub[200];
        char path[200];
        // printf("identify_rec result: %d\n", identify_rec(tokens, tokens_size, pos));
        // printf("position of rec stuff: %d, %d, %d\n", pos[0], pos[1], pos[2]);
        // printf("identify_substring result: %d\n", identify_substring(tokens, tokens_size, sub));
        // printf("Substring: %s\n", sub);
        // identify_path(tokens, tokens_size, pos[0], path);
        // printf("path: %s\n", path);
        int has_rec = identify_rec(tokens, tokens_size, pos);
        int has_sub = identify_substring(tokens, tokens_size, sub);
        identify_path(tokens, tokens_size, pos[0], path, has_rec);
        

        if (has_rec == 0){
            //dp = opendir(path);
            recursive_ls(tokens, sub, pos[2], tokens_size, has_sub, path);
            //closedir(dp);
        }
        else{
            // printf("identify_substring result: %d\n", has_sub);
            // printf("Substring: %s\n", sub);
            DIR *dp;
            struct dirent *dir;
            dp = opendir(path);
            if (dp != NULL){
                //char *p;
                while ((dir = readdir(dp)) != NULL){
                    // p = strstr(dir->d_name, sub);
                    //printf("%s\n", p);
                    if(has_sub == 0 ){
                        //printf("%s\n", dir->d_name);
                            if (strstr(dir->d_name, sub) != NULL){
                            display_message(dir->d_name);
                            display_message("\n");
                            }
                    }  
                    
                    else{
                        //printf("%s\n", dir->d_name);
                        display_message(dir->d_name);
                        display_message("\n");
                    }
                } 
            }
            closedir(dp);

        }
    }
    return 0;
}

int identify_rec(char ** tokens, int size, int *pos){
    //Identify whether --rec and --b are present.
    if (size < 4){
        return 1;
    }
    int i = 0;
    int flag = 0; //To keep track of whether both --d and --rec are present
    while (i < size){
        if (strcmp(tokens[i], "--d") == 0){
            pos[1] = i;
            if (i + 1 < size){
                pos[2] = i + 1;
                flag ++;
            }
        }
        else if(strcmp(tokens[i], "--rec") == 0){
            pos[0] = i;
            flag ++;
        }
        i ++;
    }
    if (flag >= 2){
        return 0;
    }
    return 1;
}

int identify_substring(char **tokens, int size, char *sub){
    //Identify whether substring exist
    if (size < 3){
        return 1;
    }
    int i = 0;
    while (i < size){
        if (strcmp(tokens[i], "--f") == 0){
            if(i + 1 < size){
                strcpy(sub, tokens[i+1]);
                return 0;
            }
        }
        i++;
    }
    return 1;
}

int identify_path(char **tokens, int size, int rec_pos, char *path, int has_rec){
    // Identify the path
    if (has_rec != 0){
        if (strcmp(tokens[1], "--f") != 0 && strcmp(tokens[1], "--rec") != 0 && strcmp(tokens[1], "--d") != 0){
            strcpy(path, tokens[1]);
            return 0;
        }
        else{
            strcpy(path, "./");
            return 0;
        }
    }
    if (strcmp(tokens[1], "--f") != 0 && strcmp(tokens[1], "--rec") != 0 && strcmp(tokens[1], "--d") != 0){
        strcpy(path, tokens[1]);
        return 0;
    }
    if (rec_pos + 1 < size){
        
        if (strcmp(tokens[rec_pos + 1], "--f") != 0 && strcmp(tokens[rec_pos + 1], "--rec") != 0 && strcmp(tokens[rec_pos + 1], "--d") != 0){
            strcpy(path, tokens[rec_pos + 1]);
            return 0;
        }
    }
    strcpy(path, "./");
    return 0;
}

int recursive_ls(char **tokens, char *sub, int d, int size, int has_sub, char *path){
    if (d == 1){
        DIR *dp;
        struct dirent *dir;
        dp = opendir(path);
        if (dp != NULL){
            while ((dir = readdir(dp)) != NULL){
                if(has_sub == 0){
                    if (strstr(dir->d_name, sub)){
                        display_message(dir->d_name);
                        display_message("\n");
                    }
                }
                else{
                    display_message(dir->d_name);
                    display_message("\n");
                    }
            }
            closedir(dp);
        }
    }
    else{
        DIR *dp;
        struct dirent *dir;
        dp = opendir(path);
        if (dp != NULL){
            while ((dir = readdir(dp)) != NULL){
                if(has_sub == 0){
                    //printf("%s\n", dir->d_name);
                    if (strstr(dir->d_name, sub)){
                        display_message(dir->d_name);
                        display_message("\n");
                    }
                    if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
                        char new_path[200] = { 0 };
                        strcat(new_path, path);
                        strcat(new_path, "/");
                        strcat(new_path, dir->d_name);
                        recursive_ls(tokens, sub, d - 1, size, has_sub, new_path);
                    }
                    
                }
                else{
                    //printf("%s\n", dir->d_name);
                    display_message(dir->d_name);
                    display_message("\n");
                    if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
                        char new_path[200] = { 0 };
                        strcat(new_path, path);
                        strcat(new_path, "/");
                        strcat(new_path, dir->d_name);
                        recursive_ls(tokens, sub, d - 1, size, has_sub, new_path);
                    }
                }
            }
        }
        closedir(dp);
    }
    return 0;
}