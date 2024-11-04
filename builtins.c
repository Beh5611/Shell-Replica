#include <string.h>

#include "builtins.h"
#include "io_helpers.h"
#include "variables.h"


// ====== Command execution =====

/* Return: index of builtin or -1 if cmd doesn't match a builtin
 */
bn_ptr check_builtin(const char *cmd) {
    ssize_t cmd_num = 0;
    while (cmd_num < BUILTINS_COUNT &&
           strncmp(BUILTINS[cmd_num], cmd, MAX_STR_LEN) != 0) {
        cmd_num += 1;
    }
    return BUILTINS_FN[cmd_num];
}


// ===== Builtins =====

/* Prereq: tokens is a NULL terminated sequence of strings.
 * Return 0 on success and -1 on error ... but there are no errors on echo. 
 */
ssize_t bn_echo(char **tokens) {
    ssize_t index = 1;
    char *del = "$";
    
    if (tokens[index] != NULL) {
        // TODO:
        // Implement the echo command
        char *equal = strchr(tokens[index], *del);
        if (equal != NULL){
            
            int var_len = strlen(tokens[index]) - 1;
            char var[var_len + 1];
            
            strncpy(var, &(tokens[index][1]), var_len);
            var[var_len] = '\0';
            if (find_var(front, var) != 0){
                display_message(tokens[index]);
            }
        }
        else{
            display_message(tokens[index]);
        }
        index += 1;
    }
    while (tokens[index] != NULL) {
        // TODO:
        // Implement the echo command
        char *equal = strchr(tokens[index], *del);
        if (equal != NULL){
            int var_len = strlen(tokens[index]) - 1;
            char var[var_len + 1];
            
            strncpy(var, &(tokens[index][1]), var_len);
            var[var_len] = '\0';
            display_message(" ");
            if (find_var(front, var) != 0){
                display_message(tokens[index]);
            }
            
            
        }
        else{
            display_message(" ");
            display_message(tokens[index]);
        }
        index += 1;
    }
    display_message("\n");

    return 0;
}
