#ifndef COMBAT_H
#define COMBAT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/perso.h"
#include "../lib/monstre.h"
#include "../lib/combat.h"
#include "math.h"

typedef struct {

    perso_t * perso;
    monstre_t * monstre;
    SDL_Window * pFenetre;
    SDL_Renderer * renderer;

    SDL_Texture * texture_monstre;
    SDL_Texture * texture_perso;
    SDL_Texture * texture_fond_ecran;

    SDL_Texture * texture_attaque_legere;
    SDL_Texture * texture_attaque_lourde; // MAssoud : j'ai ajouté pour les img des attaques

    SDL_Rect bouton_leger;
    SDL_Rect bouton_lourd;

    int hover_bouton_leger;
    int hover_bouton_lourd;

    TTF_Font * font;

} combat_t;

/* Prototypes des fonctions */

/* Leo */
extern void ouvrir_fenetre_combat(combat_t ** combat);
extern void maj_affichage_fenetre_combat(combat_t * combat);
extern void detruire_fenetre_combat(combat_t ** combat);
extern void dessiner_icone(combat_t *combat, SDL_Rect *r_icone, 
                        int hover, const char *texte, 
                        SDL_Texture *texture_arme);

#endif
