// Fichier contenant les tests de démonstration pour les parties 1 et 2 du projet.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.c"
#include "timer.c"
#include "search.c"

#define NUM_SEARCHES 10000

void tests_part_1_2()
{
    // Création d'une liste de 5 niveaux vide
    t_d_list *list = create_d_list(5);
    printf("\nListe vide créée : \n");
    print_d_list(list);

    // Insertion des valeurs avec des niveaux différents
    printf("\nInsertion de 18, 25, 31, 32, 56, 59 et 91 dans la liste\nLeurs niveaux respectifs sont 0, 4, 3, 2, 1, 0, 5\n \n");
    insert_d_list(list, 18, 0);
    insert_d_list(list, 25, 4);
    insert_d_list(list, 31, 3);
    insert_d_list(list, 32, 2);
    insert_d_list(list, 56, 1);
    insert_d_list(list, 59, 0);
    insert_d_list(list, 91, 5);


    // Affiche la liste
    printf("Liste remplie : \n");
    print_d_list(list);

    // Insertion d'une valeur au début de la liste
    insert_head_d_list(list, 65, 2);
    printf("\nListe après l'insertion de 65 au début : \n");
    print_d_list(list);

    // Supprimer une valeur
    int value = 31;
    delete_d_list(list, value);
    printf("\nListe après la suppression de %d : \n", value);
    print_d_list(list);

    // Afficher juste une ligne de la liste
    printf("\nListe avec seulement un niveau : \n");
    print_d_list_level(list, 0);



    // Libérer la mémoire
    free_d_list(list);

    // Test des fonctions de recherche

    FILE *log_file = fopen("C:\\Users\\cleme\\Documents\\Prepa\\L2\\S3\\Informatique\\Projet_SDD\\V_1.0\\log.txt","w");
    FILE *csv_file = fopen("C:\\Users\\cleme\\Documents\\Prepa\\L2\\S3\\Informatique\\Projet_SDD\\V_1.0\\result.csv","w");
    char format[] = "%d\t%s\t%s\n" ;
    int level;
    char *time_lvl0;
    char *time_all_levels;

    for (int list_level = 7; list_level <= 20; list_level++) {

        //Création de la liste
        t_d_list *list2 = create_d_list(list_level);
        int original_size = pow(2, list_level - 7);  // Définition de la taille originale de la liste

        for (int i = 0; i < original_size; i++) {
            int value = rand() % (2 * original_size + 1);
            int level = rand() % (list2->level_max);
            insert_d_list(list2, value, level);
        }

        printf("\nTest du niveau %d\n", list_level);

        // Mesure du temps de recherche pour la recherche simple
        startTimer();
        for (int i = 0; i < NUM_SEARCHES; i++) {
            int value = rand() % (list2->size + 1);
            search_d_list_simple(list2, value);
        }
        stopTimer();

        time_lvl0 = getTimeAsString(); // Enregistrement du temps de recherche simple
        printf("Temps écoulé pour la recherche simple : ");
        printf("%s\n", time_lvl0);

        // Mesure du temps de recherche pour la recherche à plusieurs niveaux
        startTimer();
        for (int i = 0; i < NUM_SEARCHES; i++) {
            int value = rand() % (list2->size + 1);
            search_d_list_levels(list2, value);
        }
        stopTimer();

        // Enregistrement du temps de recherche à plusieurs niveaux
        time_all_levels = getTimeAsString(); 
        printf("Temps écoulé pour la recherche à plusieurs niveaux : ");
        printf("%s\n", time_all_levels);

        // Écriture dans le fichier log.txt
        fprintf(log_file,format,list_level,time_lvl0, time_all_levels); 
        fprintf(csv_file,"%d;%s;%s\n",list_level,time_lvl0, time_all_levels);
        fflush(log_file); // On vide le buffer
        fflush(csv_file);

        // Double la taille de la liste
        original_size = list2->size;

        if (list_level == 20) {
            break;
        }

        // On ajoute des valeurs à la liste aléatoirement
        for (int i = 0; i < original_size; i++) {
            int value = rand() % (2 * original_size + 1);
            int level = rand() % (list2->level_max);
            insert_d_list(list2, value, level);
        }
    }
    fclose(log_file);
    fclose(csv_file);
}
