#ifndef PARTIE_H_INCLUDED
#define PARTIE_H_INCLUDED
#include "piece.h"
#include"entete.h"

/* ------------------------- PARTIE.H ------------------------- */

/* ------------------------- PROTOTYPEs ------------------------- */

/* ******************* Les bases ******************* */
int case_vide (partie_t partie, coord_t coord);
void modifier_case (partie_t *partie, piece_t piece, coord_t coord);
void changer_joueur (partie_t *partie);
void partie_jouer ();
void pause_console ();
void partie_jouer_charger (char *fichier, double n);
void replay_jouer(partie_t * partie, double n);
/* **************************************************** */

/* ******************* Affichage ******************* */
void menu_jeu ();
void message_joueur (int joueur);
void afficher_deplacer_piece (partie_t *partie);
void afficher_echiquier (partie_t *partie);
void afficher_sauvegarder (partie_t *partie);
void afficher_liste_coups (liste_coups_t *coups);
void afficher_liste_captures (liste_captures_t *c);
void afficher_liste_pieces (partie_t *partie);
void afficher_liste_captures_joueur (partie_t *partie);
/* **************************************************** */

/* ******************* Saisies ******************* */
int saisie_choix();
coord_t saisie_case();
/* **************************************************** */

/* ******************* partie_t ******************* */
partie_t *partie_nouvelle ();
partie_t *partie_creer ();
void partie_detruire (partie_t *partie);
/* **************************************************** */

/* ******************* Sauvegarde et chargement ******************* */
void partie_sauvegarder (partie_t *partie, char *chemin_sauvegarder);
partie_t *replay_charger (char *chemin_charger);
void partie_plt_save (partie_t *partie, char *chemin_sauvegarder);
void partie_part_save (partie_t *partie, char *chemin_sauvegarder);
/* **************************************************** */

/* ******************* Deplacement ******************* */
void deplacement (partie_t *partie, coord_t coord_dep, coord_t coord_arr);
void annuler_deplacement (partie_t *partie);
int deplacement_valide (partie_t *partie, coord_t depart, coord_t arrive);
int deplacement_valide_pion (coord_t depart, coord_t arrive);
int deplacement_valide_fou (coord_t depart, coord_t arrive);
int deplacement_valide_tour (coord_t depart, coord_t arrive);
int deplacement_valide_roi (coord_t depart, coord_t arrive);
int deplacement_valide_reine (coord_t depart, coord_t arrive);
void deplacement_animation (partie_t *partie, coord_t dep, coord_t arr, double t);
int check_ligne ( partie_t *part, coord_t depart, coord_t arrivee);
/* **************************************************** */

/* ******************* Capture ******************* */
capture_t *capture_creer (int etat, piece_t piece);
void capture_detruire (capture_t *capture);

liste_captures_t *liste_captures_creer ();
void liste_captures_detruire (liste_captures_t *l);
int liste_captures_vide (liste_captures_t *captures);
int liste_captures_taille (liste_captures_t *captures);
void liste_captures_ajouter_debut (liste_captures_t *liste_captures, piece_t *piece);
capture_t *liste_captures_extraire_debut (liste_captures_t *liste_captures);

void supprimer_derniere_capture (liste_captures_t *captures);
/* **************************************************** */

/* ******************* Coup ******************* */
coup_t *coup_creer (int etat, coord_t depart, coord_t arrive);
void coup_detruire (coup_t *coup);

liste_captures_t *liste_coups_creer ();
void liste_coups_detruire (liste_coups_t *l);
int liste_coups_vide (liste_coups_t *coups);
int liste_coups_taille (liste_coups_t *coups);
void liste_coups_ajouter_debut (liste_coups_t *liste_coups, int etat, coord_t depart, coord_t arrive);
capture_t *liste_coups_extraire_fin (liste_coups_t *liste_coups);

void supprimer_dernier_coup (liste_coups_t *coups);
/* **************************************************** */

/* ******************* Initialisation ******************* */
void init_liste_captures(liste_captures_t *liste_captures);
void init_liste_coups(liste_coups_t *liste_coups);
/* **************************************************** */

#endif
