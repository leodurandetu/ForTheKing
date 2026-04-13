#ifndef MONSTRE_H
#define MONSTRE_H

/**
 * @file monstre.h
 * @author Léo, Saandi & Massoud
 * @brief Définition des ennemis et de leurs statistiques de combat.
 */

/** * @author Léo, Saandi & Massoud
 * @brief Types de monstres disponibles dans le jeu.
 */
typedef enum {
    SQUELETTE,
    TROLL,
    YETI,
    VER_GEANT,
    BOSS_FINAL
} type_monstre_t;

/** * @author Léo, Saandi & Massoud
 * @brief Structure représentant un monstre et ses statistiques de combat.
 * @details Cette structure est utilisée tant pour l'affichage sur la carte 
 * que pour les calculs lors des phases de combat.
 */
typedef struct {
    type_monstre_t type; /**< Catégorie du monstre. */
    int x, y;            /**< Coordonnées actuelles sur la carte. */
    
    /* Statistiques de combat */
    int degats;          /**< Dégâts de base infligés. */
    int force;           /**< Multiplicateur de dégâts physiques. */
    int sante;           /**< Points de vie actuels. */
    int sante_max;       /**< Points de vie maximum. */
    int intelligence;    /**< Influence les capacités magiques ou spéciales. */
    int rapidite;        /**< Détermine l'ordre de passage en combat. */
    int evasion;         /**< Chance d'esquiver une attaque. */
    int niveau;          /**< Niveau du monstre (influence la progression des statistiques). */
    
} monstre_t;

/* --- Prototypes des fonctions --- */

/**
 * @author Léo
 * @brief Alloue et initialise un nouveau monstre avec des statistiques de base.
 * @param type Le type de monstre à créer (Squelette ou Troll).
 * @param x Coordonnée X de départ sur la grille.
 * @param y Coordonnée Y de départ sur la grille.
 * @return monstre_t* Un pointeur vers le monstre créé, ou NULL en cas d'erreur.
 */
monstre_t * creer_monstre_aleatoire(type_monstre_t type, int x, int y);

/**
 * @author Léo
 * @brief Alloue et initialise un nouveau monstre de type boss final.
 * @return monstre_t* Un pointeur vers le monstre créé, ou NULL en cas d'erreur.
 */
monstre_t * creer_boss_final();

#endif
