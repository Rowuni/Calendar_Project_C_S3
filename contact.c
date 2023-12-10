// Fichier comportant toutes les fonctions relatives à la gestion de l'agenda et des contacts

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "contact.h"

int number_of_rdv = 0;

// Convertit tous les caractères en minuscules
void toLowerCase(char *str) {

    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

// Lit une valeur entière dans la plage donnée
int readIntInRange(int min, int max, const char *prompt) {
    int value;
    char input[50];
    int result;

    // Boucle jusqu'à ce que l'utilisateur entre une valeur valide
    do {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);

        // Tente de convertir l'entrée de l'utilisateur en un entier
        result = sscanf(input, "%d", &value);

        // Si la conversion a échoué ou si la valeur est hors de la plage, affiche un message d'erreur
        if (result < 1 || value < min || value > max) {
            printf("\033[31mVeuillez entrer un nombre entre %d et %d.\033[0m\n", min, max);
        }
    // Continue la boucle tant que l'entrée est invalide
    } while (result < 1 || value < min || value > max);

    // Retourne la valeur entrée par l'utilisateur
    return value;
}

// Lit une chaîne de caractères de la console
char *scanString() {
    char *str = NULL;
    int c;
    size_t size = 0;

    // Ignorer les caractères de nouvelle ligne au début de l'entrée
    while ((c = getchar()) == '\n');

    // Lire les caractères jusqu'à la fin de la ligne
    do {
        // Réallouer de la mémoire pour la chaîne, en ajoutant de l'espace pour le nouveau caractère et le caractère de fin de chaîne '\0'
        str = realloc(str, size + 2);
        // Ajouter le nouveau caractère à la chaîne
        str[size++] = c;
    } while ((c = getchar()) != '\n' && c != EOF); // Continuer à lire jusqu'à la fin de la ligne ou la fin du fichier

    // Ajouter le caractère de fin de chaîne à la fin de la chaîne
    str[size] = '\0';

    // Retourner la chaîne lue
    return str;
}

// Crée un nouvel agenda vide
Agenda *create_agenda() {
    Agenda *agenda = malloc(sizeof(Agenda));
    agenda->contacts = NULL;
    return agenda;
}

// Crée un nouveau contact avec le nom donné
Contact *create_contact(char *nom_prenom) {
    Contact *contact = malloc(sizeof(Contact));
    contact->nom_prenom = strdup(nom_prenom);  // Duplique la chaîne de caractères
    contact->rendezvous = NULL;
    return contact;
}

// Insère un contact dans la liste de contacts de l'agenda en ordre lexicographique
void insert_contact(Agenda *agenda, Contact *contact, t_d_list_contact *list_contact) {
    // Pointeur vers le premier contact de l'agenda
    ContactNode **current = &(agenda->contacts);

    // Parcourir la liste des contacts
    while (*current != NULL) {
        // Dupliquer les noms pour éviter de modifier les originaux
        char *name1_copy = strdup((*current)->contact->nom_prenom);
        char *name2_copy = strdup(contact->nom_prenom);

        // Séparer les noms et prénoms
        char *name1_first = strtok(name1_copy, "_");
        char *name1_second = strtok(NULL, "_");
        char *name2_first = strtok(name2_copy, "_");
        char *name2_second = strtok(NULL, "_");

        // Comparer les noms d'abord, puis les prénoms si les noms sont identiques
        int comparison = strcmp(name1_first, name2_first);
        if (comparison == 0) {
            comparison = strcmp(name1_second, name2_second);
        }

        // Libérer la mémoire des copies
        free(name1_copy);
        free(name2_copy);

        // Si le nouveau contact doit être inséré à cette position, sortir de la boucle
        if (comparison >= 0) {
            break;
        }

        // Sinon, passer au contact suivant
        current = &((*current)->next);
    }

    // Créer un nouveau noeud pour le contact
    ContactNode *new_node = malloc(sizeof(ContactNode));
    new_node->contact = contact;
    new_node->next = *current;

    // Insérer le nouveau noeud à la position trouvée
    *current = new_node;
}

