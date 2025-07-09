//Fichier main du projet
//Version : 1.0.0

//Partie 1 (100%) | 6/6
//Partie 2 (100%) | 7/7
//Partie 3 (100%) | 7/7
//Bonus (100%)    | 5/5
//Total           | 25/25

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "contact.c"
#include "tests.c"


int main() {
    int choix;
    int jour, mois, annee, heure, minute, duree_heure, duree_minute;
    int jour_supp, mois_supp, annee_supp, heure_supp, minute_supp, duree_heure_supp, duree_minute_supp;
    char objet[100];
    char objet_supp[100];
    Agenda *agenda = create_agenda();
    t_d_list_contact *list_contact = create_list_contact();
    char nom_prenom_contact[100];
    do {
        // Menu principal
        int result = 0;
        choix = 0;
        printf("\n\n\n      Menu principal \n");
        printf("\n1. Rechercher un contact\n");
        printf("2. Afficher les rendez-vous d’un contact\n");
        printf("3. Créer un contact\n");
        printf("4. Créer un rendez-vous pour un contact\n");
        printf("5. Supprimer un rendez-vous\n");
        printf("6. Sauvegarder le fichier de tous les contacts et les rendez-vous \n");
        printf("7. Charger un fichier de rendez-vous sauvegardé\n");
        printf("8. Créer une liste de contacts aléatoire grâce aux fichiers fournis\n");
        printf("9. Fournir les temps de calcul pour une insertion de nouveau contact\n");
        printf("10. Tester les parties 1 et 2 du projet\n");
        printf("11. Quitter\n");
        printf("\nChoisissez une option : ");
        result = scanf("%d", &choix);

        // Vider le tampon d'entrée
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (choix < 1 || choix > 11) {
            printf("\nOption valide\n");
            continue;
        }

        Contact *contact = NULL;
        switch (choix) {
            case 1:
            // Code pour rechercher un contact
                printf("\nNom et prénom du contact sous la forme 'nom_prenom' : ");
                scanf("%s", nom_prenom_contact);
                contact = find_contact_auto(agenda, nom_prenom_contact);
                if (contact != NULL) {
                    printf("\nContact %s trouvé.\n", contact->nom_prenom);
                } else {
                    printf("Contact non trouvé.\n");
                }
                break;
            case 2:
            // Code pour afficher les rendez-vous d'un contact
                printf("\nNom et prénom du contact sous la forme 'nom_prenom' : ");
                scanf("%s", nom_prenom_contact);
                contact = find_contact(agenda, nom_prenom_contact);
                if (contact != NULL) {
                    print_contact_rendezvous(contact);
                } else {
                    printf("Contact non trouvé.\n");
                }
                break;
            case 3:
            // Code pour créer un contact
                printf("\nNom et prénom du contact sous la forme 'nom_prenom' : ");
                char *nom_prenom_contact = scanString();
                for (int i = 0; nom_prenom_contact[i]; i++) {
                    if (!((nom_prenom_contact[i] >= 'a' && nom_prenom_contact[i] <= 'z') || 
                          (nom_prenom_contact[i] >= 'A' && nom_prenom_contact[i] <= 'Z') || 
                          nom_prenom_contact[i] == '_' || i>1 && nom_prenom_contact[i] == '-')) {
                        //Dans le cas du caractère ² ou ³, l'exception ne peut pas être gérée. Cela pourra donc conduire le programme à se comporter bizarrement. 
                        printf("\033[31mCaractère non autorisé trouvé. Veuillez n'utiliser que des lettres.\033[0m\n");
                        free(nom_prenom_contact);
                        printf("\nNom et prénom du contact sous la forme 'nom_prenom' : ");
                        nom_prenom_contact = scanString();
                        i = -1; // Réinitialiser l'index pour vérifier la nouvelle chaîne depuis le début
                    }
                }
                create_and_insert_contact(agenda, nom_prenom_contact, list_contact);
                free(nom_prenom_contact);
                break;
            case 4:
            // Code pour créer un rendez-vous pour un contact
                printf("\nNom et prénom du contact sous la forme 'nom_prenom' : ");
                char *nom_prenom_contact_rdv = scanString();
                for (int i = 0; nom_prenom_contact_rdv[i]; i++) {
                    if (!((nom_prenom_contact_rdv[i] >= 'a' && nom_prenom_contact_rdv[i] <= 'z') || 
                          (nom_prenom_contact_rdv[i] >= 'A' && nom_prenom_contact_rdv[i] <= 'Z') || 
                          nom_prenom_contact_rdv[i] == '_' || i>1 && nom_prenom_contact_rdv[i] == '-')) {
                        printf("\033[31mCaractère non autorisé trouvé. Veuillez n'utiliser que des lettres.\033[0m\n");
                        free(nom_prenom_contact_rdv);
                        printf("\nNom et prénom du contact sous la forme 'nom_prenom' : ");
                        nom_prenom_contact_rdv = scanString();
                        i = -1; // Réinitialiser l'index pour vérifier la nouvelle chaîne depuis le début
                    }
                }

                contact = find_contact(agenda, nom_prenom_contact_rdv);
                if (contact == NULL){
                    create_and_insert_contact(agenda, nom_prenom_contact_rdv, list_contact);
                    contact = find_contact(agenda, nom_prenom_contact_rdv);
                }
                jour = readIntInRange(1, 31, "Jour du rendez-vous : ");
                mois = readIntInRange(1, 12, "Mois du rendez-vous : ");
                annee = readIntInRange(2023, 2100, "Année du rendez-vous : ");
                heure = readIntInRange(0, 23, "Heure du rendez-vous : ");
                minute = readIntInRange(0, 59, "Minute du rendez-vous : ");
                duree_heure = readIntInRange(0, 23, "Durée en heures du rendez-vous : ");
                duree_minute = readIntInRange(0, 59, "Durée en minutes du rendez-vous : ");
                printf("Objet du rendez-vous : ");
                char *objet = scanString();
                RendezVous *rendezvous = create_rendezvous(jour, mois, annee, heure, minute, duree_heure, duree_minute, objet);
                add_rendezvous_to_contact(contact, rendezvous);
                printf("Rendez-vous ajouté.\n");
                free(objet);
                free(nom_prenom_contact_rdv);
                break;
            case 5:
            // Code pour supprimer un rendez-vous
                jour_supp = readIntInRange(1, 31, "Jour du rendez-vous à supprimer : ");
                mois_supp = readIntInRange(1, 12, "Mois du rendez-vous à supprimer : ");
                annee_supp = readIntInRange(2023, 2100, "Année du rendez-vous à supprimer : ");
                heure_supp = readIntInRange(0, 23, "Heure du rendez-vous à supprimer : ");
                minute_supp = readIntInRange(0, 59, "Minute du rendez-vous à supprimer : ");
                duree_heure_supp = readIntInRange(0, 23, "Durée en heures du rendez-vous à supprimer : ");
                duree_minute_supp = readIntInRange(0, 59, "Durée en minutes du rendez-vous à supprimer : ");
                printf("Objet du rendez-vous : ");
                char *objet_supp = scanString();

                // Trouver le rendez-vous à supprimer
                RendezVous *rendezvous_a_supprimer = find_rendezvous(agenda, jour_supp, mois_supp, annee_supp, heure_supp, minute_supp, duree_heure_supp, duree_minute_supp, objet_supp);
                if (rendezvous_a_supprimer == NULL) {
                    printf("Rendez-vous non trouvé.\n");
                } else {
                    printf("Rendez-vous trouvé.\n");
                    supprimer_rendezvous_de_tous_les_contacts(agenda, rendezvous_a_supprimer); // Suppression du rendez-vous de tous les contacts
                }
                free(objet_supp);           
                break;
            case 6:
                // Code pour sauvegarder le fichier de tous les rendez-vous
                save_all_rendezvous_to_file(agenda);
                printf("Liste des rendez vous sauvegardée dans le fichier rendezvous.txt\n");
                break;
            case 7:
                // Code pour charger un fichier de rendez-vous
                load_contacts_and_rendezvous_from_file(agenda, list_contact);
                printf("Liste des rendez vous chargée depuis le fichier rendezvous.txt\n");               
                break;
            case 8:
                // Code pour charger un fichier de rendez-vous déjà existant
                printf("\nChargement en cours...\n");
                load_contacts_from_file(agenda, "C:\\Users\\cleme\\Documents\\Prepa\\L2\\S3\\Informatique\\Projet_SDD\\V_1.0\\noms2008nat_complet.txt", "C:\\Users\\cleme\\Documents\\Prepa\\L2\\S3\\Informatique\\Projet_SDD\\V_1.0\\nat2021.csv", list_contact);
                printf("\nListe des contacts chargée\n");
                break;
            case 9:
                // Mesure du temps de recherche pour la recherche simple (Complexité O(n))
                printf("Nom et prénom du contact sous la forme 'nom_prenom' : ");
                // Lecture du nom et prénom du contact à partir de l'entrée utilisateur
                char *nom_prenom_contact_test = scanString();
                // Création d'un nouveau contact
                Contact *new_contact = malloc(sizeof(Contact));
                Contact *new_contact_2 = malloc(sizeof(Contact)); // Contact pour la recherche à plusieurs niveaux
                new_contact->nom_prenom = nom_prenom_contact_test;

                startTimer();
                add_contact_to_list_from_top(list_contact, new_contact);
                save_contacts_to_file(list_contact);
                stopTimer();

                // Récupération du temps écoulé pour l'insertion simple 
                char *time_simple_insert = getTimeAsString();
                printf("Temps écoulé pour l'insertion simple : ");
                printf("%s\n", time_simple_insert);

                // Mesure du temps de recherche pour la recherche à plusieurs niveaux Complexité O(log(n))
                printf("Nom et prénom du 2e contact sous la forme 'nom_prenom' : ");
                // Lecture du nom et prénom du deuxième contact à partir de l'entrée utilisateur
                char *nom_prenom_contact_test_2 = scanString();
                new_contact_2->nom_prenom = nom_prenom_contact_test_2;

                startTimer();
                add_contact_to_list_from_bottom(list_contact, new_contact_2);
                stopTimer();
                char *time_top_insert = getTimeAsString(); 
                printf("Temps écoulé pour la recherche à plusieurs niveaux : ");
                printf("%s\n", time_top_insert);
                save_contacts_to_file(list_contact);
                free(nom_prenom_contact_test);
                break;
            case 10:
                // Tests des parties 1 et 2 du projet
                tests_part_1_2();
                break;
        }
    } while (choix != 11);

    printf("Merci d'avoir utilisé notre agenda !\n");
    return 0;
}
