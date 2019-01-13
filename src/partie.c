#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "partie.h"


/* **************************************************************************************
***************************************** Bases **************************************
*************************************************************************************** */

/* ------------------------------------------------------------- */

int case_vide (partie_t partie, coord_t coord) {

    if (partie.echiquier[coord.x][coord.y].type == VIDE) { return TRUE; }
    else { return FALSE; }

}

/* ------------------------------------------------------------- */

void modifier_case (partie_t *partie, piece_t piece, coord_t coord) {

    partie->echiquier[coord.x][coord.y] = piece;

}

/* ------------------------------------------------------------- */

void changer_joueur (partie_t *partie) {
    if (partie->joueur == NOIR) { partie->joueur = BLANC; }
    else { partie->joueur = NOIR; }
}

/* ------------------------------------------------------------- */

void partie_jouer () {

    partie_t *partie = partie_nouvelle();
    int choix;

    do {

        printf("\033[H\033[2J");

        afficher_echiquier(partie);

        message_joueur(partie->joueur);
        menu_jeu();

        printf("\nVotre choix ? ");
        choix = saisie_choix();

        switch (choix) {

        case DEPLACER_PIECE :
            afficher_deplacer_piece(partie);
            printf("\033[H\033[2J\033[H\033[2J");
            break;

        case ANNULER_COUP :
            annuler_deplacement(partie);
            printf("\033[H\033[2K");
            printf("\033[H\033[2J");
            break;

        case MES_PIECES :
            afficher_liste_pieces(partie);
            afficher_liste_captures_joueur(partie);
            pause_console();
            break;

        }

    } while (choix != QUITTER);

    afficher_sauvegarder(partie);

    partie_detruire(partie);

}


/* ------------------------------------------------------------- */

void partie_jouer_charger (char *fichier, double n) {


    partie_t *partie = replay_charger(fichier);
	replay_jouer(partie, 0.1);

    int choix;

    do {

        printf("\033[H\033[2J");

        afficher_echiquier(partie);

        message_joueur(partie->joueur);
        menu_jeu();

        printf("\nVotre choix ? ");
        choix = saisie_choix();

        switch (choix) {

        case DEPLACER_PIECE :
            afficher_deplacer_piece(partie);
            printf("\033[H\033[2J\033[H\033[2J");
            break;

        case ANNULER_COUP :
            annuler_deplacement(partie);
            printf("\033[H\033[2K");
            printf("\033[H\033[2J");
            break;

        case MES_PIECES :
            afficher_liste_pieces(partie);
            afficher_liste_captures_joueur(partie);
            pause_console();
            break;

        }

    } while (choix != QUITTER);

    afficher_sauvegarder(partie);

    partie_detruire(partie);

}

/* ------------------------------------------------------------- */

void pause_console () {
	char c;
	printf("\nAppuyez sur une touche pour continuer ... ");
	scanf("%s", &c);
}

/* ------------------------------------------------------------- */

void replay_jouer(partie_t * partie, double n) {

	coup_t * c = partie->coups.last;
	int i;

	afficher_echiquier(partie);
	sleep(n);

	for (i = partie->coups.taille ; i > 0; i--) {
		deplacement(partie, c->depart, c->arrive);
		sleep(n);
		c = c->prev;
		changer_joueur(partie);
	}

	printf("\n\n\n\n");

}

/* **************************************************************************************
***************************************** Affichage **************************************
*************************************************************************************** */

/* ------------------------------------------------------------- */

void afficher_echiquier (partie_t *partie) {

    int x = 0, y = 0;

    printf("\t*********** PLATEAU ***********\n\n");
    printf("\t    0  1  2  3  4  5  6  7\n");
    printf("\t   ------------------------\n");

    for (x = 0 ; x < PLATEAU ; x++) {

        printf("\t%d | ", x);

        for (y = 0 ; y < PLATEAU ; y ++) {
            if (y == PLATEAU - 1) { printf("%c", piece_caractere(partie->echiquier[x][y])); }
            else { printf("%c  ", piece_caractere(partie->echiquier[x][y])); }
        }

        printf("\n");

    }

    printf("\n\t******************************\n");


}

