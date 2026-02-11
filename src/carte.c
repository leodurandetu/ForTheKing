#include <stdio.h>
#include <SDL2/SDL.h>
#include "../lib/carte.h"
#include "../lib/couleur.h"

/* Leo */

/*
REMARQUE :
il manque l'affichage des batiments (campements et magasins)
on pourrait améliorer aussi l'affichage plus tard
*/
void afficher_carte_sdl(SDL_Renderer * renderer, case_t carte[TAILLE_CARTE][TAILLE_CARTE],
	SDL_Texture * textures_cases[NB_BIOMES]) {
	SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);

	/* On en a besoin pour centrer la carte dans la fenêtre */
	int lFenetre; /* largeur de la fenêtre */
	int hFenetre; /* hauteur de la fenêtre */

	SDL_GetRendererOutputSize(renderer, &lFenetre, &hFenetre);

	int i, j;

	int tailleCase = 50;

	for (i = 0; i < TAILLE_CARTE; i++) {

		for (j = 0; j < TAILLE_CARTE; j++) {
			case_t maCase = carte[i][j];

			if (maCase.estVisible) {
				SDL_Rect rect;

				/* ce code un peu compliqué permet de 
				centrer la carte au milieu de la fenêtre */
				rect.y = i * tailleCase + (hFenetre / 2) - (TAILLE_CARTE * tailleCase / 2);
				rect.x = j * tailleCase + (lFenetre / 2) - (TAILLE_CARTE * tailleCase / 2);

				rect.w = tailleCase;
				rect.h = tailleCase;

				SDL_RenderCopy(renderer, textures_cases[maCase.biome], NULL, &rect);

			}

		}

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
    

    for(int i_cmpt=0; i_cmpt < TAILLE_CARTE/3; i_cmpt++){
        for(int j_cmpt=0; j_cmpt < TAILLE_CARTE/3; j_cmpt++){
            int i = rand() % TAILLE_CARTE;
            int j = rand() % TAILLE_CARTE;
            int etat_case = test_etat_case(carte,i,j);
            if(etat_case != 4 && etat_case != 5){
                carte[i][j].biome = EAU;
            }
        }
    }
}

/* Massoud */
int test_etat_case(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y) {

    switch(carte[x][y].batiment.type){
        
        case CAMPEMENT: return 4;
        case MAGASIN: return 5;
        case PAS_DE_BATIMENT: return 6;
        default: printf("Erreur dans le type de batiment.\n");
                    return -2;
    }
}

/* Saandi */
void generer_biomes(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
	int i,j;
	int nb_biomes = 4;
	biome_t biomes[] = {TERRE,DESERT,NEIGE,FORET};
	/* on couvre un tiers de la surface de la carte (longueur x largeur) */
	for(int k = 0; k < (TAILLE_CARTE * TAILLE_CARTE) / 3; k++){
		int x = rand()%nb_biomes;
		i = rand()%TAILLE_CARTE;
		j = rand()%TAILLE_CARTE;

		switch (carte[i][j].biome)
		{
		case TERRE:
		case DESERT:
		case NEIGE:
		case FORET:
			carte[i][j].biome = biomes[x];
			break;
		default:
			break;
		}
	}
}
