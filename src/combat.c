#include "../lib/combat.h"

extern void dessiner_icone(combat_t *combat, SDL_Rect *r_icone, int survole, SDL_Texture *texture_arme, SDL_Texture *texture_texte, SDL_Rect *rect_texte)
{
    // transparence
    SDL_SetRenderDrawBlendMode(combat->renderer, SDL_BLENDMODE_BLEND);

    if (survole) {
        // Survolé -> fond un peu plus clair et bordure Jaune/Or
        SDL_SetRenderDrawColor(combat->renderer, 255, 255, 255, 60); 
        SDL_RenderFillRect(combat->renderer, r_icone);
        SDL_SetRenderDrawColor(combat->renderer, 255, 215, 0, 255); 
    } else {
        // fond très sombre transparent et bordure grise
        SDL_SetRenderDrawColor(combat->renderer, 0, 0, 0, 100); 
        SDL_RenderFillRect(combat->renderer, r_icone);
        SDL_SetRenderDrawColor(combat->renderer, 150, 150, 150, 255); 
    }

    SDL_RenderDrawRect(combat->renderer, r_icone);

    if (texture_arme) {
        SDL_RenderCopy(combat->renderer, texture_arme, NULL, r_icone);
    }

    if (texture_texte && rect_texte) {
        SDL_Rect rect = *rect_texte;

        // Centrer le texte horizontalement par rapport à l'icône
        rect.x = r_icone->x + (r_icone->w - rect.w) / 2;
        rect.y = r_icone->y + r_icone->h + 5; // un peu en dessous de l'icône

        SDL_RenderCopy(combat->renderer, texture_texte, NULL, &rect);
    }

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

    // Chargement des symboles de l'attaque legère
    (*combat)->texture_attaque_legere = NULL;
    SDL_Surface *img_legere = IMG_Load("img/attaque_l.png"); 
    if (img_legere) {
        (*combat)->texture_attaque_legere = SDL_CreateTextureFromSurface((*combat)->renderer, img_legere);
        SDL_SetTextureBlendMode((*combat)->texture_attaque_legere, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_legere);
    }

    // Chargement des symboles de l'attaque lourde
    (*combat)->texture_attaque_lourde = NULL;
    SDL_Surface *img_lourde = IMG_Load("img/attaque_L.png"); 
    if (img_lourde) {
        (*combat)->texture_attaque_lourde = SDL_CreateTextureFromSurface((*combat)->renderer, img_lourde);
        SDL_SetTextureBlendMode((*combat)->texture_attaque_lourde, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_lourde);
    }

    TTF_Init();
    (*combat)->font = TTF_OpenFont("Fonts/Enchanted Land.otf", 24);

    SDL_Color blanc = {255, 255, 255, 255}; // blanc

    // Texte bouton léger
    SDL_Surface *surf_leger = TTF_RenderUTF8_Blended((*combat)->font, "Attaque Légère", blanc);

    if (surf_leger) {
        (*combat)->texture_texte_leger = SDL_CreateTextureFromSurface((*combat)->renderer, surf_leger);
        (*combat)->rect_texte_leger.w = surf_leger->w;
        (*combat)->rect_texte_leger.h = surf_leger->h;
        SDL_FreeSurface(surf_leger);
    }

    // Texte bouton lourd
    SDL_Surface *surf_lourd = TTF_RenderUTF8_Blended((*combat)->font, "Attaque Lourde", blanc);
    if (surf_lourd) {
        (*combat)->texture_texte_lourd = SDL_CreateTextureFromSurface((*combat)->renderer, surf_lourd);
        (*combat)->rect_texte_lourd.w = surf_lourd->w;
        (*combat)->rect_texte_lourd.h = surf_lourd->h;
        SDL_FreeSurface(surf_lourd);
    }

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

                    (*combat)->survole_bouton_leger = SDL_PointInRect(&point, &((*combat)->bouton_leger));
                    (*combat)->survole_bouton_lourd = SDL_PointInRect(&point, &((*combat)->bouton_lourd));

                    majAffichage = 1;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    point.x = e.button.x;
                    point.y = e.button.y;

                    // Test
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

   int w_fond, h_fond;
    // On récupère les dimensions originales de l'image de fond
    if (combat->texture_fond_ecran) {
        SDL_QueryTexture(combat->texture_fond_ecran, NULL, NULL, &w_fond, &h_fond);
    } else {
        w_fond = largeur; h_fond = hauteur;
    }

    float echelle = fmax( (float) largeur / w_fond, (float) hauteur / h_fond);
    float hauteur_fond_affichee = h_fond * echelle;
    float y_image_fond = (hauteur - hauteur_fond_affichee) / 2.0f;
    float y_sol = y_image_fond + (hauteur_fond_affichee * 0.78f);
    // On place les personnages pour qu'ils touchent ce sol
    int y_personnage = (int)y_sol - taille;

    SDL_Rect perso = {
        marge,
        y_personnage,
        taille,
        taille
    };

    SDL_Rect monstre = {
        largeur - marge - taille,
        y_personnage,
        taille,
        taille
    };

    /* fond blanc sous les personnages */
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //SDL_RenderFillRect(renderer, &perso);
    //SDL_RenderFillRect(renderer, &monstre);

    if (combat->texture_perso)
        SDL_RenderCopy(renderer, combat->texture_perso, NULL, &perso);

    if (combat->texture_monstre)
        SDL_RenderCopy(renderer, combat->texture_monstre, NULL, &monstre);

    /* boutons */

    // On fait des icônes carrées, plus petites
    int taille_icone = largeur * 0.08f; 

    // Placement en hauteur : un peu au-dessus du sol, entre les personnages
    int y_icones = y_personnage + (taille * 0.2f);

    /* 35% de la largeur à gauche / à droite */
    int marge_bord = largeur * 0.35f;

    combat->bouton_leger = (SDL_Rect){
        marge_bord,
        y_icones,
        taille_icone, 
        taille_icone
    };

    combat->bouton_lourd = (SDL_Rect){
        largeur - marge_bord - taille_icone, 
        y_icones,
        taille_icone, 
        taille_icone
    };

    dessiner_icone(combat, &combat->bouton_leger, combat->survole_bouton_leger, combat->texture_attaque_legere, combat->texture_texte_leger, &combat->rect_texte_leger);
    dessiner_icone(combat, &combat->bouton_lourd, combat->survole_bouton_lourd, combat->texture_attaque_lourde, combat->texture_texte_lourd, &combat->rect_texte_lourd);

    SDL_RenderPresent(renderer);
}

void detruire_fenetre_combat(combat_t ** combat) {
    
    if (combat != NULL && *combat != NULL) {

        if ((*combat)->texture_attaque_legere != NULL) {
            SDL_DestroyTexture((*combat)->texture_attaque_legere);
            (*combat)->texture_attaque_legere = NULL;
        }

        if ((*combat)->texture_attaque_lourde != NULL) {
            SDL_DestroyTexture((*combat)->texture_attaque_lourde);
            (*combat)->texture_attaque_lourde = NULL;
        }

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

        if (((*combat)->texture_texte_leger) != NULL) {
            SDL_DestroyTexture((*combat)->texture_texte_leger);
            (*combat)->texture_texte_leger = NULL;
        }
        if (((*combat)->texture_texte_lourd) != NULL) {
            SDL_DestroyTexture((*combat)->texture_texte_lourd);
            (*combat)->texture_texte_lourd = NULL;
        }

        free(*combat);
        *combat = NULL;
    }

}
