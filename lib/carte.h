#ifndef CARTE_H
#define CARTE_H

#include <SDL2/SDL.h>
#include <math.h>
#include "../lib/monstre.h"
#include "../lib/perso.h"

/* Définitions */
#define TAILLE_CARTE 100
#define NB_COORD 4
#define VRAI 1
#define FAUX 0
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
} Terrain_t;

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
    Terrain_t terrain;
    batiment_t batiment;
    monstre_t *monstre;
    int estVisible;
} case_t;

typedef struct {
    int x, y;
} coordonnee_t;

/* Prototypes des fonctions de logique */
void init_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void generer_eau(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void generer_biomes(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void placer_batiments(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void remplir_zone(case_t carte[TAILLE_CARTE][TAILLE_CARTE], biome_t biome, coordonnee_t dep, coordonnee_t arr);
int test_etat_case(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y);
void afficher_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]); // Console
void devoiler_brouillard_rayon(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y, int rayon);
void liberer_memoire_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void placer_monstres(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void ajout_obstacles(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
int deplacement_possible(case_t carte[TAILLE_CARTE][TAILLE_CARTE], perso_t *perso, int x, int y);
int case_occupee(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y);
void coords_case_libre(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int *x, int *y);
void souris_vers_case(int mouseX, int mouseY, int *carte_x, int *carte_y, int tailleCase, perso_t *perso, SDL_Renderer *renderer);
int chemin_valide(case_t[TAILLE_CARTE][TAILLE_CARTE], int xDepart, int yDepart, int xCible, int yCible, int pts_deplacement_max, perso_t *perso, int *distance);

/* Prototypes des fonctions de rendu SDL2 */
void afficher_hex_texture(SDL_Renderer* renderer, float cx, float cy, float rayon, SDL_Texture* texture, SDL_Color couleur);
void dessiner_contour_double_dore(SDL_Renderer* r, float cx, float cy, float rayon, int alpha);
void dessiner_contour_ftk(SDL_Renderer* r, float cx, float cy, float rayon, int epaisseur, SDL_Color couleur);
void afficher_hex_couleur(SDL_Renderer* renderer, float cx, float cy, float rayon, SDL_Color couleur);
void afficher_carte_sdl(SDL_Renderer * renderer,
    case_t carte[TAILLE_CARTE][TAILLE_CARTE],
    SDL_Texture * textures_cases[NB_BIOMES],
    SDL_Texture * textures_obstacles[4], 
    SDL_Texture * texture_brouillard,
    SDL_Texture * texture_monstre,
    SDL_Texture * texture_campement,
    int tailleCase,
    int persX, int persY,
    int case_selection_x, int case_selection_y,
    perso_t *perso);

#endif 
