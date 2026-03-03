#ifndef AFFICHAGE_INFOS_H
#define AFFICHAGE_INFOS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* Prototypes des fonctions */
void mise_a_jour_texte(char * contenu, SDL_Renderer * renderer, TTF_Font* police, SDL_Surface ** texte, SDL_Texture ** texte_tex, SDL_Rect * txtDestRect);

#endif