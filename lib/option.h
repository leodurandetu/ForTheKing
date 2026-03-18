#ifndef OPTION_AF_H
#define OPTION_AF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/carte.h"
#include "../lib/perso.h"
#include "../lib/monstre.h"
#include "../lib/combat.h"

void dessiner_bouton(SDL_Renderer *renderer, TTF_Font *font,
                     SDL_Rect bouton, const char *texte,
                     SDL_Color couleur);
void dessiner_monstre(SDL_Renderer *renderer);

/*void ouvrir_fenetre_choix(monstre_t *monstre, perso_t *perso, combat_t* c, void (*fenetre_combat)(combat_t**));*/

int ouvrir_fenetre_choix(combat_t* combat);
int monstre_dans_case_cliquee(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y);

#endif