/* ------------------------------------------------------------- */

void afficher_liste_coups (liste_coups_t *c) {

    coup_t *coup;

    for (coup = c->first ; coup != NULL ; coup = coup->next)
    {
        printf("Depart : %d %d\n", coup->depart.x, coup->depart.y);
        printf("Arrivee : %d %d\n", coup->arrive.x, coup->arrive.y);
        printf("Etat : %d\n\n", coup->piece_etat);
    }
}

/* ------------------------------------------------------------- */

void afficher_liste_captures (liste_captures_t *c) {

    capture_t *capture = c->first;

    while (capture != NULL)
    {
        printf("Piece : %c\n", piece_caractere(capture->piece));
        printf("Etat : %d\n\n", capture->etat);
        capture = capture->next;
    }

}

/* ------------------------------------------------------------- */

void afficher_liste_pieces (partie_t *partie) {

    int x, y, piece_ligne = FALSE;

    printf("\nPieces : \n");

    for (x = 0 ; x < PLATEAU ; x++) {

        for (y = 0 ; y < PLATEAU ; y ++) {
            if (partie->joueur == partie->echiquier[x][y].couleur) {
                printf("%c (%d, %d) ", piece_caractere(partie->echiquier[x][y]), x, y);
                piece_ligne = TRUE;
            }

            if (y == PLATEAU - 1 && piece_ligne == TRUE)
                printf("\n");

        }

        piece_ligne = FALSE;

    }

}

/* ------------------------------------------------------------- */

void afficher_liste_captures_joueur (partie_t *partie) {

    capture_t *capture = partie->captures.first;

    printf("\nCaptures : ");

    if (capture == NULL || capture->piece.couleur == partie->joueur) {
        printf("AUCUNES");
        }

    else {

        while (capture != NULL)
        {
            if (capture->piece.couleur != partie->joueur) {
                printf("%c ", piece_caractere(capture->piece));
            }

            capture = capture->next;
        }

    }

    printf("\n");
}

/* ------------------------------------------------------------- */

void menu_jeu () {

    printf("\n1. Deplacer une piece");
    printf("\n2. Annuler le coup precedent");
    printf("\n3. Voir mes pieces et mes captures\n");
    printf("\n0. Quitter le jeu\n");

}

/* ------------------------------------------------------------- */

void afficher_deplacer_piece (partie_t *partie) {

    coord_t depart, arrive;

    printf("\nQuel piece deplacer ?");
    depart = saisie_case();

    printf("\nOu voulez-vous la deplacer ?");
    arrive = saisie_case();

    if (deplacement_valide(partie, depart, arrive))
        changer_joueur(partie);

}

/* ------------------------------------------------------------- */

void message_joueur (int joueur) {

    printf("\t\tJoueur : ");

    if (joueur == NOIR)
        printf("Noir");
    else
        printf("BLANC");

    printf("\n");

}

/* ------------------------------------------------------------- */

void afficher_sauvegarder (partie_t *partie) {

    char nomSave[TAILLE] = "", cheminSave[TAILLE] = "/", cheminComplet[TAILLE] = "";
    int save;

    printf("\nVoulez-vous sauvegarder votre partie ?\n1. Oui\n0. Non\n");

    do {
    printf("Alors ? ");
    scanf("%d", &save);
    } while (save < 0 || save > 1);

    if (save == 1) {

        printf("Nom du fichier : ");
        scanf("%s", nomSave);

		/*
        printf("Chemin du fichier (ex : /home/ ou /) : ");
        scanf("%s", cheminSave);
		*/

        strcat(cheminComplet, ".");
        strcat(cheminComplet, cheminSave);
        strcat(cheminComplet, nomSave);

        partie_sauvegarder(partie, cheminComplet);
    }

}

/* ------------------------------------------------------------- */

/* **************************************************************************************
***************************************** Saisies **************************************
*************************************************************************************** */

/* ------------------------------------------------------------- */

coord_t saisie_case() {

    coord_t coord;

    do {
    printf("\nCoordonnees x, y (ex : 1-0) : ");
    scanf("%d-%d", &coord.x, &coord.y);
    } while (coord.x < 0 && coord.y > 7);

    return coord;

}