// Crée un nouveau contact avec le nom donné et l'insère dans l'agenda
void create_and_insert_contact(Agenda *agenda, char *nom_prenom, t_d_list_contact *list_contact) {
    // Convertit le nom en minuscules
    toLowerCase(nom_prenom);

    // Crée un nouveau contact et l'insère dans l'agenda
    Contact *contact = create_contact(nom_prenom);
    insert_contact(agenda, contact, list_contact);
}

// Affiche tous les rendez-vous pour un contact
void print_contact_rendezvous(Contact *contact) {
    printf("\nRendez-vous pour : %s\n", contact->nom_prenom);
    if (contact->rendezvous == NULL) {
        printf("Aucun rendez-vous de prévu\n");
    } else {
        // Initialise un pointeur vers le premier rendez-vous du contact
        RendezVousNode *current = contact->rendezvous;

        // Parcourt la liste des rendez-vous du contact
        while (current != NULL) {
            // Récupère le rendez-vous actuel
            RendezVous *rendezvous = current->rendezvous;

            // Affiche les détails du rendez-vous
            printf("- Le %02d/%02d/%04d à %02d:%02d pendant %02dh%02d.\n",
                   rendezvous->jour, rendezvous->mois, rendezvous->annee,
                   rendezvous->heure, rendezvous->minute,
                   rendezvous->duree_heure, rendezvous->duree_minute);
            printf("Objet : %s\n", rendezvous->objet);

            current = current->next;
        }
    }
}

// Recherche un contact dans la liste
Contact *find_contact(Agenda *agenda, char *nom_prenom) {
    // Initialise un pointeur vers le premier contact de l'agenda
    ContactNode *current = agenda->contacts;

    // Parcourt la liste des contacts
    while (current != NULL) {
        // Si le nom du contact actuel correspond au nom recherché, retourne le contact
        if (strcmp(current->contact->nom_prenom, nom_prenom) == 0) {
            return current->contact;
        }

        // Passe au contact suivant
        current = current->next;
    }

    return NULL;
}

// Recherche un contact dans la liste et propose une complétion automatique
Contact *find_contact_auto(Agenda *agenda, char *nom_prenom) {
    char response[4];

    // Vérifie si le nom_prenom entré a au moins 3 caractères
    if (strlen(nom_prenom) >= 3) {
        ContactNode *current = agenda->contacts;

        // Parcourt la liste des contacts
        while (current != NULL) {
            // Compare les premiers caractères du nom_prenom du contact actuel avec le nom_prenom entré
            if (strncmp(current->contact->nom_prenom, nom_prenom, strlen(nom_prenom)) == 0) {
                printf("\nVoulez-vous dire %s ? (oui/non) ", current->contact->nom_prenom);
                scanf("%3s", response);
                getchar(); // Pour consommer le '\n' laissé par scanf
                toLowerCase(response);

                // Si l'utilisateur confirme que le contact actuel est le bon, retourne le contact
                if (strcmp(response, "oui") == 0) {
                    return current->contact;
                }
            }

            // Passe au contact suivant
            current = current->next;
        }
    }

    // Si aucun contact correspondant n'a été trouvé, retourne NULL
    return NULL;
}

// Crée un nouveau rendez-vous
RendezVous *create_rendezvous(int jour, int mois, int annee, int heure, int minute, int duree_heure, int duree_minute, char *objet) {
    RendezVous *rendezvous = malloc(sizeof(RendezVous));
    rendezvous->jour = jour;
    rendezvous->mois = mois;
    rendezvous->annee = annee;
    rendezvous->heure = heure;
    rendezvous->minute = minute;
    rendezvous->duree_heure = duree_heure;
    rendezvous->duree_minute = duree_minute;
    rendezvous->objet = strdup(objet);  // Duplique la chaîne de caractères
    number_of_rdv++;
    return rendezvous;
}

