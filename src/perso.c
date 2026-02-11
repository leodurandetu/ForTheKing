#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../lib/perso.h"
#include "../lib/carte.h"

perso_t * init_perso(perso_type_t persoType)
{
    perso_t * perso = malloc(sizeof(perso_t));

    perso->x = TAILLE_CARTE / 2;
    perso->y = TAILLE_CARTE / 2;

    perso->type = persoType;

    switch (persoType)
    {

        case BRUTE:
            perso->force = 32;
            perso->sante = 50;
            perso->intelligence = 4;
            perso->rapidite = 8;
            perso->evasion = 3;
            break;

        case CHASSEUR:
            perso->force = 20;
            perso->sante = 40;
            perso->intelligence = 13;
            perso->rapidite = 19;
            perso->evasion = 15;
            break;

        case ASSASSIN:
            perso->force = 16;
            perso->sante = 32;
            perso->intelligence = 13;
            perso->rapidite = 24;
            perso->evasion = 24;
            break;

        case MAGE:
            perso->force = 12;
            perso->sante = 36;
            perso->intelligence = 26;
            perso->rapidite = 17;
            perso->evasion = 8;
            break;

    }

    return perso;
}

void afficher_personnage(SDL_Renderer *renderer,
                         SDL_Texture *texture,
                         perso_t *pers)
{

    int lFenetre, hFenetre;
    int tailleCase = 50;

    SDL_GetRendererOutputSize(renderer, &lFenetre, &hFenetre);

    SDL_Rect dest;

    dest.y = pers->x * tailleCase + (hFenetre / 2) - (TAILLE_CARTE * tailleCase / 2);
    dest.x = pers->y * tailleCase + (lFenetre / 2) - (TAILLE_CARTE * tailleCase / 2);

    dest.w = tailleCase;
    dest.h = tailleCase;

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}