/* ------------------------------------------------------------- */

int saisie_choix() {

    int choix;

    do {
    scanf("%d", &choix);
    } while (choix < 0 || choix > 3);

    return choix;

}

/* ------------------------------------------------------------- */

/* **************************************************************************************
***************************************** partie_t **************************************
*************************************************************************************** */

/* ------------------------------------------------------------- */

partie_t *partie_creer () {

    partie_t *partie = (partie_t *) malloc(sizeof(partie_t));

    init_liste_captures(&partie->captures);
    init_liste_coups(&partie->coups);

    return partie;

}

/* ------------------------------------------------------------- */

void partie_detruire (partie_t *partie) {

    int x = 0, y = 0;
    capture_t *capture = partie->captures.first;
    coup_t *coup = partie->coups.first;

    while (capture != NULL)
    {
        capture = capture->next;
        supprimer_derniere_capture(&partie->captures);
    }

    while (coup != NULL)
    {
        coup = coup->next;
        supprimer_dernier_coup(&partie->coups);
    }

    for (x = 0 ; x < PLATEAU ; x++) {

        for (y = 0 ; y < PLATEAU ; y ++) {
            piece_detruire(&partie->echiquier[x][y]);
        }
    }

    free(&partie->captures);
    free(&partie->coups);
    free(partie);


}

/* ------------------------------------------------------------- */

partie_t *partie_nouvelle () {

    partie_t *partie = partie_creer();
    int i;

    /* ---------- PLATEAU ---------- */
    partie->echiquier[0][0] = piece_creer(TOUR, NOIR);
    partie->echiquier[0][1] = piece_creer(CAVALIER, NOIR);
    partie->echiquier[0][2] = piece_creer(FOU, NOIR);
    partie->echiquier[0][3] = piece_creer(ROI, NOIR);
    partie->echiquier[0][4] = piece_creer(REINE, NOIR);
    partie->echiquier[0][5] = piece_creer(FOU, NOIR);
    partie->echiquier[0][6] = piece_creer(CAVALIER, NOIR);
    partie->echiquier[0][7] = piece_creer(TOUR , NOIR);

    partie->echiquier[7][0] = piece_creer(TOUR, BLANC);
    partie->echiquier[7][1] = piece_creer(CAVALIER, BLANC);
    partie->echiquier[7][2] = piece_creer(FOU, BLANC);
    partie->echiquier[7][3] = piece_creer(ROI, BLANC);
    partie->echiquier[7][4] = piece_creer(REINE, BLANC);
    partie->echiquier[7][5] = piece_creer(FOU, BLANC);
    partie->echiquier[7][6] = piece_creer(CAVALIER, BLANC);
    partie->echiquier[7][7] = piece_creer(TOUR, BLANC);

    for (i = 0 ; i < PLATEAU ; i++) {
        partie->echiquier[1][i] = piece_creer(PION, NOIR);
        partie->echiquier[6][i] = piece_creer(PION, BLANC);

        partie->echiquier[2][i] = piece_creer(VIDE, NEUTRE);
        partie->echiquier[3][i] = piece_creer(VIDE, NEUTRE);
        partie->echiquier[4][i] = piece_creer(VIDE, NEUTRE);
        partie->echiquier[5][i] = piece_creer(VIDE, NEUTRE);

    }

    partie->joueur = BLANC;

    return partie;
}

/* ------------------------------------------------------------- */

/* **************************************************************************************
********************************* Sauvegarde et chargement ******************************
*************************************************************************************** */

/* ------------------------------------------------------------- */

void partie_sauvegarder (partie_t *partie, char *chemin_sauvegarder) {

    partie_plt_save(partie, chemin_sauvegarder);
    partie_part_save(partie, chemin_sauvegarder);

}

/* ------------------------------------------------------------- */

