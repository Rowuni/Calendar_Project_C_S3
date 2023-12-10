#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct s_d_cell
{
    int value; // Niveau de la cellule
    struct s_d_cell **next; // Tableau de pointeurs vers les cellules suivantes
} t_d_cell; 

typedef struct s_d_list
{
    t_d_cell **head; // Tableau de pointeurs vers les cellules de la liste
    int level_max; // Hauteur maximale de la liste
    int size; // Nombre de cellules dans la liste
} t_d_list; 


// Définitions des fonctions

t_d_list *create_d_list(int level_max);
t_d_cell *create_d_cell(int value, int level_max);
void insert_d_list(t_d_list *list, int value, int level);
t_d_cell *search_d_list(t_d_list *list, int value);
void delete_d_list(t_d_list *list, int value);
void print_d_list(t_d_list *list);
void free_d_list(t_d_list *list);
void insert_head_d_list(t_d_list *list, int value, int level);

#endif
