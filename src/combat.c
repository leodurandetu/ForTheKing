#include "../lib/combat.h"

static void dessiner_bouton(combat_t *combat, SDL_Rect *r, int hover, const char *texte)
{
    SDL_Color fond = hover ? (SDL_Color){200,200,200,255} : (SDL_Color){150,150,150,255};

    SDL_SetRenderDrawColor(combat->renderer, fond.r, fond.g, fond.b, 255);
    SDL_RenderFillRect(combat->renderer, r);

    SDL_SetRenderDrawColor(combat->renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(combat->renderer, r);

    if (!combat->font) return;

    SDL_Color noir = {0, 0, 0, 255};

    SDL_Surface *surf = TTF_RenderUTF8_Blended(combat->font, texte, noir);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(combat->renderer, surf);

    SDL_Rect txt;
    txt.w = surf->w;
    txt.h = surf->h;
    txt.x = r->x + (r->w - txt.w)/2;
    txt.y = r->y + (r->h - txt.h)/2;

    SDL_FreeSurface(surf);

    SDL_RenderCopy(combat->renderer, tex, NULL, &txt);
    SDL_DestroyTexture(tex);
}

static void dessiner_fond_ecran(combat_t *combat, int largeur, int hauteur)
{
    if (!combat->texture_fond_ecran) return;

    int largeur_texture, hauteur_texture;
    SDL_QueryTexture(combat->texture_fond_ecran, NULL, NULL, &largeur_texture, &hauteur_texture);

    float echelle = fmax( (float) largeur / largeur_texture, (float) hauteur / hauteur_texture);

    SDL_Rect rect;

    rect.w = largeur_texture * echelle;
    rect.h = hauteur_texture * echelle;
    rect.x = (largeur - rect.w) / 2;
    rect.y = (hauteur - rect.h) / 2;

    SDL_RenderCopy(combat->renderer, combat->texture_fond_ecran, NULL, &rect);
}

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

    TTF_Init();
    (*combat)->font = TTF_OpenFont("Fonts/Enchanted Land.otf", 24);

    int running = 1;
    int majAffichage = 1;

    SDL_Point point;

    while (running) {
        SDL_Event e;

        while (SDL_PollEvent(&e)) {

            switch (e.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_MOUSEMOTION:
                    point.x = e.motion.x;
                    point.y = e.motion.y;

                    (*combat)->hover_bouton_leger = SDL_PointInRect(&point, &((*combat)->bouton_leger));
                    (*combat)->hover_bouton_lourd = SDL_PointInRect(&point, &((*combat)->bouton_lourd));

                    majAffichage = 1;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    point.x = e.button.x;
                    point.y = e.button.y;

                    if (SDL_PointInRect(&point, &((*combat)->bouton_leger))) {
                        printf("Attaque légère\n");
                    } else if (SDL_PointInRect(&point, &((*combat)->bouton_lourd))) {
                        printf("Attaque lourde\n");
                    }

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

void maj_affichage_fenetre_combat(combat_t *combat)
{
    SDL_Renderer *renderer = combat->renderer;

    int largeur, hauteur;
    SDL_GetRendererOutputSize(renderer, &largeur, &hauteur);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    dessiner_fond_ecran(combat, largeur, hauteur);

    int taille = largeur * 0.18f;
    int marge  = largeur * 0.03f;

    SDL_Rect perso = {
        marge,
        hauteur / 2 - taille / 2,
        taille,
        taille
    };

    SDL_Rect monstre = {
        largeur - marge - taille,
        hauteur / 2 - taille / 2,
        taille,
        taille
    };

    /* fond blanc sous les personnages */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &perso);
    SDL_RenderFillRect(renderer, &monstre);

    if (combat->texture_perso)
        SDL_RenderCopy(renderer, combat->texture_perso, NULL, &perso);

    if (combat->texture_monstre)
        SDL_RenderCopy(renderer, combat->texture_monstre, NULL, &monstre);

    /* boutons */

    int largeur_bouton = largeur * 0.22f;
    int hauteur_bouton = hauteur * 0.09f;
    int espace = largeur * 0.03f;

    combat->bouton_leger = (SDL_Rect){
        largeur / 2 - largeur_bouton - espace / 2,
        hauteur - hauteur_bouton - marge,
        largeur_bouton, hauteur_bouton
    };

    combat->bouton_lourd = (SDL_Rect){
        largeur / 2 + espace / 2,
        hauteur - hauteur_bouton - marge,
        largeur_bouton, hauteur_bouton
    };

    dessiner_bouton(combat, &combat->bouton_leger, combat->hover_bouton_leger, "Attaque Légère");
    dessiner_bouton(combat, &combat->bouton_lourd, combat->hover_bouton_lourd, "Attaque Lourde");

    SDL_RenderPresent(renderer);
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