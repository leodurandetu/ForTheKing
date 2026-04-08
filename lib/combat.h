#ifndef COMBAT_H
#define COMBAT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "perso.h"
#include "monstre.h"
#include "carte_types.h"
#include "math.h"
#include "affichage_combat.h"
#include "niveau.h"
#include "combat_types.h"

/* --- Gestion du cycle de vie du combat --- */

/**
 * @author Léo
 * @brief Initialise les ressources et prépare l'affichage de la scène de combat.
 * @details Charge les textures (joueur, monstre, fond selon le biome), initialise la police 
 * et configure les boutons d'action. Définit le premier tour pour le joueur.
 * @param pFenetrePrincipal Fenêtre parente utilisée pour le contexte d'affichage.
 * @param rendererPrincipal Moteur de rendu principal pour la création des textures.
 * @param biome Type de biome actuel servant à déterminer l'image de fond.
 * @param combat Pointeur vers la structure de combat à initialiser.
 * @param carte Matrice de la carte de jeu.
 * @param vies_globales Pointeur vers le compteur de vies du joueur.
 */
extern void ouvrir_fenetre_combat(SDL_Window * pFenetrePrincipal, SDL_Renderer * rendererPrincipal, biome_t biome, combat_t ** combat, 
    case_t ** carte, int *vies_globales, SDL_Texture * portraitPerso);

/**
 * @author Léo
 * @brief Gère la clôture du combat et applique les conséquences de l'issue du duel.
 * @details En cas de victoire, attribue l'XP, génère un loot (kit de soins) et place une tombe. 
 * En cas de défaite, gère l'état de mort du personnage.
 * @param rendererPrincipal Moteur de rendu pour la création d'éventuels objets de loot.
 * @param combat Pointeur vers la structure du combat qui se termine.
 * @param carte Matrice de la carte pour la suppression du monstre et l'ajout d'un bâtiment (tombe).
 * @param vainqueur Identifiant du gagnant (JOUEUR_VAINQUEUR ou MONSTRE_VAINQUEUR).
 * @param vies_globales Pointeur vers le compteur de vies pour la gestion de la défaite.
 */
extern void combat_termine(SDL_Renderer * rendererPrincipal, combat_t ** combat, case_t ** carte, vainqueur_t vainqueur, int *vies_globales);

/**
 * @author Léo
 * @brief Libère proprement la mémoire et détruit toutes les textures liées au combat.
 * @param combat Pointeur de pointeur vers la structure de combat à désallouer.
 */
extern void detruire_combat(combat_t ** combat);

/**
 * @author Saandi
 * @brief Alloue la structure de combat et lie le joueur à son adversaire.
 * @param perso Pointeur vers la structure du personnage joueur.
 * @param monstre Pointeur vers la structure du monstre rencontré.
 * @return combat_t* Un pointeur vers la structure de combat nouvellement allouée.
 */
combat_t* creer_combat(perso_t *perso, monstre_t *monstre);


/* --- Logique des tours et actions --- */

/**
 * @author Massoud
 * @brief Alterne le tour de jeu de manière cyclique.
 * @param combat Pointeur vers le contexte de combat actuel.
 * @return tour_t Le nouveau tour actif (TOUR_JOUEUR ou TOUR_MONSTRE).
 */
extern tour_t changer_tour(combat_t *combat);

/**
 * @author Massoud
 * @brief Exécute une attaque puissante du joueur sur le monstre.
 * @details Calcule des dégâts aléatoires compris entre 30% et 60% des dégâts de base du joueur.
 * @param combat Pointeur vers le contexte de combat contenant l'attaquant et la cible.
 */
extern void attaque_lourde(combat_t *combat);

/**
 * @author Massoud
 * @brief Exécute une attaque rapide du joueur sur le monstre.
 * @details Calcule des dégâts aléatoires compris entre 20% et 40% des dégâts de base du joueur.
 * @param combat Pointeur vers le contexte de combat.
 */
extern void attaque_legere(combat_t *combat);

/**
 * @author Massoud
 * @brief Exécute une attaque rapide du monstre sur le joueur.
 * @details Calcule des dégâts aléatoires compris entre 5% et 30% des dégâts de base du monstre.
 * @param combat Pointeur vers le contexte de combat.
 */
extern void attaque_legere_monstre(combat_t *combat);

/**
 * @author Massoud
 * @brief IA du monstre : décide de l'action à entreprendre selon la situation.
 * @details Analyse la santé restante du joueur et du monstre pour choisir entre une 
 * attaque légère ou lourde, intégrant une part de hasard pour l'imprévisibilité.
 * @param combat Pointeur vers le contexte de combat.
 */
extern void choix_attaque_monstre(combat_t *combat);

#endif
