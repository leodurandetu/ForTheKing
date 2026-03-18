#ifndef COMBAT_H
#define COMBAT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/perso.h"
#include "../lib/monstre.h"
#include "../lib/carte.h"
#include "math.h"

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

} combat_t;

/* Prototypes des fonctions */

/* Leo */
extern void ouvrir_fenetre_combat(combat_t ** combat, case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
extern void maj_affichage_fenetre_combat(combat_t * combat);
extern void detruire_fenetre_combat(combat_t ** combat, case_t carte[TAILLE_CARTE][TAILLE_CARTE], vainqueur_t vainqueur);
/* Massoud */
extern void dessiner_icone(combat_t *combat, SDL_Rect *r_icone, int survole, SDL_Texture *texture_arme, SDL_Texture *tex_texte, SDL_Rect *rect_texte);
extern tour_t changer_tour(combat_t *combat);
extern void attaque_lourde(combat_t *combat);
extern void attaque_legere(combat_t *combat);
extern void attaque_legere_monstre(combat_t *combat);
extern void attaque_legere_monstre(combat_t *combat);
extern void choix_attaque_monstre(combat_t *combat);

#endif
