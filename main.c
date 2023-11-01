//Fichier main du projet
//Auteurs : Clément Laatar / Nicolas Gouge / Victor Bucas
//Version : 0.1.0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.c"

int main()
{
    // Create a multi-level list with 5 levels
    t_d_list *list = create_d_list(5);

    // Insert some values with different cell levels
    // The level of each cell is determined randomly in the insert_d_list function
    insert_d_list(list, 18);
    insert_d_list(list, 25);
    insert_d_list(list, 31);
    insert_d_list(list, 32);
    insert_d_list(list, 56);
    insert_d_list(list, 59);
    insert_d_list(list, 91);

    // Print the list
    printf("Initial list:\n");
    print_d_list(list);

    // Search for a value
    int value = 30;
    t_d_cell *cell = search_d_list(list, value);
    if (cell != NULL) {
        printf("Found %d in the list.\n", value);
    } else {
        printf("%d is not in the list.\n", value);
    }

    // Delete a value
    delete_d_list(list, value);

    // Print the list again
    printf("List after deleting %d:\n", value);
    print_d_list(list);

    // Free the memory allocated for the list
    free_d_list(list);

    return 0;
}