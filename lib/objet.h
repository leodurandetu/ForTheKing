#ifndef OBJET_H
#define OBJET_H

#include <SDL2/SDL.h>
#include "perso.h"

/* Structures */

typedef enum {
    KIT_DE_SOINS
} objet_type_t;

typedef struct {

    objet_type_t type;
    char * nom;
    SDL_Texture * texture;
    int quantite;

    void (*utiliser)(perso_t *);

} objet_t;

/* Prototypes des fonctions */

void utiliser_kit_de_soins(perso_t * perso);
objet_t creer_kit_de_soins();

#endif