#ifndef SANCTUAIRE_MENU_H
#define SANCTUAIRE_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/carte_types.h"
#include "../lib/carte.h"
#include "../lib/perso.h"


/* Résultat du menu sanctuaire :
 * le joueur a saisi le bonus ou est parti */
typedef enum {
    SANCTUAIRE_SAISI,
    SANCTUAIRE_QUITTE
} resultat_sanctuaire_t;

/* Affiche le menu du sanctuaire à droite de la case cliquée.
 * Retourne SANCTUAIRE_SAISI si le joueur a pris le bonus,
 * SANCTUAIRE_QUITTE sinon. */
resultat_sanctuaire_t afficher_menu_sanctuaire(
    SDL_Renderer *renderer,
    TTF_Font *font,
    perso_t *perso,
    case_t **carte,
    int sanc_x, int sanc_y,
    int tailleCase
);

#endif