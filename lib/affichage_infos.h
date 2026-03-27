#ifndef AFFICHAGE_INFOS_H
#define AFFICHAGE_INFOS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "perso.h"

/* --- Prototypes des fonctions --- */

/**
 * @author Léo
 * @brief Met à jour les ressources SDL (surface et texture) pour afficher un nouveau texte.
 * @details Libère l'ancienne texture avant de créer la nouvelle pour éviter les fuites mémoire 
 * lors de la mise à jour dynamique des informations (ex: compteurs de points de vie).
 * @param contenu La nouvelle chaîne de caractères à transformer en texture.
 * @param renderer Le moteur de rendu SDL.
 * @param police La police de caractères TTF à utiliser.
 * @param texte Pointeur vers la surface SDL (sera mise à jour).
 * @param texte_tex Pointeur vers la texture SDL (sera libérée puis recréée).
 * @param txtDestRect Pointeur vers le rectangle de destination pour mettre à jour les dimensions.
 */
void mise_a_jour_texte(char * contenu, SDL_Renderer * renderer, TTF_Font* police, 
                       SDL_Surface ** texte, SDL_Texture ** texte_tex, SDL_Rect * txtDestRect);

/**
 * @author Massoud
 * @brief Extrait une information spécifique d'une structure personnage sous forme de chaîne.
 * @details Formate une statistique du personnage (ex: "PV : 10/20") dans un buffer pour l'affichage.
 * @param perso Pointeur vers le personnage dont on veut extraire les données.
 * @param attribut Le nom de la caractéristique à récupérer (ex: "PV", "Force", "XP").
 * @param buffer_sortie Le tableau de caractères (chaîne) où sera stocké le texte formaté final.
 */
void get_info_personnage(perso_t *perso, const char* attribut, char* buffer_sortie);

#endif
