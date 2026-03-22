#ifndef AFFICHAGE_COMMUN_H
#define AFFICHAGE_COMMUN_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "perso.h"

/* Structures et Enums */

typedef enum {
    INV_HORIZONTAL,
    INV_VERTICAL
} orientation_inv_t;

/* Prototypes des fonctions */

SDL_Texture* creer_texte(SDL_Renderer* renderer, TTF_Font* police, const char* texte, SDL_Color couleur, SDL_Rect* rectangle_sortie);
void dessiner_barre(SDL_Renderer* r, TTF_Font* f, const char* label, int val, int max, SDL_Rect zone, SDL_Color coul_barre);
void afficherMessageCentre(SDL_Renderer *renderer, TTF_Font *font, const char *message, int fenetre_w, int fenetre_h, SDL_Texture **texture, char *ancienMessage);
void dessiner_inventaire(SDL_Renderer * renderer, TTF_Font * font, perso_t * perso, SDL_Rect fond_interface, SDL_Rect rect_nom, int clic_gauche, int * maj_affichage, orientation_inv_t orientation, int start_droite_x);

#endif