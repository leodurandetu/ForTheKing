#include "../lib/option.h"
#include "../lib/affichage_commun.h"

void dessiner_bouton(SDL_Surface *surface, TTF_Font *font,
                     SDL_Rect *bouton, const char *texte,
                     SDL_Color couleur)
{
    /* Couleur du bouton */
    Uint32 col = SDL_MapRGB(surface->format,
                            couleur.r, couleur.g, couleur.b);

    /* Dessin du rectangle du bouton */
    SDL_FillRect(surface, bouton, col);

    /* Couleur du texte */
    SDL_Color noir = {0,0,0,255};

    /* Création de la surface contenant le texte */
    SDL_Surface *surfTexte = TTF_RenderUTF8_Blended(font, texte, noir);
    if(!surfTexte) return;

    /* Position du texte (centré dans le bouton) */
    SDL_Rect posTexte;

    posTexte.w = surfTexte->w;
    posTexte.h = surfTexte->h;

    posTexte.x = bouton->x + (bouton->w - posTexte.w) / 2;
    posTexte.y = bouton->y + (bouton->h - posTexte.h) / 2;

    /* Copie du texte sur la surface principale */
    SDL_BlitSurface(surfTexte, NULL, surface, &posTexte);

    /* Libération de la surface texte */
    SDL_FreeSurface(surfTexte);
}

SDL_Texture* dessiner_monstre(SDL_Renderer *renderer, const char *chemin,
                               int x, int y, int w, int h)
{
    SDL_Surface *img = IMG_Load(chemin);
    if (!img) return NULL;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
    if (!texture) return NULL;

    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    return texture;
}