partie_t *replay_charger (char *chemin_charger) {

    FILE * fichierJeu, * fichierCoups;
    char caracActuelle, plt[256], part[256];
    partie_t *partie = partie_creer();
    int x, y;

	strcat(plt, "Plateaux/");
	strcat(part, "Parties/");

	strcat(plt, chemin_charger);
	strcat(part, chemin_charger);

	strcat(plt, ".plt");
	strcat(part, ".part");

    fichierJeu = fopen(plt, "r");
	fichierCoups = fopen(part, "r");

    if (fichierJeu != NULL)
    {


        if (fgetc(fichierJeu) == 'P' && fgetc(fichierJeu) == 'L') {

            caracActuelle = fgetc(fichierJeu);

            for (x = 0 ; x < PLATEAU ; x++) {

                for (y = 0 ; y < PLATEAU ; y ++) {

                    if (caracActuelle == '\n')
                        caracActuelle = fgetc(fichierJeu);
					
                    partie->echiquier[x][y] = piece_identifier(caracActuelle);
                    caracActuelle = fgetc(fichierJeu);

                }

            }
        }

        fclose(fichierJeu);

    }

	

	if (fichierCoups != NULL)
    {


        if (fgetc(fichierJeu) == 'P' && fgetc(fichierJeu) == 'R') {

            caracActuelle = fgetc(fichierJeu);

            for (x = 0 ; x < PLATEAU ; x++) {

                for (y = 0 ; y < PLATEAU ; y ++) {

                    if (caracActuelle == '\n')
                        caracActuelle = fgetc(fichierJeu);
					
                    /* Rajouter le coup dans la liste */

                    caracActuelle = fgetc(fichierJeu);

                }

            }
        }

        fclose(fichierJeu);

    }

    else
        perror("Erreur");

    return partie;

}

/* ------------------------------------------------------------- */

void partie_plt_save (partie_t *partie, char *chemin_sauvegarder){

    FILE * fichierJeu;
    int x, y;
    char jeu[TAILLE] = "PL\n", piece[2] = ".";
    char chemin[TAILLE] = "";

	strcat(chemin, "Plateaux/");
    strcat(chemin, chemin_sauvegarder);
    strcat(chemin, ".plt");

    for (x = 0 ; x < PLATEAU ; x++) {

        for (y = 0 ; y < PLATEAU ; y ++) {

            piece[0] = piece_caractere(partie->echiquier[x][y]);
            strcat(jeu, piece);

            if (y == PLATEAU - 1)
                strcat(jeu, "\n");

        }

    }


    fichierJeu = fopen(chemin, "wb");

    if (fichierJeu != NULL)
    {
        fprintf(fichierJeu, jeu);
        fclose(fichierJeu);
        printf("\nPlateau sauvegarde\n");
    }

    else
        perror("Erreur");

}

/* ------------------------------------------------------------- */

void partie_part_save (partie_t *partie, char *chemin_sauvegarder){

    coup_t *coup = partie->coups.last;
    FILE * fichierJeu;
    char jeu[TAILLE] = "PR\n";
    char coord[5] = "";
    char chemin[TAILLE] = "";

	strcat(chemin, "Parties/");
    strcat(chemin, chemin_sauvegarder);
    strcat(chemin, ".part");

    if (coup != NULL) {

        while (coup != NULL)
        {
            sprintf(coord, "%d%d%d%d", coup->depart.x, coup->depart.y, coup->arrive.x, coup->arrive.y);
            strcat(jeu, coord);
            strcat(jeu, "\n");
            coup = coup->prev;
        }

    }


    fichierJeu = fopen(chemin, "wb");

    if (fichierJeu != NULL)
    {
        fprintf(fichierJeu, jeu);
        fclose(fichierJeu);
        printf("Partie sauvegarde\n");
    }

    else
        perror("Erreur");

}

/* ------------------------------------------------------------- */


/* **************************************************************************************
***************************************** Deplacement **************************************
*************************************************************************************** */

/* ------------------------------------------------------------- */

