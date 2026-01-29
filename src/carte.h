/* Structures */
typedef enum {
    TERRE,
    EAU,
    DESERT
} biome_t;

typedef enum {
    RIEN,
    CAMPEMENT,
    MAGASIN
} type_bat_t;

typedef struct {
    
    type_bat_t type;

} batiment_t;

typedef struct case_s {

    biome_t biome;
    batiment_t batiment;

} case_t;

#define TAILLE_CARTE 500

case_t carte[TAILLE_CARTE][TAILLE_CARTE];

/* Prototypes des fonctions */
void afficher_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void init_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void generer_eau(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void generer_biomes(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
case_t test_etat_case(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y);
