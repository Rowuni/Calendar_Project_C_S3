#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct s_d_cell
{
    int value;
    struct s_d_cell **next; // array of next pointers
} t_d_cell; 

typedef struct s_d_list
{
    t_d_cell **head; // array of head pointers
    int level_max; // height of the list
    int size; // number of elements in the list
} t_d_list; 

t_d_list *create_d_list(int level_max);
t_d_cell *create_d_cell(int value, int level_max);
void insert_d_list(t_d_list *list, int value);
void delete_d_list(t_d_list *list, int value);
t_d_cell *search_d_list(t_d_list *list, int value);
void print_d_list(t_d_list *list);
void free_d_list(t_d_list *list);

#endif