void deplacement (partie_t *partie, coord_t coord_dep, coord_t coord_arr) {

    piece_t *p_arr = &partie->echiquier[coord_arr.x][coord_arr.y];
    piece_t p_dep = partie->echiquier[coord_dep.x][coord_dep.y];


    if (case_vide(*partie, coord_arr)) {
        liste_coups_ajouter_debut (&partie->coups, LIBRE, coord_dep, coord_arr);
    }

    else {
        liste_coups_ajouter_debut (&partie->coups, CAPTURE, coord_dep, coord_arr);
        liste_captures_ajouter_debut(&partie->captures, p_arr);
    }


    modifier_case(partie, piece_creer(VIDE, NEUTRE), coord_dep);
    modifier_case(partie, p_dep, coord_arr);




 }

/* ------------------------------------------------------------- */

void annuler_deplacement (partie_t *partie) {

    coup_t *last = partie->coups.first;

    if (liste_coups_vide(&partie->coups) != TRUE) {

    partie->echiquier[last->depart.x][last->depart.y] = partie->echiquier[last->arrive.x][last->arrive.y];

    if (partie->coups.first->piece_etat == CAPTURE) {
        partie->echiquier[last->arrive.x][last->arrive.y] =  piece_creer(partie->captures.first->piece.type, partie->captures.first->piece.couleur);
        supprimer_derniere_capture(&partie->captures);
    }
    else
        partie->echiquier[last->arrive.x][last->arrive.y] =  piece_creer(VIDE, NEUTRE);

    supprimer_dernier_coup(&partie->coups);

    }

    changer_joueur(partie);

}

/* ------------------------------------------------------------- */

int deplacement_valide (partie_t *partie, coord_t depart, coord_t arrive) {

    piece_t piece = partie->echiquier[depart.x][depart.y];

    if (piece.type == PION) {
        if (deplacement_valide_pion(depart, arrive) && piece_couleur(piece) == partie->joueur && check_ligne(partie, depart, arrive)) {
            deplacement(partie, depart, arrive);
            return TRUE;
        }
    }

    else if (piece.type == TOUR) {
        if (deplacement_valide_tour(depart, arrive) && piece.couleur == partie->joueur && check_ligne(partie, depart, arrive)) {
            deplacement(partie, depart, arrive);
            return TRUE;
        }
    }

	else if (piece.type == FOU) {
		if (deplacement_valide_fou(depart, arrive) && piece.couleur == partie->joueur && check_ligne(partie, depart, arrive)) {
			deplacement(partie, depart, arrive);
			return TRUE;
		}
	}

	else if (piece.type == ROI) {
        if (deplacement_valide_roi(depart, arrive) && piece.couleur == partie->joueur && check_ligne(partie, depart, arrive)) {
            deplacement(partie, depart, arrive);
            return TRUE;
        }
    }

	else if (piece.type == REINE) {
        if (deplacement_valide_reine(depart, arrive) && piece.couleur == partie->joueur && check_ligne(partie, depart, arrive)) {
            deplacement(partie, depart, arrive);
            return TRUE;
        }
    }

    return FALSE;

}

/* ------------------------------------------------------------- */

int deplacement_valide_pion (coord_t depart, coord_t arrive) {

    int x = arrive.x - depart.x;
    int inc = -1;

    if (x > 0)
        inc = 1;


    if (depart.x == 6 || depart.x == 1) {
            if (arrive.y == depart.y) {
                if (arrive.x == depart.x + inc || arrive.x == depart.x + (2 * inc))
                    return TRUE;
                    }
            }

    if (arrive.y == depart.y) {
        if (arrive.x == depart.x + inc)
            return TRUE;
                    }

    return FALSE;

}

/* ------------------------------------------------------------- */

int deplacement_valide_tour (coord_t depart, coord_t arrive) {

    if (depart.x == arrive.x || depart.y == arrive.y)
        return TRUE;

    return FALSE;

}

    /* ------------------------------------------------------------- */

int deplacement_valide_fou (coord_t depart, coord_t arrive) {

	coord_t pos;

	pos.x = abs(arrive.x - depart.x);
	pos.y = abs(arrive.y - depart.y);

    if (pos.x == pos.y)
        return TRUE;

    return FALSE;

}

/* ------------------------------------------------------------- */

int deplacement_valide_roi (coord_t depart, coord_t arrive) {

	coord_t pos;

	pos.x = abs(arrive.x - depart.x);
	pos.y = abs(arrive.y - depart.y);

    if ((pos.x == 0 || pos.x == 1) && (pos.y == 0 || pos.y == 1))
        return TRUE;


    return FALSE;
}

