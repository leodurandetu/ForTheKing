#ifndef COMBAT_TYPES_H
#define COMBAT_TYPES_H

#include "perso.h"
#include "monstre.h"
#include "math.h"
#include "carte_types.h"

/* Structures et Enums */

typedef enum {
    TOUR_JOUEUR,
    TOUR_MONSTRE
}tour_t;

typedef enum {
    PAS_DE_VAINQUEUR,
    JOUEUR_VAINQUEUR,
    MONSTRE_VAINQUEUR
} vainqueur_t;

typedef struct {

    perso_t * perso;
    monstre_t * monstre;
    tour_t tour;
    SDL_Window * pFenetre;
    SDL_Renderer * renderer;

    SDL_Texture * texture_monstre;
    SDL_Texture * texture_perso;
    SDL_Texture * texture_fond_ecran;

    SDL_Texture * texture_attaque_legere;
    SDL_Texture * texture_attaque_lourde;

    SDL_Rect bouton_leger;
    SDL_Rect bouton_lourd;

    int survole_bouton_leger;
    int survole_bouton_lourd;

    TTF_Font * font;

    SDL_Texture * texture_texte_leger;
    SDL_Texture * texture_texte_lourd;

    SDL_Rect rect_texte_leger;
    SDL_Rect rect_texte_lourd;

    biome_t biome;

} combat_t;

#endif