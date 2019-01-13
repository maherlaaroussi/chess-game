#ifndef ENTETE_H_INCLUDED
#define ENTETE_H_INCLUDED


/* ----- DEFINEs ----- */

#define PLATEAU 8
#define TAILLE 200

/* ----- ENUMs ----- */

enum pieces {VIDE, ROI, REINE, FOU, CAVALIER, TOUR, PION};
enum couleurs {NOIR, BLANC, NEUTRE};
enum etats {LIBRE, CAPTURE};
enum bool {FALSE, TRUE};
enum choix {QUITTER, DEPLACER_PIECE, ANNULER_COUP, MES_PIECES};

/* ----- STRUCTUREs ----- */

typedef struct piece_s {
    int type;
    int couleur;
} piece_t;

typedef struct coord_s {
    int x;
    int y;
} coord_t;

typedef struct pieces_captures_s {
    int etat;
    piece_t piece;
    struct pieces_captures_s *next;
} capture_t;

typedef struct liste_captures_s {
    capture_t *first;
    int taille;
} liste_captures_t;

typedef struct pieces_coups_s {
    coord_t depart;
    coord_t arrive;
    int piece_etat;
    struct pieces_coups_s *next;
    struct pieces_coups_s *prev;
} coup_t;

typedef struct liste_coups_s {
    coup_t *first;
    coup_t *last;
    int taille;
} liste_coups_t;

typedef struct partie_s {
    piece_t echiquier [PLATEAU][PLATEAU];
    liste_captures_t captures;
    liste_coups_t coups;
    int joueur;

} partie_t;


#endif
