#ifndef AFFICHAGE_COMBAT_H
#define AFFICHAGE_COMBAT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "affichage_commun.h"
#include "combat_types.h"
#include "math.h"
#include "commun.h"
#include "combat.h"
#include "option.h"

/* --- Prototypes des fonctions --- */

/**
 * @author Massoud & Léo
 * @brief Gère la boucle de rendu et les interactions de la fenêtre de combat.
 * @details Met à jour l'affichage des entités (joueur et monstre), des barres de vie 
 * et traite les clics utilisateur pour déclencher les actions de combat.
 * @param combat Pointeur vers la structure d'état contenant toutes les données du combat en cours.
 * @param clicGauche État logique du clic souris (1 si le bouton est pressé, 0 sinon).
 */
void maj_affichage_fenetre_combat(combat_t * combat, int clicGauche, int * maj_affichage);

/**
 * @author Massoud & Léo
 * @brief Affiche un log ou un message textuel spécifique au combat.
 * @details Cette fonction permet de donner un feedback textuel au joueur sur les actions réalisées.
 * Déclarée en 'extern' car son implémentation peut se situer dans un autre module de rendu.
 * @param combat Pointeur vers la structure de combat pour accéder au renderer et aux polices.
 * @param texte Chaîne de caractères contenant le message à afficher (ex: "Coup critique !").
 */
extern void afficher_message_combat(combat_t *combat, const char *texte);

/**
 * @author Massoud et Léo
 * @brief Demande au joueur si il veut attaquer ou fuir, puis lance le combat si nécessaire.
 * @param renderer Le moteur de rendu SDL.
 * @param police2 La police pour les textes.
 * @param perso Pointeur vers le personnage.
 * @param monstre Pointeur vers le monstre.
 * @param tailleCase Taille d'une case hexagonale en pixels.
 * @param nbFuites Pointeur vers un entier qui représente le nombre de fuites.
 * @param vies_globales Pointeur vers un entier qui représente le nombre de vies globales.
 * @param etat Pointeur vers l'état du jeu.
 */
void demander_et_lancer_combat(SDL_Renderer * renderer, TTF_Font * police2, perso_t * perso, 
    monstre_t * monstre, int tailleCase, int * nbFuites, int * vies_globales, etat_jeu_t * etat,
    case_t ** carte, int carte_x, int carte_y, combat_t ** combat_actuel, SDL_Window * pFenetre,
    SDL_Texture * portraitPerso);

#endif
