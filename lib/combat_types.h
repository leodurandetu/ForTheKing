#ifndef COMBAT_TYPES_H
#define COMBAT_TYPES_H

#include "perso.h"
#include "monstre.h"
#include "math.h"
#include "carte_types.h"

/**
 * @file combat_types.h
 * @author Massoud, Léo & Saandi
 * @brief Définitions des types et structures pour la gestion des combats.
 */

/** * @author Massoud, Léo & Saandi
 * @brief Détermine à qui appartient le tour actuel durant le combat.
 */
typedef enum {
    TOUR_JOUEUR,    /**< Le joueur peut choisir une action. */
    TOUR_MONSTRE    /**< L'IA du monstre s'exécute. */
} tour_t;

/** * @author Massoud, Léo & Saandi
 * @brief État de l'issue du combat.
 */
typedef enum {
    PAS_DE_VAINQUEUR, /**< Le combat est toujours en cours. */
    JOUEUR_VAINQUEUR, /**< Le joueur a gagné (monstre tué). */
    MONSTRE_VAINQUEUR /**< Le monstre a gagné (joueur tué). */
} vainqueur_t;

/** * @author Massoud, Léo & Saandi
 * @brief Structure de contexte pour une instance de combat.
 * @details Regroupe les pointeurs vers les entités, les ressources SDL 
 * et les états d'interface nécessaires au rendu et à la logique de la scène de combat.
 */
typedef struct {
    /* --- Entités --- */
    perso_t * perso;        /**< Référence vers la structure du joueur. */
    monstre_t * monstre;    /**< Référence vers la structure de l'adversaire. */
    tour_t tour;            /**< Identifiant du tour actif (joueur ou monstre). */

    /* --- Système de rendu --- */
    SDL_Window * pFenetre;  /**< Fenêtre SDL dédiée à l'affichage du combat. */
    SDL_Renderer * renderer;/**< Moteur de rendu associé à la fenêtre de combat. */

    /* --- Ressources graphiques --- */
    SDL_Texture * texture_monstre;     /**< Sprite de l'ennemi. */
    SDL_Texture * texture_perso;       /**< Sprite du joueur. */
    SDL_Texture * texture_fond_ecran;  /**< Texture d'arrière-plan adaptée au biome actuel. */

    /* --- Boutons d'action --- */
    SDL_Texture * texture_attaque_legere; /**< Visuel pour le bouton d'attaque rapide. */
    SDL_Texture * texture_attaque_lourde; /**< Visuel pour le bouton d'attaque puissante. */
    SDL_Rect bouton_leger;                /**< Zone de collision/affichage du bouton léger. */
    SDL_Rect bouton_lourd;                /**< Zone de collision/affichage du bouton lourd. */

    /* --- États de l'interface --- */
    int survole_bouton_leger; /**< Indicateur de survol (1 si la souris est sur le bouton léger). */
    int survole_bouton_lourd; /**< Indicateur de survol (1 si la souris est sur le bouton lourd). */

    /* --- Texte et Police --- */
    TTF_Font * font;                   /**< Police de caractères pour l'interface de combat. */
    SDL_Texture * texture_texte_leger; /**< Texture générée pour le texte du bouton léger. */
    SDL_Texture * texture_texte_lourd; /**< Texture générée pour le texte du bouton lourd. */
    SDL_Rect rect_texte_leger;         /**< Position et taille du texte léger. */
    SDL_Rect rect_texte_lourd;         /**< Position et taille du texte lourd. */

    /* --- Environnement --- */
    biome_t biome; /**< Type de biome récupéré de la carte pour le décor. */

} combat_t;

#endif
