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
 * @brief Dessine un bouton interactif avec du texte centré.
 * @param renderer Le moteur de rendu SDL.
 * @param font La police de caractères pour le texte du bouton.
 * @param bouton Rectangle définissant la zone du bouton.
 * @param texte Le libellé à afficher à l'intérieur.
 * @param couleur La couleur de fond ou de bordure du bouton.
 */
void dessiner_bouton(SDL_Renderer *renderer, TTF_Font *font,
                     SDL_Rect bouton, const char *texte,
                     SDL_Color couleur);

/**
 * @author Saandi
 * @brief Affiche visuellement le monstre dans l'interface de choix.
 * @param renderer Le moteur de rendu SDL.
 */
void dessiner_monstre(SDL_Renderer *renderer);


/* --- Logique de sélection et fenêtrage --- */

/**
 * @author Saandi
 * @brief Ouvre une boîte de dialogue demandant au joueur de confirmer une action.
 * @param combat Contexte de combat utilisé pour l'affichage des protagonistes.
 * @return int Retourne le choix de l'utilisateur (1 pour confirmer, 0 pour annuler).
 */
int ouvrir_fenetre_choix(combat_t* combat);

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
