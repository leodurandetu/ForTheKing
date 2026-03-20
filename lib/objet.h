#ifndef OBJET_H
#define OBJET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct perso_s perso_t;

/* Structures et Enums */

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
objet_t creer_kit_de_soins(SDL_Renderer * renderer);
void detruire_objet(objet_t * objet);

#endif