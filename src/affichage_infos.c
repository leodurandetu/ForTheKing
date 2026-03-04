#include "../lib/affichage_infos.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void mise_a_jour_texte(char * contenu, SDL_Renderer * renderer, TTF_Font* police, SDL_Surface ** texte, SDL_Texture ** texte_tex, SDL_Rect * txtDestRect) {
    SDL_Color couleurNoire = {0 , 0, 0};

    if (*texte_tex != NULL) {
        SDL_DestroyTexture(*texte_tex);
        *texte_tex = NULL;
    }

    if (*texte != NULL) {
        SDL_FreeSurface(*texte);
        *texte = NULL;
    }

    *texte = TTF_RenderUTF8_Blended ( police , contenu, couleurNoire );

    if (! (*texte) ) {
        fprintf ( stderr , " Erreur a la creation du texte : %s\n", SDL_GetError ());
        exit ( EXIT_FAILURE );
    }

    SDL_SetRenderDrawColor ( renderer , 0, 0, 0, 255);
    *texte_tex = SDL_CreateTextureFromSurface ( renderer , *texte );

    if (! (*texte_tex) ){
        fprintf ( stderr , " Erreur a la creation du rendu du texte : %s\n", SDL_GetError ());
        exit ( EXIT_FAILURE );
    }

    int lFenetre, hFenetre;
    SDL_GetRendererOutputSize(renderer, &lFenetre, &hFenetre);

    SDL_FreeSurface ( *texte ); /* on a la texture , plus besoin du texte */
    *texte = NULL;
    
    /* Position ou sera mis le texte dans la fenetre */
    txtDestRect->y = 10;
    SDL_QueryTexture ( *texte_tex , NULL , NULL , &( txtDestRect->w), &( txtDestRect->h ));
    txtDestRect->x = (lFenetre - txtDestRect->w) / 2;

}