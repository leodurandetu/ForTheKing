#ifndef COMBAT_H
#define COMBAT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "perso.h"
#include "monstre.h"
#include "carte.h"
#include "math.h"
#include "affichage_combat.h"
#include "combat_types.h"

/* Prototypes des fonctions */

/* Leo */
extern void ouvrir_fenetre_combat(combat_t ** combat, case_t carte[TAILLE_CARTE][TAILLE_CARTE]);
extern void detruire_fenetre_combat(combat_t ** combat, case_t carte[TAILLE_CARTE][TAILLE_CARTE], vainqueur_t vainqueur);
/* Massoud */
extern tour_t changer_tour(combat_t *combat);
extern void attaque_lourde(combat_t *combat);
extern void attaque_legere(combat_t *combat);
extern void attaque_legere_monstre(combat_t *combat);
extern void attaque_legere_monstre(combat_t *combat);
extern void choix_attaque_monstre(combat_t *combat);
/* Saandi*/
combat_t* creer_combat(perso_t *perso, monstre_t *monstre);
#endif
