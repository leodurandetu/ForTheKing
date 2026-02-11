#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "../lib/carte.h"

case_t carte[TAILLE_CARTE][TAILLE_CARTE];

int main() {
    printf("For The King!\n");
    
    SDL_Window* pFenetre = NULL;
    SDL_Renderer * renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf ( stdout, "Echec de l’initialisation de la SDL (%s) \n", SDL_GetError ());
        return -1;
    }

    /* Creation de la fenetre */
    pFenetre = SDL_CreateWindow("For The King!", SDL_WINDOWPOS_UNDEFINED ,
            SDL_WINDOWPOS_UNDEFINED ,
            640 ,
            480 ,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!pFenetre) {
        fprintf(stderr, "Erreur à la création de la fenêtre : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer ( pFenetre , -1 , SDL_RENDERER_ACCELERATED );
    if ( renderer == NULL ){
        fprintf ( stderr , "Erreur a la creation du renderer \n" );
        exit ( EXIT_FAILURE );
    }

    char * nom_images[NB_BIOMES] = {
        "img/terre.png", "img/eau.png",
        "img/desert.png", "img/neige.png",
        "img/foret.png"
    };

    SDL_Texture * textures_cases[NB_BIOMES];

    int i;

    for (i = 0; i < NB_BIOMES; i++) {
        SDL_RWops * rwop = SDL_RWFromFile(nom_images[i] , "rb" );
        SDL_Surface * image = IMG_LoadPNG_RW (rwop);
        
        if (!image) {
            printf ( "IMG_LoadPNG_RW : %s \n " , IMG_GetError ());
        }

        SDL_Texture * tex =  SDL_CreateTextureFromSurface( renderer , image );
        if (! tex ){
            fprintf ( stderr , " Erreur a la creation du rendu de l’image : %s \n " , SDL_GetError ());
            exit ( EXIT_FAILURE );
        }
        SDL_FreeSurface ( image ); /* on a la texture , plus besoin de l ’ image */

        textures_cases[i] = tex;
    }

    srand(time(NULL));
    init_carte(carte);
    generer_eau(carte);
    generer_biomes(carte);

    int tailleCase = 50;

    if (pFenetre) {
        int running = 1;

        while (running) {
            SDL_Event e;

            while (SDL_PollEvent(&e)) {
                switch(e.type) {
                    case SDL_QUIT:
                        running = 0;
                        break;

                    case SDL_MOUSEWHEEL:
                        tailleCase += e.wheel.y;

                        SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
                        SDL_RenderClear (renderer);

                        afficher_carte_sdl(renderer, carte, textures_cases, tailleCase);

                        SDL_RenderPresent(renderer);

                    case SDL_WINDOWEVENT:
                        switch(e.window.event) {
                            case SDL_WINDOWEVENT_EXPOSED:
                            case SDL_WINDOWEVENT_SIZE_CHANGED:
                            case SDL_WINDOWEVENT_RESIZED:
                            case SDL_WINDOWEVENT_SHOWN:
                                SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
                                SDL_RenderClear (renderer);

                                afficher_carte_sdl(renderer, carte, textures_cases, tailleCase);

                                SDL_RenderPresent(renderer);

                                break;
                        }

                        break;
                }
            }

        }

    } else {
        fprintf(stderr, "Erreur de création de la fenêtre : %s\n", SDL_GetError());
    }

    SDL_DestroyWindow(pFenetre);

    SDL_Quit();

    return 0;
}