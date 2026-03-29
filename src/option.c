#include "../lib/option.h"
/*#include "../lib/combat.h"*/

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

SDL_Rect dessiner_monstre(SDL_Renderer *renderer)
{
    SDL_Surface *img = IMG_Load("img/squelette.png");

    SDL_Rect rect_monstre = {120, 20, 60, 60};

    if(img)
    {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);

        SDL_RenderCopy(renderer, texture, NULL, &rect_monstre);

        SDL_FreeSurface(img);
        SDL_DestroyTexture(texture);
    }

    return rect_monstre;
}



int afficher_option(SDL_Renderer* gRenderer, TTF_Font *gFont,
                    int perso_x, int perso_y, int perso_w, int perso_h,
                    int fuite_bloquee){
  int lancerCombat = 0;
    int running = 1;

    const int MENU_W = 200;
    const int MENU_H = 130;
    const int BTN_W  = 160;
    const int BTN_H  = 35;
    const int MARGIN = 15;
    const int GAP    = 10;

    /* Récupération dynamique de la taille du renderer */
    int ECRAN_W, ECRAN_H;
    SDL_RenderGetLogicalSize(gRenderer, &ECRAN_W, &ECRAN_H);
    if (ECRAN_W == 0 || ECRAN_H == 0)
        SDL_GetRendererOutputSize(gRenderer, &ECRAN_W, &ECRAN_H);

    /*
     * On tente de placer le menu à droite du sprite,
     * aligné verticalement sur son centre.
     * Si ça déborde à droite → on passe à gauche.
     */
    int mx = perso_x + perso_w + 6;   /* colle à droite du sprite */
    int my = perso_y + (perso_h - MENU_H) / 2;  /* centré verticalement */

    if (mx + MENU_W > ECRAN_W)
        mx = perso_x - MENU_W - 6;    /* repli à gauche */

    /* Clamp vertical */
    if (my < 0)              my = 0;
    if (my + MENU_H > ECRAN_H) my = ECRAN_H - MENU_H;

    SDL_Rect fond = {mx, my, MENU_W, MENU_H};

    int btn_x     = (MENU_W - BTN_W) / 2;
    int btn_y_atk = MARGIN;
    int btn_y_run = MARGIN + BTN_H + GAP;

    SDL_Rect bouton_attaque = {btn_x, btn_y_atk, BTN_W, BTN_H};
    SDL_Rect bouton_fuite   = {btn_x, btn_y_run, BTN_W, BTN_H};

    /* Rectangles en coordonnées écran pour la détection de clic */
    SDL_Rect atk_screen = {fond.x + btn_x, fond.y + btn_y_atk, BTN_W, BTN_H};
    SDL_Rect run_screen = {fond.x + btn_x, fond.y + btn_y_run, BTN_W, BTN_H};

    SDL_Color rouge = {150, 0,   0,   255};
    SDL_Color vert  = {0,   150, 0,   255};
    SDL_Color gris  = {120, 120, 120, 255};

    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(
        0, MENU_W, MENU_H, 32, SDL_PIXELFORMAT_RGBA8888
    );
    if (!surface) return 0;

    SDL_Event e;

    while (running)
    {
        /* === Événements === */
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                SDL_FreeSurface(surface);
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

        /* Petite flèche pointant vers le joueur */
        int arrow_y = fond.y + MENU_H / 2;

        if (mx == perso_x + perso_w + 6)
        {
            /* Menu à droite → flèche à gauche du menu */
            SDL_RenderDrawLine(gRenderer,
                fond.x,     arrow_y - 6,
                fond.x - 8, arrow_y);
            SDL_RenderDrawLine(gRenderer,
                fond.x - 8, arrow_y,
                fond.x,     arrow_y + 6);
        }
        else
        {
            /* Menu à gauche → flèche à droite du menu */
            SDL_RenderDrawLine(gRenderer,
                fond.x + MENU_W,     arrow_y - 6,
                fond.x + MENU_W + 8, arrow_y);
            SDL_RenderDrawLine(gRenderer,
                fond.x + MENU_W + 8, arrow_y,
                fond.x + MENU_W,     arrow_y + 6);
        }

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

    SDL_FreeSurface(surface);
    return lancerCombat;
}
