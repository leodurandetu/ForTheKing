#include <stdio.h>
#include <SDL2/SDL.h>
#include "carte.h"
#include "couleur.h"

/* Leo */

/*
REMARQUE :
il manque l'affichage des batiments (campements et magasins)
on pourrait améliorer aussi l'affichage plus tard
*/
void afficher_carte(SDL_Renderer * renderer, case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
	SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);

	/* On en a besoin pour centrer la carte dans la fenêtre */
	int lFenetre; /* largeur de la fenêtre */
	int hFenetre; /* hauteur de la fenêtre */

	SDL_GetRendererOutputSize(renderer, &lFenetre, &hFenetre);

	int i, j;

	int tailleCase = 20;

	for (i = 0; i < TAILLE_CARTE; i++) {

		for (j = 0; j < TAILLE_CARTE; j++) {
			case_t maCase = carte[i][j];

			if (!maCase.estVisible) {
				SDL_SetRenderDrawColor (renderer, 0, 0, 0, 255);
			} else {

				switch (maCase.biome) {

					case TERRE:
						SDL_SetRenderDrawColor (renderer, 163, 130, 33, 255);
						break;

					case EAU:
						SDL_SetRenderDrawColor (renderer, 33, 66, 163, 255);
						break;

					case DESERT:
						SDL_SetRenderDrawColor (renderer, 232, 231, 114, 255);
						break;

					default:
						SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
						break;

				}

			}

			SDL_Rect rect;

			/* ce code un peu compliqué permet de 
			centrer la carte au milieu de la fenêtre */
			rect.y = i * tailleCase + (hFenetre / 2) - (TAILLE_CARTE * tailleCase / 2);
			rect.x = j * tailleCase + (lFenetre / 2) - (TAILLE_CARTE * tailleCase / 2);

			rect.w = tailleCase;
			rect.h = tailleCase;

			SDL_RenderFillRect(renderer, &rect);
		}

	}

}

/* Leo */
void afficher_carte_terminal(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
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
int test_etat_case(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y) {
	return 0;
}


/* Saandi */
void generer_biomes(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {

}