// Ajoute un rendez-vous à un contact
void add_rendezvous_to_contact(Contact *contact, RendezVous *rendezvous) {
    // Crée un nouveau noeud pour le rendez-vous
    RendezVousNode *node = malloc(sizeof(RendezVousNode));
    node->rendezvous = rendezvous;

    // Vérifie si le noeud et le contact ne sont pas NULL
    if (node != NULL && contact != NULL) {
        // Ajoute le nouveau noeud au début de la liste des rendez-vous du contact
        node->next = contact->rendezvous;
    } else {
        // Affiche un message d'erreur si le noeud ou le contact est NULL
        printf("Erreur : node ou contact est NULL\n");
    }

    // Met à jour le pointeur vers le premier rendez-vous du contact
    contact->rendezvous = node;
}

// Recherche d'un rendez-vous dans l'agenda
RendezVous *find_rendezvous(Agenda *agenda, int jour, int mois, int annee, int heure, int minute, int duree_heure, int duree_minute, char *objet) {
    // Initialise un pointeur vers le premier contact de l'agenda
    ContactNode *current_contact_node = agenda->contacts;

    // Parcourt la liste des contacts
    while (current_contact_node != NULL) {
        // Initialise un pointeur vers le premier rendez-vous du contact actuel
        RendezVousNode *current_rendezvous_node = current_contact_node->contact->rendezvous;

        // Parcourt la liste des rendez-vous du contact actuel
        while (current_rendezvous_node != NULL) {
            // Récupère le rendez-vous actuel
            RendezVous *current_rendezvous = current_rendezvous_node->rendezvous;

            // Vérifie si le rendez-vous actuel correspond aux critères de recherche
            if (current_rendezvous->jour == jour && current_rendezvous->mois == mois && current_rendezvous->annee == annee &&
                current_rendezvous->heure == heure && current_rendezvous->minute == minute && current_rendezvous->duree_heure == duree_heure &&
                current_rendezvous->duree_minute == duree_minute && strcmp(current_rendezvous->objet, objet) == 0) {
                // Si le rendez-vous correspond, retourne le rendez-vous
                return current_rendezvous;
            }
            current_rendezvous_node = current_rendezvous_node->next;
        }
        current_contact_node = current_contact_node->next;
    }

    // Si aucun rendez-vous correspondant n'a été trouvé, retourne NULL
    return NULL;
}

// Supprime un rendez-vous d'un contact
void supprimer_rendezvous_de_tous_les_contacts(Agenda *agenda, RendezVous *rendezvous) {
    // Initialise un pointeur vers le premier contact de l'agenda
    ContactNode *current_contact_node = agenda->contacts;

    // Parcourt la liste des contacts
    while (current_contact_node != NULL) {
        // Récupère le contact actuel
        Contact *current_contact = current_contact_node->contact;

        // Initialise un pointeur vers le premier rendez-vous du contact actuel
        RendezVousNode *current_rendezvous_node = current_contact->rendezvous;
        // Initialise un pointeur vers le rendez-vous précédent (NULL au début)
        RendezVousNode *previous_rendezvous_node = NULL;

        // Parcourt la liste des rendez-vous du contact actuel
        while (current_rendezvous_node != NULL) {
            // Si le rendez-vous actuel est le rendez-vous à supprimer
            if (current_rendezvous_node->rendezvous == rendezvous) {
                // Si le rendez-vous à supprimer est le premier de la liste
                if (previous_rendezvous_node == NULL) {
                    // Le premier rendez-vous devient le suivant
                    current_contact->rendezvous = current_rendezvous_node->next;
                } else {
                    // Le rendez-vous suivant du rendez-vous précédent devient le rendez-vous suivant du rendez-vous à supprimer
                    previous_rendezvous_node->next = current_rendezvous_node->next;
                }

                // Libère la mémoire du rendez-vous
                free(current_rendezvous_node->rendezvous);
                // Libère la mémoire du nœud du rendez-vous
                free(current_rendezvous_node);

                // Sort de la boucle
                break;
            }

            // Passe au rendez-vous suivant
            previous_rendezvous_node = current_rendezvous_node;
            current_rendezvous_node = current_rendezvous_node->next;
        }

        // Passe au contact suivant
        current_contact_node = current_contact_node->next;
    }

    // Décrémente le nombre total de rendez-vous
    number_of_rdv--;
}

