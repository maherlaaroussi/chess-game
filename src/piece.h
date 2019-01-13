#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED
#include"entete.h"

/* ------------------------- PIECE.H ------------------------- */

/* ------------------------- PROTOTYPEs ------------------------- */

/* ******************* piece_t ******************* */
piece_t piece_creer (int type, int couleur);
int piece_couleur (piece_t piece);
piece_t piece_identifier (char p);
char piece_caractere (piece_t piece);
void piece_afficher (piece_t piece);
void piece_detruire (piece_t *piece);
/* ************************************************** */

#endif
