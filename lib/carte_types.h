#ifndef CARTE_TYPES_H
#define CARTE_TYPES_H

/* Définitions */
#define TAILLE_CARTE 75
#define NB_COORD 4
#define INFINI 5000

/* Structures */
typedef enum {
    TERRE,
    EAU,
    DESERT,
    NEIGE,
    FORET,
    NB_BIOMES
} biome_t;

typedef enum {
    ARBRES,
    MONTAGNES,
    BASSIN_EAU,
    CACTUS,
    PAS_DE_TERRAIN,
    NB_TERRAINS,
} terrain_t;

typedef enum {
    PAS_DE_BATIMENT,
    CAMPEMENT,
    MAGASIN,
    TOUR_DU_BOSS,
    TOMBE
} type_bat_t;


// Type des sanctuaires
typedef enum {
    PAS_DE_SANCTUAIRE, 
    PUISSANCE,
    RAPIDITE,
    INTELLIGENCE,
    EXPERIENCE
} type_sanc_t;

typedef struct {
    type_bat_t type;
} batiment_t;

typedef struct case_s {
    type_sanc_t sanctuaires;
    biome_t biome;
    terrain_t terrain;
    batiment_t batiment;
    monstre_t * monstre;
    int estVisible;
} case_t;

typedef struct {
    int x, y;
} coordonnee_t;

typedef enum {
    HAUT,
    BAS,
    GAUCHE,
    DROITE,
    NB_DIRECTIONS
} direction_t;

#endif