int afficher_option(SDL_Renderer* gRenderer, TTF_Font *gFont,
                    int perso_x, int perso_y, int perso_w, int perso_h,
                    int fuite_bloquee,const char *chemin_monstre,
                    int monstre_x, int monstre_y, int tailleCase) {
    int lancerCombat = 0;
    int running = 1;

    const int IMG_W  = 60;
    const int IMG_H  = 60;
    const int MENU_W = 200;
    const int MENU_H = 200;
    const int BTN_W  = 160;
    const int BTN_H  = 35;
    const int MARGIN = 15;
    const int GAP    = 10;

    float cx_monstre, cy_monstre;
    case_vers_pixels(monstre_x, monstre_y, perso_x, perso_y,
                    tailleCase, gRenderer, &cx_monstre, &cy_monstre);

    /* Récupération dynamique de la taille du renderer */
    int fen_w, fen_h;
    SDL_GetRendererOutputSize(gRenderer, &fen_w, &fen_h);

    /* Placement du menu à droite du sprite, avec repli à gauche */
    /*int mx = perso_x + perso_w + 6;
    int my = perso_y + (perso_h - MENU_H) / 2;

    if (mx + MENU_W > ECRAN_W)
        mx = perso_x - MENU_W - 6;

    if (my < 0)                my = 0;
    if (my + MENU_H > ECRAN_H) my = ECRAN_H - MENU_H;*/

    int rayon_hex = tailleCase / 2;
    int mx = (int)(cx_monstre + rayon_hex + 8);

    if (mx + MENU_W > fen_w) {
        mx = (int)(cx_monstre - rayon_hex - 8 - MENU_W);
    }

    int my = (int)(cy_monstre - MENU_H / 2);
    
    if (my < 0) {
        my = 0;
    }

    if (my + MENU_H > fen_h) {
        my = fen_h - MENU_H;
    }

    SDL_Rect fond = {mx, my, MENU_W, MENU_H};

    int btn_x     = (MENU_W - BTN_W) / 2;
    int btn_y_atk = MARGIN + IMG_H + GAP;
    int btn_y_run = MARGIN + IMG_H + GAP + BTN_H + GAP;

    SDL_Rect bouton_attaque = {btn_x, btn_y_atk, BTN_W, BTN_H};
    SDL_Rect bouton_fuite   = {btn_x, btn_y_run, BTN_W, BTN_H};

    SDL_Rect atk_screen = {fond.x + btn_x, fond.y + btn_y_atk, BTN_W, BTN_H};
    SDL_Rect run_screen = {fond.x + btn_x, fond.y + btn_y_run, BTN_W, BTN_H};

    SDL_Color rouge = {150, 0,   0,   255};
    SDL_Color vert  = {0,   150, 0,   255};
    SDL_Color gris  = {120, 120, 120, 255};

    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(
        0, MENU_W, MENU_H, 32, SDL_PIXELFORMAT_RGBA8888
    );
    if (!surface) return 0;

    /* Chargement du monstre une seule fois avant la boucle */
    
    SDL_Texture *tex_monstre = dessiner_monstre(
        gRenderer, chemin_monstre,
        fond.x + (MENU_W - IMG_W) / 2,
        fond.y + MARGIN,
        IMG_W, IMG_H
    );

    SDL_Rect rect_monstre = {
        fond.x + (MENU_W - IMG_W) / 2,
        fond.y + MARGIN,
        IMG_W, IMG_H
    };

    SDL_Event e;

    while (running)
    {
        /* === Événements === */
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                SDL_FreeSurface(surface);
                if (tex_monstre) SDL_DestroyTexture(tex_monstre);
                return 0;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point p = {e.button.x, e.button.y};

                if (SDL_PointInRect(&p, &atk_screen))
                {
                    printf("Combat lancé\n");
                    lancerCombat = 1;
                    running = 0;
                }

                if (SDL_PointInRect(&p, &run_screen) && !fuite_bloquee)
                {
                    printf("Fuite\n");
                    lancerCombat = 0;
                    running = 0;
                }
            }
        }

        /* === Affichage === */

        /* Overlay sombre semi-transparent */
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 120);
        SDL_Rect overlay = {fond.x - 6, fond.y - 6,
                            fond.w + 12, fond.h + 12};
        SDL_RenderFillRect(gRenderer, &overlay);

        /* Fond du menu */
        SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);
        SDL_RenderFillRect(gRenderer, &fond);

        /* Bordure */
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(gRenderer, &fond);

        /* Flèche pointant vers le joueur */
        int arrow_y = fond.y + MENU_H / 2;

        if (mx > (int)cx_monstre)
        {
            // menu à droite : flèche à gauche
            SDL_RenderDrawLine(gRenderer,
                fond.x,     arrow_y - 6,
                fond.x - 8, arrow_y);
            SDL_RenderDrawLine(gRenderer,
                fond.x - 8, arrow_y,
                fond.x,     arrow_y + 6);
        }
        else
        {
            // menu à gauche : flèche à droite
            SDL_RenderDrawLine(gRenderer,
                fond.x + MENU_W,     arrow_y - 6,
                fond.x + MENU_W + 8, arrow_y);
            SDL_RenderDrawLine(gRenderer,
                fond.x + MENU_W + 8, arrow_y,
                fond.x + MENU_W,     arrow_y + 6);
        }

        /* Sprite du monstre */
        if (tex_monstre)
            SDL_RenderCopy(gRenderer, tex_monstre, NULL, &rect_monstre);

        /* Dessin des boutons sur la surface locale */
        SDL_FillRect(surface, NULL,
            SDL_MapRGBA(surface->format, 0, 0, 0, 0));

        dessiner_bouton(surface, gFont, &bouton_attaque, "Attaquer", rouge);

        if (fuite_bloquee)
            dessiner_bouton(surface, gFont, &bouton_fuite, "Fuir", gris);
        else
            dessiner_bouton(surface, gFont, &bouton_fuite, "Fuir", vert);

        SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surface);
        if (texture)
        {
            SDL_RenderCopy(gRenderer, texture, NULL, &fond);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(gRenderer);
        SDL_Delay(16);
    }

    /* Libération des ressources */
    SDL_FreeSurface(surface);
    if (tex_monstre) SDL_DestroyTexture(tex_monstre);

    return lancerCombat;
}