// Sauvegarde tous les rendez-vous dans un fichier
void save_all_rendezvous_to_file(Agenda *agenda) {
    // Ouvre le fichier en mode écriture
    FILE *file = fopen("C:\\Users\\cleme\\Documents\\Prepa\\L2\\S3\\Informatique\\Projet_SDD\\V_1.0\\rendezvous.txt","w");
    
    // Vérifie si le fichier a été correctement ouvert
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    // Initialise un pointeur vers le premier contact de l'agenda
    ContactNode *current_contact = agenda->contacts;
    
    // Parcourt la liste des contacts
    while (current_contact != NULL) {
        // Écrit le nom du contact dans le fichier
        fprintf(file, "\nRendez-vous pour : %s\n", current_contact->contact->nom_prenom);
        
        // Initialise un pointeur vers le premier rendez-vous du contact actuel
        RendezVousNode *current_rendezvous = current_contact->contact->rendezvous;
        
        // Vérifie si le contact a des rendez-vous
        if (current_rendezvous == NULL) {
            // Écrit dans le fichier que le contact n'a pas de rendez-vous
            fprintf(file, "Aucun rendez-vous de prévu\n");
        } else {
            // Parcourt la liste des rendez-vous du contact
            while (current_rendezvous != NULL) {
                // Récupère le rendez-vous actuel
                RendezVous *rendezvous = current_rendezvous->rendezvous;
                
                // Écrit les détails du rendez-vous dans le fichier
                fprintf(file, "- Le %02d/%02d/%04d à %02d:%02d pendant %02dh%02d.\n",
                        rendezvous->jour, rendezvous->mois, rendezvous->annee,
                        rendezvous->heure, rendezvous->minute,
                        rendezvous->duree_heure, rendezvous->duree_minute);
                fprintf(file, "Objet : %s\n", rendezvous->objet);
                
                // Passe au rendez-vous suivant
                current_rendezvous = current_rendezvous->next;
            }
        }
        current_contact = current_contact->next;
    }
    fclose(file);
}

// Charge les contacts à partir du fichier de noms et de prénoms de l'INSEE et les ajoute à l'agenda
void load_contacts_from_file(Agenda *agenda, const char *filename, const char *prenoms_filename, t_d_list_contact *list_contact) {

    FILE *file = fopen(filename, "r");
    FILE *prenoms_file = fopen(prenoms_filename, "r");
    if (file == NULL || prenoms_file == NULL) {
        printf("Impossible d'ouvrir le fichier %s ou %s\n", filename, prenoms_filename);
        return;
    }

    // Stockez tous les prénoms et noms dans des listes
    char prenoms[25000][32]; // tableau de 25000 prénoms de 32 caractères maximum
    char noms[25000][32]; // Tableau de 25000 noms de 32 caractères maximum
    int prenoms_count = 0; // Nombre de prénoms lus
    int noms_count = 0; // Nombre de noms lus

    // Lecture des prénoms et des noms
    while (prenoms_count < 5000 && fgets(prenoms[prenoms_count], sizeof(prenoms[prenoms_count]), prenoms_file)) {
        prenoms[prenoms_count][strcspn(prenoms[prenoms_count], "\n")] = 0;
        prenoms_count++;
    }
    while (noms_count < 5000 && fgets(noms[noms_count], sizeof(noms[noms_count]), file)) {
        noms[noms_count][strcspn(noms[noms_count], "\n")] = 0;
        noms_count++;
    }

    int contacts_count = 0;
    //Afficher la barre de progression
    printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ 0/5000 contacts créés");
    fflush(stdout); // Force l'affichage immédiat

    while (contacts_count < 5000) {
        // Choisissez un prénom et un nom au hasard dans les listes
        int random_prenom_index = rand() % prenoms_count;
        int random_nom_index = rand() % noms_count;
        char nom_prenom[64];
        sprintf(nom_prenom, "%s_%s", noms[random_nom_index], prenoms[random_prenom_index]);
        toLowerCase(nom_prenom);
        // Crée un nouveau contact avec ce nom_prenom
        Contact *contact = create_contact(nom_prenom);
        add_contact_to_list_from_top(list_contact, contact);
        

        // Ajoute le contact à la liste des contacts
        insert_contact(agenda, contact, list_contact);
        contacts_count++;

        // Mise à jour de la barre de progression tous les 8 contacts créés
        if (contacts_count % 8 == 0) {
            int progress = (contacts_count * 50) / 5000; // Pourcentage de progression
            printf("\r"); // Retour au début de la ligne
            for (int i = 0; i < 50; i++) {
                if (i < progress) {
                    printf("█"); // Affiche la partie remplie de la barre de progression
                } else {
                    printf("▒"); // Affiche la partie vide de la barre de progression
                }
            }
            printf(" %d/5000 contacts créés", contacts_count); // Affiche le nombre de contacts créés
            fflush(stdout); // Force l'affichage immédiat

        }

    }
    save_contacts_to_file(list_contact);
    fclose(file);
    fclose(prenoms_file);
}

