#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/perso.h"
#include "../lib/monstre.h"
#include "../lib/carte_types.h"

/* Prototypes des fonctions */

/**
 * @author Léo
 * @brief Sauvegarde l'état du jeu dans un fichier binaire.
 * @param nomFichier Nom du fichier dans lequel sauvegarder les données
 * @param perso Pointeur vers le personnage du jeu
 * @param carte Carte du jeu
 * @param renderer Moteur de rendu du jeu
 * @return 1 si succès, 0 sinon.
 */
int sauvegarder_partie(const char * nomFichier, perso_t * perso, case_t ** carte, SDL_Renderer * renderer);

/**
 * @author Léo
 * @brief Charge l'état du jeu depuis un fichier binaire.
 *  @param nomFichier Nom du fichier depuis lequel charger les données
 * @param perso Pointeur vers le personnage du jeu
 * @param carte Carte du jeu
 * @param renderer Moteur de rendu du jeu
 * @details Alloue la mémoire nécessaire pour les monstres.
 * @return 1 si succès, 0 sinon.
 */
int charger_partie(const char * nomFichier, perso_t * perso, case_t ** carte, SDL_Renderer * renderer);

#endif