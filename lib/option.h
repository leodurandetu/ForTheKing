#ifndef OPTION_AF_H
#define OPTION_AF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/carte.h"
#include "../lib/perso.h"
#include "../lib/monstre.h"
#include "../lib/combat.h"

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
 * @brief Charge le sprite d'un monstre et l'affiche dans le renderer à la position donnée.
 *
 * @param renderer Le moteur de rendu SDL utilisé pour l'affichage.
 * @param chemin   Chemin vers le fichier image du sprite.
 * @param x        Position horizontale (en pixels) où afficher le monstre.
 * @param y        Position verticale (en pixels) où afficher le monstre.
 * @param w        Largeur d'affichage du sprite (en pixels).
 * @param h        Hauteur d'affichage du sprite (en pixels).
 * @return SDL_Texture* Texture chargée (à détruire avec SDL_DestroyTexture),
 *                      ou NULL en cas d'échec.
 */
SDL_Texture* dessiner_monstre(SDL_Renderer *renderer, const char *chemin,
                               int x, int y, int w, int h);


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
 * @param gRenderer     Le moteur de rendu SDL utilisé pour l'affichage.
 * @param gFont         La police de caractères utilisée pour les boutons.
 * @param perso_x       Position X du joueur sur la carte.
 * @param perso_y       Position Y du joueur sur la carte.
 * @param perso_w       Largeur en pixels du sprite du joueur.
 * @param perso_h       Hauteur en pixels du sprite du joueur.
 * @param fuite_bloquee 1 si la fuite est interdite (max de fuites atteint), 0 sinon.
 * @param chemin_monstre L'image du monstre 
 * @param monstre_x     Position X de la case ou se trouve le monstre.
 * @param monstre_y     Position Y de la case ou se trouve le monstre.
 * @param taille_case   Taille de la case en pixels
 * @return int          1 si le joueur choisit d'attaquer, 0 s'il choisit de fuir.
 */
int afficher_option(SDL_Renderer *gRenderer, TTF_Font *gFont,
                    int perso_x, int perso_y, int perso_w, int perso_h,
                    int fuite_bloquee,const char *chemin_monstre,
                    int monstre_x, int monstre_y, int tailleCase);

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