// Charge les contacts et les rendez-vous à partir du fichier de rendez-vous que nous pouvons enregistrer avec la fonction save_all_rendezvous_to_file
void load_contacts_and_rendezvous_from_file(Agenda *agenda, t_d_list_contact *list_contact) {

    FILE *file = fopen("C:\\Users\\cleme\\Documents\\Prepa\\L2\\S3\\Informatique\\Projet_SDD\\V_1.0\\rendezvous.txt", "r");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    // Compter le nombre total de lignes pour la barre de progression
    int total_lines = 0;
    char ch;
    while(!feof(file))
    {
        ch = fgetc(file);
        if(ch == '\n')
        {
            total_lines++;
        }
    }
    rewind(file);  // Remettre le pointeur de fichier au début

    // Afficher la barre de progression
    printf("Chargement en cours...\n");
    printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ 0/%d lignes chargées", total_lines);
    fflush(stdout);  // Force l'affichage immédiat

    char line[256];
    fgets(line, sizeof(line), file);  // Ignorer la première ligne vide
    Contact *current_contact_load = NULL;
    int lines_loaded = 0;
    while (fgets(line, sizeof(line), file)) {
        lines_loaded++;
        if (lines_loaded % 24 == 0) {  // Mise à jour tous les 24 lignes
            int progress = (lines_loaded * 50) / total_lines;  // Pourcentage de progression
            printf("\r");  // Retour au début de la ligne
            for (int i = 0; i < 50; i++) {
                if (i < progress) {
                    printf("█");  // Affiche la partie remplie de la barre de progression
                } else {
                    printf("▒");  // Affiche la partie vide de la barre de progression
                }
            }
            printf(" %d/%d lignes chargées", lines_loaded, total_lines);  // Affiche le nombre de lignes chargées
            fflush(stdout);  // Force l'affichage immédiat
        }

        if (strncmp(line, "Rendez-vous pour :", 18) == 0) {
            char *nom_prenom_load = line + 19;
            nom_prenom_load[strcspn(nom_prenom_load, "\n")] = 0;  // Enlève le saut de ligne à la fin
            current_contact_load = create_contact(nom_prenom_load);
            insert_contact(agenda, current_contact_load, list_contact);
        } else if (strncmp(line, "- Le", 4) == 0) {
            char jour_load_str[3], mois_load_str[3], annee_load_str[5], heure_load_str[3], minute_load_str[3], duree_heure_load_str[3], duree_minute_load_str[3];            
            int jour_load = atoi(jour_load_str);
            int mois_load = atoi(mois_load_str);
            int annee_load = atoi(annee_load_str);
            int heure_load = atoi(heure_load_str);
            int minute_load = atoi(minute_load_str);
            int duree_heure_load = atoi(duree_heure_load_str);
            int duree_minute_load = atoi(duree_minute_load_str);
            fgets(line, sizeof(line), file);  // Lire la ligne suivante
            char objet_load[256];
            if (strncmp(line, "Objet : ", 8) == 0) {
                strcpy(objet_load, line + 8);
                objet_load[strcspn(objet_load, "\n")] = 0;  // Enlève le saut de ligne à la fin
            }
            RendezVous *rendezvous_load = create_rendezvous(jour_load, mois_load, annee_load, heure_load, minute_load, duree_heure_load, duree_minute_load, objet_load);
            add_rendezvous_to_contact(current_contact_load, rendezvous_load);
        }
    }
    // Afficher la barre de progression complète à la fin
    printf("\r");  // Retour au début de la ligne
    for (int i = 0; i < 50; i++) {
        printf("█");  // Affiche la partie remplie de la barre de progression
    }
    printf(" %d/%d lignes chargées", total_lines, total_lines);  // Affiche le nombre de lignes chargées
    fflush(stdout);  // Force l'affichage immédiat
    printf("\nChargement terminé.\n");

    fclose(file);
}

