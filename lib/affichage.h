#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "../lib/couleur.h"
#include "../lib/monstre.h"
#include "../lib/perso.h"
#include "../lib/carte.h"
#include "../lib/maths.h"

/* Prototypes des fonctions de rendu SDL2 */
void afficher_hex_texture(SDL_Renderer* renderer, float cx, float cy, float rayon, SDL_Texture* texture, SDL_Color couleur);
void dessiner_contour_double_dore(SDL_Renderer* r, float cx, float cy, float rayon, int alpha);
void dessiner_contour_ftk(SDL_Renderer* r, float cx, float cy, float rayon, int epaisseur, SDL_Color couleur);
void afficher_hex_couleur(SDL_Renderer* renderer, float cx, float cy, float rayon, SDL_Color couleur);
void afficher_carte_sdl(SDL_Renderer * renderer,
    case_t carte[TAILLE_CARTE][TAILLE_CARTE],
    SDL_Texture * textures_cases[NB_BIOMES],
    SDL_Texture * textures_obstacles[4], 
    SDL_Texture * texture_brouillard,
    SDL_Texture * textures_monstres[2],
    SDL_Texture * textures_batiments[2],
    int tailleCase,
    int persX, int persY,
    int case_selection_x, int case_selection_y,
    perso_t *perso);

void preparer_avant_affichage();

#endif 
