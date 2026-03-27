#ifndef CARTE_TYPES_H
#define CARTE_TYPES_H

/**
 * @file carte_types.h
 * @author Léo, Saandi & Massoud
 * @brief Définitions des structures et énumérations fondamentales de la carte.
 */

/* --- Définitions globales --- */
#define TAILLE_CARTE 75
#define NB_COORD 4
#define INFINI 5000

/** * @author Léo, Saandi & Massoud
 * @brief Types de biomes disponibles sur la carte.
 * @details Détermine la texture de fond de la case (sol).
 */
typedef enum {
    TERRE,
    EAU,
    DESERT,
    NEIGE,
    FORET,
    NB_BIOMES
} biome_t;

/** * @author Léo, Saandi & Massoud
 * @brief Types d'obstacles ou éléments de décor superposés au biome.
 */
typedef enum {
    ARBRES,
    MONTAGNES,
    BASSIN_EAU,
    CACTUS,
    PAS_DE_TERRAIN,
    NB_TERRAINS,
} terrain_t;

/** * @author Léo, Saandi & Massoud
 * @brief Types de bâtiments interactifs présents sur la carte.
 */
typedef enum {
    PAS_DE_BATIMENT,
    CAMPEMENT,
    MAGASIN,
    TOUR_DU_BOSS,
    TOMBE
} type_bat_t;

/** * @author Léo, Saandi & Massoud
 * @brief Types de sanctuaires offrant des bonus aux statistiques.
 */
typedef enum {
    PAS_DE_SANCTUAIRE, 
    PUISSANCE,
    RAPIDITE,
    INTELLIGENCE,
    EXPERIENCE
} type_sanc_t;

/** * @author Léo, Saandi & Massoud
 * @brief Structure représentant un bâtiment.
 */
typedef struct {
    type_bat_t type; /**< Type spécifique du bâtiment. */
} batiment_t;

/** * @author Léo, Saandi & Massoud
 * @brief Structure principale d'une cellule de la grille (la case).
 * @details Regroupe toutes les informations d'occupation, d'environnement et d'affichage.
 */
typedef struct case_s {
    type_sanc_t sanctuaires; /**< Sanctuaire présent sur la case. */
    biome_t biome;           /**< Type de biome (fond). */
    terrain_t terrain;       /**< Type de terrain (obstacle). */
    batiment_t batiment;     /**< Bâtiment présent sur la case. */
    monstre_t * monstre;     /**< Pointeur vers un monstre présent, NULL si la case est vide de monstre. */
    int estVisible;          /**< État pour le brouillard de guerre (1 pour visible, 0 pour caché). */
} case_t;

/** * @author Léo, Saandi & Massoud
 * @brief Structure de positionnement dans un plan 2D.
 */
typedef struct {
    int x; /**< Position horizontale. */
    int y; /**< Position verticale. */
} coordonnee_t;

/** * @author Léo, Saandi & Massoud
 * @brief Directions de déplacement possibles dans le jeu.
 */
typedef enum {
    HAUT,
    BAS,
    GAUCHE,
    DROITE,
    NB_DIRECTIONS
} direction_t;

#endif
