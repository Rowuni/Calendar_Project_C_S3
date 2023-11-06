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

void insert_d_list(t_d_list *list, int value)
{
    // Determine the level of the new cell
    int level = rand() % (list->level_max + 1);

    // Insert the new cell into the list at the appropriate level
    for (int i = 0; i <= level; i++) {
        // Create a new cell for each level
        t_d_cell *new_cell = create_d_cell(value, list->level_max);

        // Check if the head array has enough elements
        if (i < list->level_max) {
            t_d_cell *current = list->head[i];
            // Check if the current cell exists
            if (current == NULL || current->value > value) {
                // Insert the new cell at the beginning of the list
                new_cell->next[i] = list->head[i];
                list->head[i] = new_cell;
            } else {
                // Insert the new cell in the middle or at the end of the list
                while (current->next[i] != NULL && current->next[i]->value < value) {
                    current = current->next[i];
                }
                // Insert the new cell after the current cell only if the next cell's value is greater than or equal to the new cell's value
                if (current->next[i] == NULL || current->next[i]->value >= value) {
                    new_cell->next[i] = current->next[i];
                    current->next[i] = new_cell;
                }
            }
        }
    }

    // Increase the size of the list
    list->size++;
}

void delete_d_list(t_d_list *list, int value)
{
    // Start from the second level (skip head_0)
    for (int i = list->level_max - 1; i > 0; i--) {
        t_d_cell *current = list->head[i];
        if (current != NULL && current->value == value) {
            // The cell to be deleted is at the beginning of the list
            list->head[i] = current->next[i];
        } else {
            // Search for the cell in the middle or at the end of the list
            while (current->next[i] != NULL && current->next[i]->value < value) {
                current = current->next[i];
            }
            if (current != NULL && current->next[i] != NULL && current->next[i]->value == value) {
                // The cell to be deleted is in the middle or at the end of the list
                t_d_cell *temp = current->next[i];
                current->next[i] = temp->next[i];
                // Free the memory allocated for the cell
                free(temp);
                // Decrease the size of the list
                list->size--;
            }
        }
    }
}

t_d_cell *search_d_list(t_d_list *list, int value)
{
    t_d_cell *current = NULL;

    // Start from the highest level of the list
    for (int i = list->level_max - 1; i >= 0; i--) {
        if (current == NULL) {
            // Start from the head of the list
            current = list->head[i];
        }

        // Move forward until a cell with a value greater than the target value is found
        while (current->next[i] != NULL && current->next[i]->value < value) {
            current = current->next[i];
        }

        // If the cell with the target value is found, return it
        if (current->next[i] != NULL && current->next[i]->value == value) {
            return current->next[i];
        }
    }

    // If the cell with the target value is not found, return NULL
    return NULL;
}

void print_d_list(t_d_list *list) {
    // Iterate over each level
    for (int i = 0; i < list->level_max; i++) {
        printf("[list head_%d @-]", i);
        t_d_cell *current = list->head[i];
        t_d_cell *prev = list->head[0];
        // Print all the cells in the current level
        while (prev != NULL) {
            // Print dashes for missing values at levels above 0
            if (i > 0 && (current == NULL || prev->value < current->value)) {
                printf("-----------");
            }
            // Print the cell value if it exists at the current level
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
    // Free all the cells in the lowest level
    t_d_cell *current = list->head[0];
    while (current != NULL) {
        t_d_cell *temp = current;
        current = current->next[0];
        free(temp);
    }
    // Free the memory allocated for the list
    free(list);
}
