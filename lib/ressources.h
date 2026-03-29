/**
 * @file ressources.h
 * @author Massoud
 * @brief Gestion centralisée des ressources graphiques et textuelles.
 * * @details Fournit la structure et les fonctions nécessaires pour charger, 
 * stocker et libérer toutes les textures et polices utilisées dans le jeu.
 */

#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/carte.h" 

/**
 * @brief Structure regroupant toutes les textures et polices du jeu.
 * * @details Permet de conserver l'ensemble des assets graphiques (biomes, 
 * entités, UI) en mémoire pour éviter de les recharger à chaque frame.
 */
typedef struct {
    SDL_Texture* cases[NB_BIOMES];   /**< Textures des différents biomes composant la carte */
    SDL_Texture* perso;              /**< Texture du personnage principal */
    SDL_Texture* monstres[2];        /**< Textures des différents types de monstres */
    SDL_Texture* brouillard;         /**< Texture du brouillard de guerre */
    SDL_Texture* batiments[3];       /**< Textures des bâtiments interactifs ou décoratifs */
    SDL_Texture* sanctuaires[4];     /**< Textures des sanctuaires */
    SDL_Texture* obstacles[4];       /**< Textures des obstacles (rochers, arbres, etc.) */
    SDL_Texture* vie;                /**< Texture pour l'affichage des points de vie (coeurs) */
    
    TTF_Font* police_max;            /**< Police d'écriture grande taille */
    TTF_Font* police_min;            /**< Police d'écriture petite taille */
} RessourcesJeu;

/**
 * @brief Charge l'ensemble des ressources du jeu en mémoire.
 * * @details Lit les fichiers d'assets (images, polices) sur le disque et génère 
 * les textures SDL correspondantes.
 * * @param renderer Le moteur de rendu SDL utilisé pour créer les textures.
 * @param res Pointeur vers la structure RessourcesJeu à peupler.
 * * @return 0 en cas de succès, un code d'erreur (-1) si un fichier est manquant.
 */
int charger_ressources(SDL_Renderer* renderer, RessourcesJeu* res);

/**
 * @brief Libère la mémoire allouée pour les ressources du jeu.
 * * @details Détruit toutes les textures et ferme les polices contenues 
 * dans la structure pour éviter les fuites de mémoire. À appeler avant de quitter.
 * * @param res Pointeur vers la structure RessourcesJeu contenant les éléments à détruire.
 */
void liberer_ressources(RessourcesJeu* res);

#endif /* RESSOURCES_H */