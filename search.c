// Fichier contenant les fonctions de recherche sur une liste à plusieurs niveaux

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timer.h"
#include "list.h"
#include "search.h"

#define NUM_SEARCHES 10000
#define LENGTH_BASE 7

// Fonction pour rechercher une valeur dans une liste à plusieurs niveaux en utilisant une recherche simple (niveau 0 uniquement)
void search_d_list_simple(t_d_list *list, int value) {
    // On commence par la tête de la liste au niveau 0
    t_d_cell *current = list->head[0];

    // On parcourt la liste jusqu'à ce qu'on trouve une valeur supérieure ou égale à la valeur recherchée
    while (current != NULL) {
        if (current->value >= value) {
            return;  // On a trouvé la valeur, on sort de la fonction
        }
        // On passe à l'élément suivant au niveau 0
        current = current->next[0];
    }
}

// Fonction pour rechercher une valeur dans une liste à plusieurs niveaux en utilisant une recherche multi-niveaux
void search_d_list_levels(t_d_list *list, int value) {
    // On commence par la tête de la liste au niveau le plus élevé
    t_d_cell *current = NULL;

    // On parcourt les niveaux de la liste du plus élevé au plus bas
    for (int i = list->level_max - 1; i >= 0; i--) {
        if (current == NULL) {
            current = list->head[i];
        }
        // On parcourt la liste à ce niveau jusqu'à ce qu'on trouve une valeur supérieure ou égale à la valeur recherchée
        while (current != NULL && current->value < value) {
            // On passe à l'élément suivant à ce niveau
            current = current->next[i];
        }
        // Si on a trouvé la valeur, on sort de la fonction
        if (current != NULL && current->value == value) {
            return;
        }
    }
}
