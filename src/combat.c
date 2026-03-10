#include "../lib/combat.h"

void ouvrir_fenetre_combat(combat_t ** combat) {
    (*combat)->pFenetre = (SDL_CreateWindow("For The King - Combat", 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));

    if (!((*combat)->pFenetre)) {
        fprintf(stderr, "Erreur fenêtre : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Création du moteur de rendu (accéléré par la carte graphique)
    (*combat)->renderer = (SDL_CreateRenderer((*combat)->pFenetre, -1, SDL_RENDERER_ACCELERATED));
    if (!((*combat)->renderer)) {
        fprintf(stderr, "Erreur renderer : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Chargement du sprite du personnage
    (*combat)->texture_perso = NULL;
    SDL_Surface *img_perso = IMG_Load("img/mage.png");
    if (img_perso) {
        // Rend la couleur blanche (255, 255, 255) transparente
        SDL_SetColorKey(img_perso, SDL_TRUE, SDL_MapRGB(img_perso->format, 255, 255, 255));
        (*combat)->texture_perso = (SDL_CreateTextureFromSurface((*combat)->renderer, img_perso));
        SDL_SetTextureBlendMode((*combat)->texture_perso, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_perso);
    }

    // Chargement du sprite du monstre (unique pour l'instant)
    (*combat)->texture_monstre = NULL;
    SDL_Surface *img_monstre = IMG_Load("img/squelette.png");
    if (img_monstre) {
        (*combat)->texture_monstre = (SDL_CreateTextureFromSurface((*combat)->renderer, img_monstre));
        SDL_SetTextureBlendMode((*combat)->texture_monstre, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_monstre);
    }
    
    // Chargement du sprite du fond d'écran
    (*combat)->texture_fond_ecran = NULL;
    SDL_Surface *img_fond_foret = IMG_Load("img/fond_foret.png");
    if (img_fond_foret) {
        (*combat)->texture_fond_ecran = (SDL_CreateTextureFromSurface((*combat)->renderer, img_fond_foret));
        SDL_SetTextureBlendMode((*combat)->texture_fond_ecran, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_fond_foret);
    }

    int running = 1;
    int majAffichage = 1;

    while (running) {
        SDL_Event e;

        while (SDL_PollEvent(&e)) {

            switch (e.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_WINDOWEVENT:

                    if (e.window.event == SDL_WINDOWEVENT_RESIZED
                        || e.window.event == SDL_WINDOWEVENT_EXPOSED) {
                        majAffichage = 1;
                    }

                    if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                        running = 0;
                    }

                    break;

            }

        }

        if (majAffichage) {
            maj_affichage_fenetre_combat(*combat);
            majAffichage = 0;
        }

    }

    detruire_fenetre_combat(combat);
}

void maj_affichage_fenetre_combat(combat_t * combat) {
    SDL_SetRenderDrawColor((combat->renderer), 255, 255, 255, 255);
    SDL_RenderClear((combat->renderer));

    int lFenetre, hFenetre;
    SDL_GetRendererOutputSize((combat->renderer), &lFenetre, &hFenetre);

    // Activation du mode BLEND pour que l'alpha fonctionne
    SDL_SetRenderDrawBlendMode((combat->renderer), SDL_BLENDMODE_BLEND);

    if ((combat->texture_fond_ecran) != NULL) {
        SDL_Rect rect;

        rect.w = lFenetre;
        rect.h = hFenetre;
        rect.x = 0;
        rect.y = (hFenetre / 2) - (rect.h / 2);

        SDL_RenderCopy((combat->renderer), (combat->texture_fond_ecran), NULL, &rect);
    }

    if ((combat->texture_perso) != NULL) {
        SDL_Rect rect;

        float echelle = 0.2f;
        int taille = lFenetre * echelle;

        rect.w = taille;
        rect.h = taille;
        rect.x = 5;
        rect.y = (hFenetre / 2) - (rect.h / 2);

        SDL_RenderCopy((combat->renderer), (combat->texture_perso), NULL, &rect);
    }

    if ((combat->texture_monstre) != NULL) {
        SDL_Rect rect;

        float echelle = 0.2f;
        int taille = lFenetre * echelle;

        rect.w = taille;
        rect.h = taille;
        rect.x = lFenetre - 5 - rect.w;
        rect.y = (hFenetre / 2) - (rect.h / 2);

        SDL_RenderCopy((combat->renderer), (combat->texture_monstre), NULL, &rect);
    }

    SDL_RenderPresent((combat->renderer));
}

void detruire_fenetre_combat(combat_t ** combat) {
    
    if (combat != NULL && *combat != NULL) {

        if ((*combat)->texture_monstre != NULL) {
            SDL_DestroyTexture((*combat)->texture_monstre);
            (*combat)->texture_monstre = NULL;
        }

        if ((*combat)->texture_perso != NULL) {
            SDL_DestroyTexture((*combat)->texture_perso);
            (*combat)->texture_perso = NULL;
        }

        if ((*combat)->texture_fond_ecran != NULL) {
            SDL_DestroyTexture((*combat)->texture_fond_ecran);
            (*combat)->texture_fond_ecran = NULL;
        }

        if (((*combat)->renderer) != NULL) {
            SDL_DestroyRenderer((*combat)->renderer);
            ((*combat)->renderer) = NULL;
        }

        if (((*combat)->pFenetre) != NULL) {
            SDL_DestroyWindow((*combat)->pFenetre);
            ((*combat)->pFenetre) = NULL;
        }

        free(*combat);
        *combat = NULL;
    }

}