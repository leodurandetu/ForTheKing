/* Structures */
typedef enum {
    TERRE,
    EAU,
    DESERT
} biome_t;

typedef struct case_s {

    biome_t biome;

} case_t;

#define TAILLE_CARTE 500

case_t carte[TAILLE_CARTE][TAILLE_CARTE];

/* Prototypes des fonctions */
void afficher_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void generer_eau(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void generer_biomes(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);