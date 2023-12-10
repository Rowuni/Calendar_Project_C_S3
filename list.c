// Fichier contenant les fonctions basiques sur une liste à plusieurs niveaux

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

t_d_list *create_d_list(int level_max)
{
    t_d_list *list = malloc(sizeof(t_d_list));
    list->head = calloc(level_max, sizeof(t_d_cell *));
    list->level_max = level_max;
    list->size = 0;
    return list;
}

t_d_cell *create_d_cell(int value, int level_max)
{
    t_d_cell *cell = malloc(sizeof(t_d_cell));
    cell->value = value;
    cell->next = calloc(level_max, sizeof(t_d_cell *));
    return cell;
}

void insert_d_list(t_d_list *list, int value, int level)
{
    // Vérifie si le niveau donné est bien compris dans la liste
    if (level < 0 || level > list->level_max-1) {
        printf("\nNiveau invalide pour %d. Le niveau doit être compris entre 0 et %d, donc %d n'est pas valide.\nLa valeur %d n'a donc pas été placée.\n \n", value, list->level_max-1, level, value);
        return;
    }

    // Insère la cellule au bon endroit
    for (int i = 0; i <= level; i++) {
        // Crée une nouvelle cellule avec la valeur donnée et le nombre de niveaux de la cellule
        t_d_cell *new_cell = create_d_cell(value, list->level_max);

        // Vérifie si le tableau de pointeurs de la liste est vide
        if (i < list->level_max) {
            t_d_cell *current = list->head[i];
            // Vérifie si la liste est vide ou si la valeur de la première cellule est plus grande que la valeur à insérer
            if (current == NULL || current->value > value) {
                // Insère la nouvelle cellule au début de la liste
                new_cell->next[i] = list->head[i];
                list->head[i] = new_cell;
            } else {
                // Parcours la liste jusqu'à ce que la valeur de la cellule suivante soit plus grande que la valeur à insérer
                while (current->next[i] != NULL && current->next[i]->value < value) {
                    current = current->next[i];
                }
                // Insère la nouvelle cellule après la cellule actuelle uniquement si la valeur de la cellule suivante est supérieure ou égale à la valeur de la nouvelle cellule.
                if (current->next[i] == NULL || current->next[i]->value >= value) {
                    new_cell->next[i] = current->next[i];
                    current->next[i] = new_cell;
                }
            }
        }
    }

    // Augmente la taille de la liste
    list->size++;
}

void insert_head_d_list(t_d_list *list, int value, int level) {
    // Vérifie si le niveau donné est bien compris dans la liste
    if (level < 0 || level > list->level_max-1) {
        printf("\n Niveau invalide pour %d. Le niveau doit être compris entre 0 et %d.\n La valeur %d n'a donc pas été placée.\n \n", value, list->level_max-1, value);
        return;
    }

    // Crée une nouvelle cellule avec la valeur donnée et le nombre de niveaux de la cellule
    t_d_cell *new_cell = create_d_cell(value, list->level_max);

    // Insère la cellule au début de la liste
    for (int i = 0; i <= level; i++) {
        new_cell->next[i] = list->head[i];
        list->head[i] = new_cell;
    }

    list->size++;
}

t_d_cell *search_d_list(t_d_list *list, int value) {
    t_d_cell *current = NULL;

    // On parcourt les niveaux de la liste du plus élevé au plus bas
    for (int i = list->level_max - 1; i >= 0; i--) {
        // Si c'est la première itération, on initialise 'current' avec la tête de la liste à ce niveau
        if (current == NULL) {
            current = list->head[i];
        }
        // On parcourt la liste à ce niveau jusqu'à ce qu'on trouve une valeur supérieure ou égale à la valeur recherchée
        while (current != NULL && current->value < value) {
            current = current->next[i];
        }
        // Si on a trouvé la valeur, on retourne la cellule contenant cette valeur
        if (current != NULL && current->value == value) {
            return current;
        }
    }
    return NULL;
}

void delete_d_list(t_d_list *list, int value) {
    t_d_cell *to_delete = search_d_list(list, value);
    // Vérifie si la valeur à supprimer existe dans la liste
    if (to_delete == NULL) {
        printf("Valeur %d non trouvée dans la liste.\n", value);
        return;
    }

    // Supprime la cellule au bon endroit
    for (int i = 0; i < list->level_max; i++) {
        if (list->head[i] != NULL && list->head[i]->value == value) {
            list->head[i] = list->head[i]->next[i];
        } else {
            t_d_cell *current = list->head[i];
            while (current != NULL && current->next[i] != NULL) {
                if (current->next[i]->value == value) {
                    current->next[i] = current->next[i]->next[i];
                    break;
                }
                current = current->next[i];
            }
        }
    }

    // Libère la mémoire de la cellule supprimée
    free(to_delete);
}

void print_d_list(t_d_list *list) {
    for (int i = 0; i < list->level_max; i++) {
        printf("[list head_%d @-]", i);
        t_d_cell *current = list->head[i];
        t_d_cell *prev = list->head[0];
        // Affiche toutes les cellules du niveau actuel
        while (prev != NULL) {
            // Affiche des tirets pour les valeurs manquantes aux niveaux supérieurs à 0
            if (i > 0 && (current == NULL || prev->value != current->value)) {
                int length = snprintf(NULL, 0, "-->[ %d|@-]", prev->value); // Calcule la longueur de la chaîne de caractères à afficher
                for (int j = 0; j < length; j++) { // Affiche des tirets pour chaque caractère de la chaîne de caractères
                    printf("-");
                }
            }
            // Affiche la valeur de la cellule si elle existe au niveau actuel
            if (current != NULL && prev->value == current->value) {
                printf("-->[ %d|@-]", current->value);
                current = current->next[i];
            }
            prev = prev->next[0];
        }
        printf("-->NULL\n");
    }
}

void free_d_list(t_d_list *list)
{
    // Libère la mémoire de toutes les cellules de la liste
    t_d_cell *current = list->head[0];
    while (current != NULL) {
        t_d_cell *temp = current;
        current = current->next[0];
        free(temp);
    }
    // Libère la mémoire de la liste
    free(list);
}

void print_d_list_level(t_d_list *list, int level) {
    // Vérifie si le niveau donné est bien compris dans la liste
    if (level < 0 || level >= list->level_max) {
        printf("\nLe niveau %d est invalide. Le niveau doit être compris entre 0 et %d.\n", level, list->level_max-1);
        return;
    }

    printf("[list head_%d @-]", level);
    t_d_cell *current = list->head[level];
    t_d_cell *prev = list->head[0];
    // Affiche toutes les cellules du niveau actuel
    while (prev != NULL) {
        // Affiche des tirets pour les valeurs manquantes aux niveaux supérieurs à 0
        if (level > 0 && (current == NULL || prev->value != current->value)) {
            printf("-----------");
        }
        // Afffiche la valeur de la cellule si elle existe au niveau actuel
        if (current != NULL && prev->value == current->value) {
            printf("-->[ %d|@-]", current->value);
            current = current->next[level];
        }
        prev = prev->next[0];
    }

    // Affiche la fin de chaque niveau
    printf("-->NULL\n");
}