/* ------------------------------------------------------------- */

int deplacement_valide_reine (coord_t depart, coord_t arrive) {

	coord_t pos;

	pos.x = abs(arrive.x - depart.x);
	pos.y = abs(arrive.y - depart.y);

	/* Diagonale */
    if (pos.x == pos.y)
        return TRUE;

	/* Verticale et horizontale */
	else if (depart.x == arrive.x || depart.y == arrive.y)
		return TRUE;

	

    return FALSE;

}

/* ------------------------------------------------------------- */

int check_ligne (partie_t *part, coord_t depart, coord_t arrivee){

	/* Fonction à moitié fonctionelle */
	return TRUE; /* Temporaire */

    int i;
	coord_t pos, coord_now;

	pos.x = abs(arrivee.x - depart.x);
	pos.y = abs(arrivee.y - depart.y);


	/* Horizontale */
    if (depart.x == arrivee.x){
        if (depart.y > arrivee.y){
            for(i = depart.y ; i >= arrivee.y ; i-- ){
                coord_now.x = depart.x;
				coord_now.y = i;

                if(case_vide(*part, coord_now) == FALSE)
                    return FALSE;
            }
		}

		else {
        	for(i = depart.y ; i <= arrivee.y ; i++){
                coord_now.x = depart.x;
				coord_now.y = i;

                if(case_vide(*part, coord_now) == FALSE)
                    return FALSE;
        }
        }
	}

	

	/* Verticale */
    if (depart.y == arrivee.y){
        if(depart.x > arrivee.x){
            for(i = depart.x ; i >= arrivee.x ; i--){
                coord_now.x = i;
		coord_now.y = arrivee.y;

                if(case_vide(*part, coord_now) == FALSE)
                    return FALSE;

            }

        }

	else {
	    for(i = depart.x ; i <= arrivee.x ; i++){
	        coord_now.x = i;
			coord_now.y = arrivee.y;

        if(case_vide(*part, coord_now) == FALSE)
            return FALSE;
	    }
	}
    }


	/* Diagonale */
	if (pos.x == pos.y){
		if(depart.x > arrivee.x){
		    for(i = depart.x ; i >= arrivee.x ; i--){
					coord_now.x = i;
					coord_now.y = depart.y + i;

			if(case_vide(*part, coord_now) == FALSE)
			    return FALSE;

		    }

		}

		else {
		    for(i = depart.x ; i <= arrivee.x ; i++){
			coord_now.x = i;
				coord_now.y = depart.y + i;

			if(case_vide(*part, coord_now) == FALSE)
			    return FALSE;
		    }
		}
	}

    return TRUE;

}


/* ------------------------------------------------------------- */

void deplacement_animation (partie_t *partie, coord_t dep, coord_t arr, double t) {

    int i;

    for (i = 0 ; i < t*10 ; i++) {
        system("cls");
        afficher_echiquier(partie);
        if (i % 3 == 0)
            printf("\n\t\t\\ Deplacement \\");
        if (i % 3 == 1)
            printf("\n\t\t- Deplacement -");
        if (i % 3 == 2)
            printf("\n\t\t/ Deplacement /");

        pause_console();

        if (i == 7*t)
            deplacement(partie, dep, arr);


    }

    system("clear" );


}

/* ------------------------------------------------------------- */

/* ------------------------------------------------------------- */

/* **************************************************************************************
***************************************** Capture **************************************
*************************************************************************************** */

/* ------------------------------------------------------------- */

capture_t *capture_creer (int etat, piece_t piece) {

    capture_t *capture = (capture_t *) malloc(sizeof(capture_t));

    if(!capture) exit(EXIT_FAILURE);

    capture->etat = etat;
    capture->piece = piece;
    capture->next = NULL;

    return capture;
}

/* ------------------------------------------------------------- */

 void liste_captures_ajouter_debut (liste_captures_t *liste_captures, piece_t *piece) {

    capture_t *capture = capture_creer(CAPTURE, *piece);

    if (liste_captures_vide(liste_captures) == FALSE)
        capture->next = liste_captures->first;



    liste_captures->first = capture;
    liste_captures->taille++;

}

