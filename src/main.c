#include <stdio.h>
#include <stdlib.h>
#include "partie.h"

int main (int argc, char **argv)
{
	/* Nouvelle partie */
	if (argc == 1)
    	partie_jouer();
    
    /* Charger ou rejouer une partie */
    else if (argc == 2)
    	partie_jouer_charger(argv[1], 1);

    	
    /* Charger une partie avec animation avec un temps de pause */
    else if (argc == 3)
    	partie_jouer_charger(argv[1], *argv[2]);

    return EXIT_SUCCESS;
}
