#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "variables.h"
#include "io_helpers.h"



struct node *set_myvar(char *token, struct node *front){
    
    
    char *f = EQ;
    const char *ptr = strchr(token, *f);
    int var_len = ptr - token;
    int token_len = strlen(token);
    
    char var[var_len + 2];
    char val[token_len - var_len + 2];

    strncpy(var,&token[0], var_len);
    strncpy(val,&token[var_len + 1], token_len - var_len);
    //strncpy(val,&token[var_len + 1], sizeof(char) * (token_len - var_len - 1));
    var[var_len] = '\0';
    val[token_len - var_len - 1] = '\0';
    
    if (front != NULL){
        if (duplicate_checker(front, var, val) == 0){
            return front;
        }
    }

    struct node *new = create_new_var(var, val);
    

    if (front == NULL){
        return new;
    }
    add_node(front, new);
    
    return front;

}



struct node *create_new_var(char* name, char* val){
    struct node *new_node = malloc(sizeof(struct node));
    
    new_node->var_name = malloc(sizeof(char)*70);
    new_node->var_value = malloc(sizeof(char)*70);
    new_node->next = NULL;
    strcpy(new_node->var_name, name);
    strcpy(new_node->var_value, val);
    
    return new_node;
}
int find_var(struct node *front, char *name){
    
    struct node * curr = front;
    
    while (curr != NULL){
        if (strcmp(curr->var_name, name) == 0){
            display_message(curr->var_value);
            return 0;
        }
        curr = curr->next;
    }
    return 1;
}
int true_if_find(struct node *front, char *name){
    
    struct node * curr = front;
    
    while (curr != NULL){
        if (strcmp(curr->var_name, name) == 0){
            return 0;
        }
        curr = curr->next;
    }
    return 1;
}

int duplicate_checker(struct node *front, char *name, char *value){
    struct node * curr = front;
    
    while (curr != NULL){
        if (strcmp(curr->var_name, name) == 0){
            
            strncpy(curr->var_value, value, strlen(value));
            curr->var_value[strlen(value)] = '\0';
            return 0;
        }
        curr = curr->next;
    }
    return 1;
}


void add_node(struct node *front, struct node *new_node){
    struct node * curr = front;
    while (curr->next != NULL){
        curr = curr->next;
    }
    curr->next = new_node;
}

void print_lst(struct node *front){
    struct node * curr = front;
    while (curr != NULL){
        printf("Loop Test: %s,%s\n", curr->var_name, curr->var_value);
        curr = curr->next;
    }
}
void free_mem(struct node *front){
    struct node * curr = front;
    struct node * next;
    while (curr != NULL){
        free(curr->var_name);
        free(curr->var_value);
        next = curr->next;
        free(curr);
        curr = next;
        
    }
}