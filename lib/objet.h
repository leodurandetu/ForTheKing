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
 * spécifique lors de l'utilisation de l'objet (Polymorphisme en C).
 */
typedef struct {
    objet_type_t type;      /**< Identifiant du type d'objet. */
    char * nom;             /**< Nom affiché dans l'interface. */
    SDL_Texture * texture;  /**< Icône visuelle de l'objet. */
    int quantite;           /**< Nombre d'exemplaires possédés. */

    /** * @author Léo
     * @brief Pointeur vers la fonction d'effet de l'objet.
     * @param perso Le personnage qui subit l'effet lors de l'utilisation.
     */
    void (*utiliser)(perso_t *);

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

#endif
