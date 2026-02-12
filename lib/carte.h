#include <SDL2/SDL.h>

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
    PAS_DE_BATIMENT,
    CAMPEMENT,
    MAGASIN
} type_bat_t;

typedef struct {
    
    type_bat_t type;

} batiment_t;

typedef struct case_s {

    biome_t biome;
    batiment_t batiment;
    int estVisible;

} case_t;

typedef struct{
    int x,y;
}coordonnee_t;

#define TAILLE_CARTE 20
#define NB_COORD 4

/* Prototypes des fonctions */
void afficher_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void afficher_carte_sdl(SDL_Renderer * renderer,
    case_t carte[TAILLE_CARTE][TAILLE_CARTE],
    SDL_Texture * textures_cases[NB_BIOMES],
    int taille_case
);
void init_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void generer_eau(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void generer_biomes(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
int test_etat_case(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y);
void remplir_zone(case_t carte[TAILLE_CARTE][TAILLE_CARTE], biome_t biome, coordonnee_t dep, coordonnee_t arr);
