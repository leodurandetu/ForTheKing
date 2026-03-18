#include "../lib/option.h"
/*#include "../lib/combat.h"*/

void dessiner_bouton(SDL_Renderer *renderer, TTF_Font *font,
                     SDL_Rect bouton, const char *texte,
                     SDL_Color couleur)
{
    SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, 255);
    SDL_RenderFillRect(renderer, &bouton);

    SDL_Color noir = {0,0,0,255};

    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, texte, noir);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_Rect txt;

    txt.w = surf->w;
    txt.h = surf->h;

    txt.x = bouton.x + (bouton.w - txt.w)/2;
    txt.y = bouton.y + (bouton.h - txt.h)/2;

    SDL_FreeSurface(surf);

    SDL_RenderCopy(renderer, tex, NULL, &txt);

    SDL_DestroyTexture(tex);
}

void dessiner_monstre(SDL_Renderer *renderer)
{
    SDL_Surface *img = IMG_Load("img/squelette.png");

    if(!img)
        return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);

    SDL_Rect rect_monstre;

    rect_monstre.x = 120;
    rect_monstre.y = 20;
    rect_monstre.w = 60;
    rect_monstre.h = 60;

    SDL_RenderCopy(renderer, texture, NULL, &rect_monstre);

    SDL_FreeSurface(img);
    SDL_DestroyTexture(texture);
}


int ouvrir_fenetre_choix(combat_t* combat)
{
    SDL_Window *fenetre = NULL;
    SDL_Renderer *renderer = NULL;

    fenetre = SDL_CreateWindow(
        "Rencontre",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        300, 200,
        SDL_WINDOW_SHOWN
    );

    if (!fenetre) return 0;

    renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(fenetre);
        return 0;
    }

    TTF_Font *font = TTF_OpenFont("Fonts/Enchanted Land.otf", 20);
    if (!font) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        return 0;
    }

    SDL_Rect bouton_attaque = {40,120,100,40};
    SDL_Rect bouton_fuite   = {160,120,100,40};

    SDL_Color rouge = {150,0,0,255};
    SDL_Color vert  = {0,150,0,255};

    int running = 1;
    int lancerCombat = 0;

    while(running)
    {
        SDL_Event e;

        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                running = 0;

            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point p = {e.button.x, e.button.y};

                if(SDL_PointInRect(&p,&bouton_attaque))
                {
                    printf("Combat lancé\n");
                    lancerCombat = 1;
                    running = 0;
                }

                if(SDL_PointInRect(&p,&bouton_fuite))
                {
                    printf("Fuite\n");
                    running = 0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer,200,200,200,255);
        SDL_RenderClear(renderer);

        dessiner_monstre(renderer);

        dessiner_bouton(renderer,font,bouton_attaque,"Attaquer",rouge);
        dessiner_bouton(renderer,font,bouton_fuite,"Fuir",vert);

        SDL_RenderPresent(renderer);
    }

    /* Libération propre */
    if(font) TTF_CloseFont(font);
    if(renderer) SDL_DestroyRenderer(renderer);
    if(fenetre) SDL_DestroyWindow(fenetre);

    return lancerCombat;
}