// Crée la liste de contacts pour les chaînages
t_d_list_contact *create_list_contact() {
    // Alloue de la mémoire pour la liste de contacts
    t_d_list_contact *list = malloc(sizeof(t_d_list_contact));
    if (list == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    int level_max = 4;  // Niveau maximum toujours à 4
    // Alloue de la mémoire pour le tableau de têtes de liste
    list->head = malloc(level_max * sizeof(t_d_cell_contact *));
    if (list->head == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        free(list);  // Libère la mémoire allouée pour la liste en cas d'erreur
        return NULL;
    }

    // Initialise toutes les têtes de liste à NULL
    for (int i = 0; i < level_max; i++) {
        list->head[i] = NULL;
    }

    // Initialise le niveau maximum et la taille de la liste
    list->level_max = level_max;
    list->size = 0;

    // Retourne la liste créée
    return list;
}

// Crée un nouveau contact et l'insère avec la méthode d'insertion simple
void add_contact_to_list_from_top(t_d_list_contact *list, Contact *contact) {
    // Créer une nouvelle cellule pour le contact
    t_d_cell_contact *new_cell = malloc(sizeof(t_d_cell_contact));
    if (new_cell == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return;
    }
    new_cell->contact = contact;
    new_cell->next = malloc(list->level_max * sizeof(t_d_cell_contact *));
    if (new_cell->next == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        free(new_cell);
        return;
    }

    // Initialiser les pointeurs suivants à NULL
    for (int i = 0; i < list->level_max; i++) {
        new_cell->next[i] = NULL;
    }

    // Parcourir le niveau 0 de la liste pour trouver la bonne place pour le nouveau contact
    t_d_cell_contact *current = list->head[0];
    t_d_cell_contact *previous = NULL;
    while (current != NULL && strcmp(current->contact->nom_prenom, contact->nom_prenom) < 0) {
        previous = current;
        current = current->next[0];
    }

    // Insérer la nouvelle cellule dans la liste au niveau 0
    new_cell->next[0] = current;
    if (previous == NULL) {
        list->head[0] = new_cell;
    } else {
        previous->next[0] = new_cell;
    }

    // Déterminer le niveau de chaînage
    int chain_level = list->level_max - 1;
    if (previous != NULL) {
        chain_level = determine_chain_level(previous->contact->nom_prenom, contact->nom_prenom);
    }

    // Effectuer le chaînage correspondant aux autres niveaux
    chain_contact(list, new_cell, previous, current, chain_level);

    list->size++;
}

// Crée un nouveau contact et l'insère avec la méthode d'insertion à plusieurs niveaux
void add_contact_to_list_from_bottom(t_d_list_contact *list, Contact *contact) {
    // Créer une nouvelle cellule pour le contact
    t_d_cell_contact *new_cell = malloc(sizeof(t_d_cell_contact));
    if (new_cell == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return;
    }
    new_cell->contact = contact;
    new_cell->next = malloc(list->level_max * sizeof(t_d_cell_contact *));
    if (new_cell->next == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        free(new_cell);
        return;
    }

    // Initialiser les pointeurs suivants à NULL
    for (int i = 0; i < list->level_max; i++) {
        new_cell->next[i] = NULL;
    }

    // Parcourir la liste à partir du niveau le plus bas
    t_d_cell_contact *current = list->head[list->level_max - 1];
    t_d_cell_contact *previous = NULL;
    for (int i = list->level_max - 1; i >= 0; i--) {
        current = list->head[i];
        while (current != NULL && strcmp(current->contact->nom_prenom, contact->nom_prenom) < 0) {
            previous = current;
            current = current->next[i];
        }
        // Insérer la nouvelle cellule dans la liste au niveau actuel si les conditions sont remplies
    int chain_level = list->level_max - 1;
    if (previous != NULL) {
        chain_level = determine_chain_level(previous->contact->nom_prenom, contact->nom_prenom);
        }
    if ((i == 0) || (i <= chain_level)) {
        new_cell->next[i] = current;
        if (previous == NULL) {
            list->head[i] = new_cell;
        } else {
            previous->next[i] = new_cell;
        }
    }

        // Descendre d'un niveau en repartant du contact précédent
        if (previous != NULL) {
            current = previous;
        }
    }

    list->size++;
}

// Fonction pour visualiser l'état de la liste pour le débogage
void save_contacts_to_file(t_d_list_contact *list) {
    FILE *file = fopen("C:\\Users\\cleme\\Documents\\Prepa\\L2\\S3\\Informatique\\Projet_SDD\\V_1.0\\contact_complexite.txt", "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    for (int i = 0; i < list->level_max; i++) {
        t_d_cell_contact *current = list->head[i];
        while (current != NULL) {
            fprintf(file, "%s %d\n", current->contact->nom_prenom, i);
            current = current->next[i];
        }
    }

    fclose(file);
}

// Détermine le niveau de chaînage pour un nouveau contact
int determine_chain_level(char *previous_name, char *new_name) {
    // Convertir les caractères en minuscules
    char prev_char = tolower(previous_name[0]);
    char new_char = tolower(new_name[0]);

    // Vérifier si les deux premières lettres sont les mêmes mais la troisième est différente
    if (prev_char == new_char && tolower(previous_name[1]) == tolower(new_name[1]) && tolower(previous_name[2]) != tolower(new_name[2])) {
        return 1;
    }
    // Vérifier si la première lettre est la même mais la deuxième est différente
    else if (prev_char == new_char && tolower(previous_name[1]) != tolower(new_name[1])) {
        return 2;
    }
    // Vérifier si la première lettre est différente
    else if (prev_char != new_char) {
        return 3;
    }
    // Si aucune des conditions n'est remplie, ne pas chaîner à ce niveau
    else {
        return 0;
    }
}

// Chaîne un nouveau contact à un niveau donné
void chain_contact(t_d_list_contact *list, t_d_cell_contact *new_cell, t_d_cell_contact *previous, t_d_cell_contact *current, int chain_level) {
    // Parcourt chaque niveau jusqu'au niveau de chaînage
    for (int i = 0; i <= chain_level; i++) {
        // Chaîne le nouveau contact au contact actuel au niveau i
        new_cell->next[i] = current;
        
        // Si le contact précédent est NULL, cela signifie que nous ajoutons au début de la liste
        if (previous == NULL) {
            // Le nouveau contact devient la tête de la liste au niveau i
            list->head[i] = new_cell;
        } else {
            // Sinon, le contact précédent au niveau i pointe vers le nouveau contact
            previous->next[i] = new_cell;
        }
    }
}
