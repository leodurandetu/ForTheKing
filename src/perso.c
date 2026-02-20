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

void afficher_personnage(SDL_Renderer *renderer, SDL_Texture *texture, perso_t *pers, int tailleCase)
{
    int lFenetre, hFenetre;
    SDL_GetRendererOutputSize(renderer, &lFenetre, &hFenetre);

    int taillePerso = tailleCase / 2;
    int marge = 2;
    int epaisseurBord = 2;

    SDL_Rect rectFond;
    rectFond.w = taillePerso + (marge * 2);
    rectFond.h = taillePerso + (marge * 2);
    rectFond.x = (lFenetre / 2) - (rectFond.w / 2);
    rectFond.y = (hFenetre / 2) - (rectFond.h / 2);

    SDL_Rect rectBordure = {
        rectFond.x - epaisseurBord,
        rectFond.y - epaisseurBord,
        rectFond.w + (epaisseurBord * 2),
        rectFond.h + (epaisseurBord * 2)
    };

    /* Bordure Noire */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rectBordure);

    /* Rectangle Blanc */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rectFond);

    SDL_Rect dest;

    dest.x = (lFenetre / 2) - (taillePerso / 2);
    dest.y = (hFenetre / 2) - (taillePerso / 2);
    
    dest.w = taillePerso;
    dest.h = taillePerso;

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}
