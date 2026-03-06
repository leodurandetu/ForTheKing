#include "../lib/affichage_infos.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/perso.h"
#include <string.h> 


void get_info_personnage(perso_t *p, const char* attr, char* buf) {
    if      (strcmp(attr, "Force") == 0)            sprintf(buf, "Force : %d", p->force);
    else if (strcmp(attr, "Sante") == 0)            sprintf(buf, "PV : %d", p->sante);
    else if (strcmp(attr, "Intelligence") == 0)     sprintf(buf, "Intelligence : %d", p->intelligence);
    else if (strcmp(attr, "Rapidite") == 0)         sprintf(buf, "Vitesse : %d", p->rapidite);
    else if (strcmp(attr, "Evasion") == 0)          sprintf(buf, "Esquive : %d", p->evasion);
    else if (strcmp(attr, "Pts_deplacements") == 0) sprintf(buf, "Points de déplacements : %d", p->pts_deplacements);
    else if (strcmp(attr, "Niveau") == 0)           sprintf(buf, "Niveau : %d", p->niveau);
    else if (strcmp(attr, "Exp") == 0)              sprintf(buf, "XP : %d", p->exp);
    else strcpy(buf, "Inconnu");
}


void mise_a_jour_texte(char * contenu, SDL_Renderer * renderer, TTF_Font* police, SDL_Surface ** texte, SDL_Texture ** texte_tex, SDL_Rect * txtDestRect) {
    SDL_Color couleurNoire = {0 , 0, 0, 255};

    /* Libération de l'ancienne texture si elle existe */
    if (*texte_tex != NULL) {
        SDL_DestroyTexture(*texte_tex);
        *texte_tex = NULL;
    }

    /* Libération de l'ancienne surface si elle existe */
    if (*texte != NULL) {
        SDL_FreeSurface(*texte);
        *texte = NULL;
    }

    /* Création de la surface à partir du texte 'contenu' */
    *texte = TTF_RenderUTF8_Blended(police, contenu, couleurNoire);

    if (!(*texte)) {
        fprintf(stderr, "Erreur a la creation du texte : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    
    *texte_tex = SDL_CreateTextureFromSurface(renderer, *texte);

    if (!(*texte_tex)) {
        fprintf(stderr, "Erreur a la creation du rendu du texte : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    int lFenetre, hFenetre;
    SDL_GetRendererOutputSize(renderer, &lFenetre, &hFenetre);

    SDL_FreeSurface(*texte); 
    *texte = NULL;
    
    /* Calcul de la position et de la taille */
    txtDestRect->y = 10;
    SDL_QueryTexture(*texte_tex, NULL, NULL, &(txtDestRect->w), &(txtDestRect->h));
    txtDestRect->x = (lFenetre - txtDestRect->w) / 2;
}
