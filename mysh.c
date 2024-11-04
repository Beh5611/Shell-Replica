#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "builtins.h"
#include "io_helpers.h"
#include "variables.h"
#include "commands.h"




int main(int argc, char* argv[]) {
    char *prompt = "mysh$ "; // TODO Step 1, Uncomment this.

    char input_buf[MAX_STR_LEN + 1];
    input_buf[MAX_STR_LEN] = '\0';
    char *token_arr[MAX_STR_LEN] = {NULL};

    
    front = NULL;

    while (1) {
        // Prompt and input tokenization

        // TODO Step 2:
        // Display the prompt via the display_message function.

        display_message(prompt);

        int ret = get_input(input_buf);
        size_t token_count = tokenize_input(input_buf, token_arr);

        // Clean exit
        if (*input_buf != 0x0A && ret != -1 && (token_count == 0 || (strncmp("exit", token_arr[0], 4) == 0))) {
            break;
        }

        // Command execution
        if (token_count >= 1) {
            //echo hello world|wc|wc|wc
            /*
            if pipe:
                command = [echo, hello, world]
                pipe = [wc, wc, wc]
                =>
                run [echo, hello, world] => wc (aka pipe[0]) => wc (aka pipe[1])


                run [echo, hello, world] => wc (aka pipe[0])
                while pipe:
                    int fd[2]
                    pipe(fd)
                    f = fork()
                    if (f>0){
                        close(pipefd[0]); 
                        dup2(pipefd[1], STDOUT_FILENO);
                        bn_ptr builtin_fn = check_builtin(command[0]);
                        char *del = "=";
                        char *equal = strchr(command[0], *del);
                        if (token_count == 1 && equal != NULL){
                        front = set_myvar(command[0], front);
                        }
                        else if (builtin_fn != NULL) {
                            ssize_t err = builtin_fn(command);
                            if (err == - 1) {
                                display_error("ERROR: Builtin failed: ", command[0]);
                            }
                        } else {
                            display_error("ERROR: Unrecognized command: ", command[0]);
                        }
                    }else if (f == 0){
                        close(pipefd[1]);  
                        dup2(pipefd[0], STDIN_FILENO);
                        x = pipe.pop(0)
                        lst = tokenize(x)
                        bn_ptr builtin_fn = check_builtin(lst[0]);
                    }

                    
            */
            bn_ptr builtin_fn = check_builtin(token_arr[0]);
            char *del = "=";
            char *equal = strchr(token_arr[0], *del);
            if (token_count == 1 && equal != NULL){
                
            front = set_myvar(token_arr[0], front);
                
            }
            else if (builtin_fn != NULL) {
                ssize_t err = builtin_fn(token_arr);
                if (err == - 1) {
                    display_error("ERROR: Builtin failed: ", token_arr[0]);
                }
            } else {
                display_error("ERROR: Unrecognized command: ", token_arr[0]);
            }
        }

    }
    free_mem(front);
    return 0;
}
