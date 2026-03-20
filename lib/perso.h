#ifndef PERSO_H
#define PERSO_H

#include <SDL2/SDL.h>
#include "inventaire.h"

/* Structures et Enums */

typedef enum {

    BRUTE,
    CHASSEUR,
    ASSASSIN,
    MAGE

} perso_type_t;

typedef struct perso_s {

    int x, y;
    int degats;
    int force;
    int sante;
    int sante_max;
    int intelligence;
    int rapidite;
    int evasion;
    int pts_deplacements;

    int niveau;
    int exp;

    inventaire_t inventaire;

    perso_type_t type;

} perso_t;

/* Prototypes des fonctions*/

/* Massoud */
void afficher_personnage(SDL_Renderer * renderer, SDL_Texture * texture_perso, perso_t * perso, int tailleCase);
/* Leo */
perso_t * init_perso(perso_type_t persoType, int xDepart, int yDepart);
/* Leo */
void detruire_perso(perso_t ** perso);
/* Massoud */
int get_pers_movements_points(perso_t *perso);
/* Leo */
void restaurer_points_deplacements(perso_t * perso);
void redonner_un_pv(perso_t * perso);

#endif
