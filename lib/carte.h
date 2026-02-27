#ifndef CARTE_H
#define CARTE_H

#include <SDL2/SDL.h>
#include <math.h>

/* Définitions */
#define TAILLE_CARTE 50
#define NB_COORD 4

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

typedef struct {
    int x, y;
} coordonnee_t;

/* Prototypes des fonctions de logique */
void init_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void generer_eau(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void generer_biomes(case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
void remplir_zone(case_t carte[TAILLE_CARTE][TAILLE_CARTE], biome_t biome, coordonnee_t dep, coordonnee_t arr);
int test_etat_case(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y);
void afficher_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]); // Console

/* Prototypes des fonctions de rendu SDL2 */
void afficher_hex_texture(SDL_Renderer* renderer, float cx, float cy, float rayon, SDL_Texture* texture, SDL_Color couleur);
void dessiner_contour_double_dore(SDL_Renderer* r, float cx, float cy, float rayon, int alpha);
void dessiner_contour_ftk(SDL_Renderer* r, float cx, float cy, float rayon, int epaisseur, SDL_Color couleur);
void afficher_hex_couleur(SDL_Renderer* renderer, float cx, float cy, float rayon, SDL_Color couleur);
void afficher_carte_sdl(SDL_Renderer * renderer,
    case_t carte[TAILLE_CARTE][TAILLE_CARTE],
    SDL_Texture * textures_cases[NB_BIOMES], 
    int tailleCase,
    int persX, int persY,
    int case_selection_x, int case_selection_y,
    int perso_selectionne);

#endif 
