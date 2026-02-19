#include <stdio.h>
#include <SDL2/SDL.h>
#include "../lib/carte.h"
#include "../lib/couleur.h"

#define EPAISSEUR_BORDURES 2

/* Leo */
void afficher_carte_sdl(SDL_Renderer * renderer, case_t carte[TAILLE_CARTE][TAILLE_CARTE],
	SDL_Texture * textures_cases[NB_BIOMES], int tailleCase, int persX, int persY) {
	SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);

	/* On en a besoin pour centrer la carte dans la fenêtre */
	int lFenetre; /* largeur de la fenêtre */
	int hFenetre; /* hauteur de la fenêtre */

	SDL_GetRendererOutputSize(renderer, &lFenetre, &hFenetre);

	int decalageX = (lFenetre / 2) - (persX * tailleCase + tailleCase / 2);
    int decalageY = (hFenetre / 2) - (persY * tailleCase + tailleCase / 2);

	int i, j;

	/* Dessin des cases de la carte */
	for (i = 0; i < TAILLE_CARTE; i++) {

		for (j = 0; j < TAILLE_CARTE; j++) {
			case_t maCase = carte[i][j];

			if (maCase.estVisible) {
				SDL_Rect rect = {
					j * tailleCase + decalageX,
					i * tailleCase + decalageY,
					tailleCase,
					tailleCase
				};

				SDL_RenderCopy(renderer,
					textures_cases[carte[i][j].biome],
					NULL,
					&rect);
			}

		}

	}

	/* Dessin des bordures entre les cases */
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

	int largeurTotale = TAILLE_CARTE * tailleCase;
	int hauteurTotale = TAILLE_CARTE * tailleCase;

	/* Lignes verticales */
	for (int i = 0; i <= TAILLE_CARTE; i++) {
		SDL_Rect barreVerticale = {
			decalageX + (i * tailleCase),
			decalageY,
			EPAISSEUR_BORDURES,
			hauteurTotale + EPAISSEUR_BORDURES
		};
		SDL_RenderFillRect(renderer, &barreVerticale);
	}

	/* Lignes horizontales */
	for (int i = 0; i <= TAILLE_CARTE; i++) {
		SDL_Rect barreHorizontale = {
			decalageX,
			decalageY + (i * tailleCase),
			largeurTotale + EPAISSEUR_BORDURES,
			EPAISSEUR_BORDURES
		};
		SDL_RenderFillRect(renderer, &barreHorizontale);
	}

}

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

					case NEIGE:
						couleur_fond(WHITE_BG);
						break;

					case FORET:
						couleur_fond(GREEN_BG);
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

}


/* Massoud */
void generer_eau(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {

    // Nombre de zones d'eau 
    int nb_zones = 15 + rand() % 20;

    for (int z = 0; z < nb_zones; z++) {

        // Point de départ aléatoire
        coordonnee_t dep;
        dep.x = rand() % TAILLE_CARTE;
        dep.y = rand() % TAILLE_CARTE;

        // Taille aléatoire (ex: 3 à 8 cases)
        int largeur = 40 + rand() % 100;
        int hauteur = 50 + rand() % 120;

        // Point d'arrivée
        coordonnee_t arr;
        arr.x = dep.x + largeur;
        arr.y = dep.y + hauteur;

        // Remplir la zone avec le biome EAU
        remplir_zone(carte, EAU, dep, arr);
    }
}

/*Saandi*/
void remplir_zone(case_t carte[TAILLE_CARTE][TAILLE_CARTE], biome_t biome, coordonnee_t dep, coordonnee_t arr){
	// Déterminer les bornes min et max
	int x_min = (dep.x < arr.x) ? dep.x : arr.x;
	int x_max = (dep.x > arr.x) ? dep.x : arr.x;
	int y_min = (dep.y < arr.y) ? dep.y : arr.y;
	int y_max = (dep.y > arr.y) ? dep.y : arr.y;
	
	// Limiter aux dimensions de la carte
	if(x_max >= TAILLE_CARTE) x_max = TAILLE_CARTE - 1;
	if(y_max >= TAILLE_CARTE) y_max = TAILLE_CARTE - 1;
	if(x_min < 0) x_min = 0;
	if(y_min < 0) y_min = 0;
	
	// Remplir la zone
	for(int i = x_min; i <= x_max; i++){
		for(int j = y_min; j <= y_max; j++){
			carte[i][j].biome = biome;
		}
	}
}

/* Saandi */
void generer_biomes(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
	int nb_biomes = 4;
	biome_t biomes[] = {TERRE, DESERT, NEIGE, FORET};
	
	// Pour chaque biome, créer plusieurs zones
	for(int i = 0; i < nb_biomes; i++){
		// Créer 2-3 zones par biome pour plus de variété
		int nb_zones = 10 + rand() % 20; // 2 ou 3 zones
		
		for(int z = 0; z < nb_zones; z++){
			// Générer un point de départ aléatoire
			coordonnee_t dep;
			dep.x = rand() % TAILLE_CARTE;
			dep.y = rand() % TAILLE_CARTE;
			
			// Générer une taille de zone aléatoire (par exemple 5 à 15 cases)
			int largeur = 40 + rand() % 80;
			int hauteur = 30 + rand() % 100;
			
			// Calculer l'arrivée
			coordonnee_t arr;
			arr.x = dep.x + largeur;
			arr.y = dep.y + hauteur;
			
			// Remplir la zone
			remplir_zone(carte, biomes[i], dep, arr);
		}
	}
}

