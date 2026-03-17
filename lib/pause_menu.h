#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* Massoud
 * Affiche le menu de pause par-dessus le jeu.
 * Retourne :
 *  0 = Continuer la partie
 *  1 = Quitter vers le menu
 */
int afficher_menu_pause(SDL_Renderer* renderer, TTF_Font* police, int windowW, int windowH);

#endif