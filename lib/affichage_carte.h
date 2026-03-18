#ifndef AFFICHAGE_CARTE_H
#define AFFICHAGE_CARTE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "couleur.h"
#include "monstre.h"
#include "perso.h"
#include "carte.h"
#include "maths.h"
#include "affichage_commun.h"

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
    SDL_Texture * textures_batiments[3],
    SDL_Texture * textures_sanctuaires[4],
    int tailleCase,
    int persX, int persY,
    int case_selection_x, int case_selection_y,
    perso_t *perso);

void preparer_avant_affichage();

void dessiner_interface_carte(SDL_Renderer *renderer, TTF_Font* ffont, SDL_Texture* portrait, perso_t * perso);

#endif 
