#include <stdlib.h>
#include <stdio.h>
#include "piece.h"

/* ------------------------------ FONCTIONS ------------------------------ */

/* ******************************************************** */

/*
Permet de créer une pièce
@type Le type de pièece
@couleur Sa couleur; nour, blanc o NULL
Return la pièce crée
*/
piece_t piece_creer (int type, int couleur) {

    piece_t piece;
    piece.type = type;
    piece.couleur = couleur;

    return piece;

}

/* ******************************************************** */

/*
Permet de connaître la couleur d'une pièce
@piece La pièce
Return la couleur de la pièce
*/

int piece_couleur (piece_t piece) {

    return piece.couleur;

}

/* ******************************************************** */

/*
La fonction identifie une pièce en fonction de son caractère
@p Le caractère à identifier
Return la pièce associé au caractère
*/

piece_t piece_identifier (char p){

    int i, j;

    for (i = 1 ; i < 7 ; i++) {
        for (j = 0 ; j < 2 ; j++) {
            if (p == piece_caractere(piece_creer(i, j)))
                return piece_creer(i, j);
        }
    }

    /*
    switch (p) {
        case 'R' : return piece_creer(ROI, NOIR); break;
        case 'r' : return piece_creer(ROI, BLANC); break;
        case 'E' : return piece_creer(REINE, NOIR); break;
        case 'e' : return piece_creer(REINE, BLANC); break;
        case 'F' : return piece_creer(FOU, NOIR); break;
        case 'f' : return piece_creer(FOU, BLANC); break;
        case 'C' : return piece_creer(CAVALIER, NOIR); break;
        case 'c' : return piece_creer(CAVALIER, BLANC); break;
        case 'T' : return piece_creer(TOUR, NOIR); break;
        case 't' : return piece_creer(TOUR, BLANC); break;
        case 'P' : return piece_creer(PION, NOIR); break;
        case 'p' : return piece_creer(PION, BLANC); break;

    }
    */

    return piece_creer(VIDE, NEUTRE);

}

/* ******************************************************** */

/*
Permet de connaître le caractère associé à une pièce
@piece La pièce à identifier
Return Le caractère associé à la pièce
*/

char piece_caractere (piece_t piece){

    int i, j;
    char pieces[6][2] = {{'R', 'r'}, {'E', 'e'}, {'F', 'f'}, {'C', 'c'}, {'T', 't'}, {'P', 'p'}};

    for (i = 1 ; i < 7 ; i++) {
        for (j = 0 ; j < 2 ; j++) {
            if (piece.type == i && piece.couleur == j)
                return pieces[i-1][j];
        }
    }

    return '.';

}

/* ******************************************************** */

/*
Affiche le char d'une pièce
@piece La pièce à afficher
Return rien
*/

void piece_afficher (piece_t piece){

    printf("%c", piece_caractere(piece));

}

/* ******************************************************** */

/*
Libère la mémoire occupé par la création d'une pièce
@*piece Pointer sur la pièce à détruire
Return rien
*/

void piece_detruire (piece_t *piece){

    free(piece);

}

/* ******************************************************** */
