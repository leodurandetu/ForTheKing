#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * @file pause_menu.h
 * @author Massoud
 * @brief Gestion de l'interface de pause en cours de partie.
 */

/* --- Prototypes des fonctions --- */

/**
 * @author Massoud
 * @brief Affiche une interface de pause en superposition (overlay) du jeu.
 * @details Cette fonction interrompt la logique principale pour capturer 
 * les entrées utilisateur (Continuer ou Quitter).
 * @param renderer Le moteur de rendu SDL2.
 * @param police La police de caractères pour les options du menu.
 * @param windowW Largeur de la fenêtre pour le centrage des éléments.
 * @param windowH Hauteur de la fenêtre pour le centrage des éléments.
 * @return int Code d'action utilisateur :
 * - 0 : Reprendre la partie (Continuer).
 * - 1 : Quitter la session actuelle et revenir au menu principal.
 */
int afficher_menu_pause(SDL_Renderer* renderer, TTF_Font* police, int windowW, int windowH);

#endif
