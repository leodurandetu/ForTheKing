#ifndef AFFICHAGE_COMBAT_H
#define AFFICHAGE_COMBAT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "affichage_commun.h"
#include "combat_types.h"
#include "math.h"

/* --- Prototypes des fonctions --- */

/**
 * @author Massoud & Léo
 * @brief Gère la boucle de rendu et les interactions de la fenêtre de combat.
 * @details Met à jour l'affichage des entités (joueur et monstre), des barres de vie 
 * et traite les clics utilisateur pour déclencher les actions de combat.
 * @param combat Pointeur vers la structure d'état contenant toutes les données du combat en cours.
 * @param clicGauche État logique du clic souris (1 si le bouton est pressé, 0 sinon).
 */
void maj_affichage_fenetre_combat(combat_t * combat, int clicGauche);

/**
 * @author Massoud & Léo
 * @brief Affiche un log ou un message textuel spécifique au combat.
 * @details Cette fonction permet de donner un feedback textuel au joueur sur les actions réalisées.
 * Déclarée en 'extern' car son implémentation peut se situer dans un autre module de rendu.
 * @param combat Pointeur vers la structure de combat pour accéder au renderer et aux polices.
 * @param texte Chaîne de caractères contenant le message à afficher (ex: "Coup critique !").
 */
extern void afficher_message_combat(combat_t *combat, const char *texte);

#endif