/* ------------------------------------------------------------- */

capture_t *liste_captures_extraire_debut (liste_captures_t *l) {

	capture_t *c = l->first;

    l->first = c->next;
    l->taille--;

    return c;

}

/* ------------------------------------------------------------- */

liste_captures_t *liste_captures_creer () {

	liste_captures_t *l = (liste_captures_t *) malloc (sizeof(liste_captures_t));
	l->first = NULL;
	l->taille = 0;

	return l;
}

/* ------------------------------------------------------------- */

void liste_captures_detruire (liste_captures_t *l) {
	while (liste_captures_vide(l)) {
		capture_detruire(l->first);
	}

	free (l);
}

/* ------------------------------------------------------------- */

void capture_detruire (capture_t *capture) {
    free(capture);
}

/* ------------------------------------------------------------- */

int liste_captures_vide (liste_captures_t *captures) {
    if (captures->taille == 0)
        return TRUE;
    else return FALSE;
}

/* ------------------------------------------------------------- */

int liste_captures_taille (liste_captures_t *c) {
    return c->taille;
}

/* ------------------------------------------------------------- */

void supprimer_derniere_capture (liste_captures_t *captures) {

    capture_t *last_capture = liste_captures_extraire_debut(captures);
	capture_detruire(last_capture);

}


/* ------------------------------------------------------------- */

/* **************************************************************************************
***************************************** Coup **************************************
*************************************************************************************** */

/* ------------------------------------------------------------- */

coup_t *coup_creer (int etat, coord_t depart, coord_t arrive) {

    coup_t *coup = (coup_t *) malloc(sizeof(coup_t));

    if(!coup) exit(EXIT_FAILURE);

    coup->depart.x = depart.x;
    coup->depart.y = depart.y;

    coup->arrive.x = arrive.x;
    coup->arrive.y = arrive.y;

    coup->piece_etat = etat;
    coup->next = NULL;
    coup->prev = NULL;

    return coup;
}

/* ------------------------------------------------------------- */

void liste_coups_ajouter_debut (liste_coups_t *liste_coups, int etat, coord_t depart, coord_t arrive) {

    coup_t *coup = coup_creer(etat, depart, arrive);

    if(liste_coups_vide(liste_coups) == FALSE) {
        liste_coups->first->prev = coup;
        coup->next = liste_coups->first;
        coup->prev = NULL;
    }
    else
        liste_coups->last = coup;

    liste_coups->first = coup;
    liste_coups->taille++;

}

/* ------------------------------------------------------------- */

void coup_detruire (coup_t *coup) {
    free(coup);
}

/* ------------------------------------------------------------- */

int liste_coups_vide (liste_coups_t *coups) {
    if (coups->taille == 0)
        return TRUE;
    else return FALSE;
}


/* ------------------------------------------------------------- */

int liste_coups_taille (liste_coups_t *coups) {
    return coups->taille;
}
/* ------------------------------------------------------------- */

void liste_coups_detruire (liste_coups_t *l) {

	while (liste_coups_vide(l)) {
		coup_detruire(l->first);
	}

	free (l);

}

/* ------------------------------------------------------------- */

void supprimer_dernier_coup (liste_coups_t *coups) {

    coup_t *last_coup = coups->first;
    coups->first = last_coup->next;
    last_coup->next = NULL;
    coups->taille--;
    coup_detruire(last_coup);

}

/* **************************************************************************************
***************************************** Initialisation **************************************
*************************************************************************************** */

/* ------------------------------------------------------------- */

 void init_liste_captures(liste_captures_t *liste_captures) {


    liste_captures->first = NULL;
    liste_captures->taille = 0;

}

/* ------------------------------------------------------------- */

void init_liste_coups(liste_coups_t *liste_coups) {

    liste_coups->first = NULL;
    liste_coups->last = NULL;
    liste_coups->taille = 0;

}

/* ------------------------------------------------------------- */

/* **************************************************************************************
*****************************************************************************************
*************************************************************************************** */
