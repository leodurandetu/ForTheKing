#ifndef OPTION_AF_H
#define OPTION_AF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/carte.h"
#include "../lib/perso.h"
#include "../lib/monstre.h"
#include "../lib/combat.h"

#define MAX_FUITE 3

/**
 * @file option_af.h
 * @author Saandi
 * @brief Gestion de l'interface de confirmation et de sélection avant combat.
 */

/* --- Fonctions de rendu d'interface --- */

/**
 * @author Saandi
 * @brief Dessine un bouton coloré avec son libellé centré sur une surface SDL.
 *
 * @param surface  La surface SDL sur laquelle le bouton est dessiné.
 * @param font     La police de caractères utilisée pour le texte du bouton.
 * @param bouton   Rectangle définissant la position et la taille du bouton.
 * @param texte    Le libellé à afficher au centre du bouton.
 * @param couleur  La couleur de fond du bouton (RGB).
 */
void dessiner_bouton(SDL_Surface *surface, TTF_Font *font,
                     SDL_Rect *bouton, const char *texte,
                     SDL_Color couleur);

/**
 * @author Saandi
 * @brief Charge et affiche le sprite du monstre (squelette) dans le renderer.
 *
 * @param renderer Le moteur de rendu SDL utilisé pour l'affichage.
 * @return SDL_Rect Rectangle représentant la zone occupée par le monstre à l'écran.
 */
SDL_Rect dessiner_monstre(SDL_Renderer *renderer);


/* --- Logique de sélection et fenêtrage --- */

/**
 * @author Saandi
 * @brief Affiche un menu contextuel à côté du joueur proposant d'attaquer ou de fuir.
 *
 * Le menu se positionne automatiquement à droite du sprite du joueur,
 * ou à gauche si la place manque. Il reste toujours dans les limites
 * de la fenêtre grâce à une récupération dynamique de la taille du renderer.
 * Si @p fuite_bloquee est non nul, le bouton "Fuir" est affiché en gris
 * et les clics sur celui-ci sont ignorés.
 *
 * @param gRenderer    Le moteur de rendu SDL utilisé pour l'affichage.
 * @param gFont        La police de caractères utilisée pour les boutons.
 * @param perso_x      Position X en pixels du sprite du joueur à l'écran.
 * @param perso_y      Position Y en pixels du sprite du joueur à l'écran.
 * @param perso_w      Largeur en pixels du sprite du joueur.
 * @param perso_h      Hauteur en pixels du sprite du joueur.
 * @param fuite_bloquee 1 si la fuite est interdite (max de fuites atteint), 0 sinon.
 * @return int          1 si le joueur choisit d'attaquer, 0 s'il choisit de fuir.
 */
int afficher_option(SDL_Renderer *gRenderer, TTF_Font *gFont,
                    int perso_x, int perso_y, int perso_w, int perso_h,
                    int fuite_bloquee);

/**
 * @author Saandi
 * @brief Vérifie si la case ciblée par le clic contient un monstre.
 * @param carte La matrice représentant la carte du jeu.
 * @param x Coordonnée X de la case dans la grille.
 * @param y Coordonnée Y de la case dans la grille.
 * @return int Retourne 1 si un monstre est présent, 0 sinon.
 */
int monstre_dans_case_cliquee(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y);

#endif
