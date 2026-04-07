#ifndef OBJET_H
#define OBJET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * @file objet.h
 * @author Léo
 * @brief Définition des objets et de leurs comportements via des pointeurs de fonctions.
 */

/* Déclaration anticipée pour éviter les inclusions cycliques */
typedef struct perso_s perso_t;

/** * @author Léo
 * @brief Types d'objets disponibles dans le jeu.
 */
typedef enum {
    KIT_DE_SOINS
} objet_type_t;

/** * @author Léo
 * @brief Structure représentant un objet de l'inventaire.
 * @details Utilise un pointeur de fonction pour définir un comportement 
 * spécifique lors de l'utilisation de l'objet.
 */
typedef struct {
    objet_type_t type;      /**< Identifiant du type d'objet. */
    int quantite;           /**< Nombre d'exemplaires possédés. */
    SDL_Texture * texture;  /**< Icône visuelle de l'objet. */
} objet_t;

/* --- Prototypes des fonctions --- */

/**
 * @author Léo
 * @brief Effet concret du kit de soins : restaure des points de vie au personnage.
 * @param perso Le personnage à soigner.
 */
void utiliser_kit_de_soins(perso_t * perso);

/**
 * @author Léo
 * @brief Constructeur pour un objet de type Kit de Soins.
 * @param renderer Le moteur de rendu pour charger la texture associée.
 * @return objet_t L'instance de l'objet initialisée.
 */
objet_t creer_kit_de_soins(SDL_Renderer * renderer);

/**
 * @author Léo
 * @brief Libère proprement les ressources d'un objet (texture et nom).
 * @param objet Pointeur vers l'objet à détruire.
 */
void detruire_objet(objet_t * objet);

/**
 * @author Léo
 * @brief Remplit l'attribut texture d'un objet en fonction de son type.
 * @param objet Pointeur vers l'objet
 * @param renderer Le moteur de rendu pour charger la texture associée.
 */
void remplir_texture_objet(objet_t * objet, SDL_Renderer * renderer);

/**
 * @author Léo
 * @brief Crée une texture pour un objet de type kit de soins.
 * @param renderer Le moteur de rendu pour charger la texture associée.
 * @return Un pointeur sur un SDL_Texture correspondant à un kit de soins.
 */
SDL_Texture * texture_kit_de_soins(SDL_Renderer * renderer);

/**
 * @author Léo
 * @brief Utilise un objet.
 * @param objet Pointeur vers l'objet à utiliser.
 * @param perso Le personnage qui utilise l'objet.
 */
void utiliser_objet(objet_t * objet, perso_t * perso);

#endif
