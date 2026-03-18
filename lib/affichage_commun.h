#ifndef AFFICHAGE_COMMUN_H
#define AFFICHAGE_COMMUN_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* Prototypes des fonctions */

SDL_Texture* creer_texte(SDL_Renderer* renderer, TTF_Font* police, const char* texte, SDL_Color couleur, SDL_Rect* rectangle_sortie);
void dessiner_barre(SDL_Renderer* r, TTF_Font* f, const char* label, int val, int max, SDL_Rect zone, SDL_Color coul_barre);

#endif