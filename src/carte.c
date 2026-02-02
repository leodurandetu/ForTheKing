#include <stdio.h>
#include "carte.h"
#include "couleur.h"

/* Leo */
void afficher_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
	int x, y;

	couleur_reset();

	for (x = 0; x < TAILLE_CARTE; x++) {

		for (y = 0; y < TAILLE_CARTE; y++) {
			/*
			Utiliser ça lorsque Massoud aura terminé
			case_t maCase = test_etat_case(carte, x, y);
			*/

			case_t maCase = carte[x][y];

			if (!maCase.estVisible) {
				printf("   ");
			} else {

				switch (maCase.biome) {

					case TERRE:
						couleur_fond(YELLOW_BG);
						break;

					case EAU:
						couleur_fond(CYAN_BG);
						break;

					case DESERT:
						couleur_fond(ORANGE_BG);
						break;

					default:
						couleur_fond(DEFAULT_BG);
						break;

				}

				batiment_t monBatiment = maCase.batiment;

				couleur_texte(BLACK_TEXT);

				switch (monBatiment.type) {

					case CAMPEMENT:
						printf(" X ");
						break;

					case MAGASIN:
						printf(" M ");
						break;

					default:
						printf("   ");
						break;

				}

			}

		}

		couleur_reset();
		printf("\n");
	}

	couleur_reset();

	printf("\n");

}

/* Leo */
void init_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
	int x, y;

	for (x = 0; x < TAILLE_CARTE; x++) {

		for (y = 0; y < TAILLE_CARTE; y++) {
			case_t maCase;
			batiment_t monBatiment;

			monBatiment.type = PAS_DE_BATIMENT;

			maCase.biome = TERRE;
			maCase.batiment = monBatiment;

			maCase.estVisible = 1;

			carte[x][y] = maCase;
		}

	}

	/* pour tester (à remplacer par generer_biomes 
	et generer_eau une fois qu'elles seront terminées)*/
	carte[5][8].biome = EAU;

	int i, j;

	for (i = 8; i <= 10; i++) {

		for (j = 1; j <= 6; j++) {
			carte[i][j].batiment.type = CAMPEMENT;
			carte[i][j].biome = DESERT;
		}

	}

}

/* Massoud */
void generer_eau(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {

}

/* Massoud */
case_t test_etat_case(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y) {

}


/* Saandi */
void generer_biomes(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {

}