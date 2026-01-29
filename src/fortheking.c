#include <stdio.h>
#include <SDL2/SDL.h>

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

    if (pFenetre) {
        int running = 1;

        while (running) {
            SDL_Event e;

            while (SDL_PollEvent(&e)) {
                switch(e.type) {
                    case SDL_QUIT:
                        running = 0;
                        break;

                    case SDL_WINDOWEVENT:
                        switch(e.window.event) {
                            case SDL_WINDOWEVENT_EXPOSED:
                            case SDL_WINDOWEVENT_SIZE_CHANGED:
                            case SDL_WINDOWEVENT_RESIZED:
                            case SDL_WINDOWEVENT_SHOWN:

                            SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
                            SDL_RenderClear (renderer);

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