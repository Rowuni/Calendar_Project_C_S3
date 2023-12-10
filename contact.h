#ifndef CONTACT_H
#define CONTACT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

// Structure pour stocker les informations d'un rendez-vous
typedef struct {
    int jour;           // Jour du rendez-vous
    int mois;           // Mois du rendez-vous
    int annee;          // Année du rendez-vous
    int heure;          // Heure du rendez-vous
    int minute;         // Minute du rendez-vous
    int duree_heure;    // Durée en heures du rendez-vous
    int duree_minute;   // Durée en minutes du rendez-vous
    char *objet;        // Objet du rendez-vous
} RendezVous;



// Structure pour un nœud dans une liste simplement chaînée de rendez-vous
typedef struct RendezVousNode {
    RendezVous *rendezvous;   // Pointeur vers une structure RendezVous
    struct RendezVousNode *next;  // Pointeur vers le prochain nœud dans la liste
} RendezVousNode;

typedef struct {
    char *nom_prenom;   // Nom et prénom du contact
    RendezVousNode *rendezvous; // Liste des rendez vous pour ce contact
} Contact;

typedef struct ContactNode {
    Contact *contact;   // Pointeur vers une structure Contact
    struct ContactNode *next;  // Pointeur vers le prochain nœud dans la liste
} ContactNode;

// Structure pour une entrée de l'agenda
typedef struct {
    Contact *contact; // Pointeur vers une structure Contact
    RendezVousNode *rendezvous; // Pointeur vers le premier nœud de la liste de rendez-vous
    t_d_cell *cell; // Pointeur vers une cellule de la liste
} EntreeAgenda;

typedef struct {
    ContactNode *contacts; // Liste des contacts
    RendezVousNode *rendezvous; // Liste de tous les rendez-vous
} Agenda;

typedef struct s_d_cell_contact {
    Contact *contact; // Contact de la cellule
    struct s_d_cell_contact **next; // Tableau de pointeurs vers les cellules suivantes
} t_d_cell_contact;

typedef struct s_d_list_contact {
    t_d_cell_contact **head; // Tableau de pointeurs vers les cellules de la liste
    int level_max; // Hauteur maximale de la liste
    int size; // Nombre de cellules dans la liste
} t_d_list_contact;


// Définitions des fonctions

void toLowerCase(char *str);
int readIntInRange(int min, int max, const char *prompt);
char *scanString();
Agenda *create_agenda();
Contact *create_contact(char *nom_prenom);
void insert_contact(Agenda *agenda, Contact *contact, t_d_list_contact *list_contact);
void create_and_insert_contact(Agenda *agenda, char *nom_prenom, t_d_list_contact *list_contact);
void print_contact_rendezvous(Contact *contact);
Contact *find_contact(Agenda *agenda, char *nom_prenom);
Contact *find_contact_auto(Agenda *agenda, char *nom_prenom);
RendezVous *create_rendezvous(int jour, int mois, int annee, int heure, int minute, int duree_heure, int duree_minute, char *objet);
void add_rendezvous_to_contact(Contact *contact, RendezVous *rendezvous);
RendezVous *find_rendezvous(Agenda *Agenda, int jour, int mois, int annee, int heure, int minute, int duree_heure, int duree_minute, char *objet);
void supprimer_rendezvous_de_tous_les_contacts(Agenda *agenda, RendezVous *rendezvous);
void save_all_rendezvous_to_file(Agenda *agenda);
void load_contacts_from_file(Agenda *agenda, const char *filename, const char *prenoms_filename, t_d_list_contact *list_contact);
void load_contacts_and_rendezvous_from_file(Agenda *agenda, t_d_list_contact *list_contact);
t_d_list_contact *create_list_contact();
void add_contact_to_list_from_top(t_d_list_contact *list, Contact *contact);
void add_contact_to_list_from_bottom(t_d_list_contact *list, Contact *contact);
void save_contacts_to_file(t_d_list_contact *list);
int determine_chain_level(char *previous_name, char *new_name);
void chain_contact(t_d_list_contact *list, t_d_cell_contact *new_cell, t_d_cell_contact *previous, t_d_cell_contact *current, int chain_level);


#endif
