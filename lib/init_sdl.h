/**
 * @file init_sdl.h
 * @author Massoud
 * @brief Fonctions d'initialisation des sous-systèmes de la SDL.
 * * @details Ce fichier contient les déclarations des fonctions responsables 
 * de la mise en place de la SDL, de l'audio (SDL_mixer) et du texte (SDL_ttf).
 */

#ifndef INIT_SDL_H
#define INIT_SDL_H

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief Initialise globalement la SDL, la vidéo, l'audio et TTF.
 * * @details Prépare l'environnement de base nécessaire au fonctionnement du jeu. 
 * Doit impérativement être appelée avant toute autre fonction liée à la SDL.
 * * @return 0 en cas de succès, -1 en cas d'erreur.
 */
int init_sdl_global(void);

/**
 * @brief Initialise SDL_mixer et charge la musique principale du jeu.
 * * @details Configure les paramètres audio (fréquence, canaux, etc.) et charge 
 * le fichier musical de fond en mémoire.
 * * @return Un pointeur vers la structure Mix_Music contenant la musique, 
 * ou NULL en cas d'échec de chargement.
 */
Mix_Music* init_sdl_musique(void);

/**
 * @brief Charge la police d'écriture par défaut en petite taille (24).
 * * @return Un pointeur vers la police TTF_Font chargée, ou NULL en cas d'erreur.
 */
TTF_Font* init_ttf_min(void);

/**
 * @brief Charge la police d'écriture par défaut en grande taille (32).
 * * @return Un pointeur vers la police TTF_Font chargée, ou NULL en cas d'erreur.
 */
TTF_Font* init_ttf_max(void);

#endif /* INIT_SDL_H */