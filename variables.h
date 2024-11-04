#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#define EQ "="


struct node *front;

struct node {
    char * var_name;
    char * var_value;
    struct node * next;
};

struct node *set_myvar(char *token, struct node *front);

struct node *create_new_var(char* name, char* val);

void add_node(struct node *front, struct node *new_node);

void print_lst(struct node *front);

void free_mem(struct node *front);

int find_var(struct node *front, char *name);

int duplicate_checker(struct node *front, char *name, char *value);

int true_if_find(struct node *front, char *name);