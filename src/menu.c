#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

/* Massoud */
int main() {

    int lancer_jeu = 0;

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Erreur SDL : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Initialisation img
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Erreur IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Initialisation de SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Initialisation de l'audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur SDL_mixer : %s\n", Mix_GetError());
        return -1;
    }

    Mix_Music *musique[2];
    musique[0] = Mix_LoadMUS("audio/Soundtrack1_menu.mp3");
    musique[1] = Mix_LoadMUS("audio/Soundtrack2_menu.mp3");

    if (!musique[0] || !musique[1]) {
        fprintf(stderr, "Erreur chargement musique : %s\n", Mix_GetError());
        // On continue sans la musique
    }
    
    // Variables pour gérer la boucle et alternance
    int indexMusique = 0;
    Uint32 debutMusique = SDL_GetTicks();
    int fadeDeclenche = 0;
    Uint32 dureeBoucle = 30000;  
    Uint32 fadeOutTime = 5000;   // 3 secondes de fondu

    int fullscreen = 0; // 0 = fenêtre, 1 = plein écran
    int windowW = 640;
    int windowH = 480;

    // Création de la fenêtre
    SDL_Window *window = SDL_CreateWindow(
        "Menu Principal",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowW,
        windowH,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "Erreur création fenêtre : %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur création renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Texture *backgroundTexture = NULL;
    SDL_Surface *backgroundSurface = IMG_Load("img/fond_menu.png");
    SDL_Texture *texLogoMusique = NULL;
    SDL_Surface *surfLogo = IMG_Load("img/logomusique.png");

    if (surfLogo) {
        texLogoMusique = SDL_CreateTextureFromSurface(renderer, surfLogo);
        SDL_FreeSurface(surfLogo);
    } else {
        fprintf(stderr, "Erreur chargement image: %s\n", IMG_GetError());
    }

    if (!backgroundSurface) {
        fprintf(stderr, "Erreur chargement image: %s\n", IMG_GetError());
    } else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);
    }

    // Chargement de la police d'écriture
    TTF_Font *police = TTF_OpenFont("Fonts/Enchanted Land.otf", 24); 
    if (!police) {
        fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
    }

    // Création des textures de texte
    SDL_Color couleurTexte = {255,255,255,255}; // Blanc
    
    SDL_Texture *textureTexteJouer = NULL;
    SDL_Texture *textureTexteCoop = NULL;
    SDL_Texture *textureTexteSolo = NULL;
    SDL_Texture *textureTexteQuitter = NULL;
    
    int jouerW = 0, jouerH = 0, quitterW = 0, quitterH = 0, soloH = 0, soloW = 0, coopH = 0, coopW = 0;

    if (police) {
        // Texte pour JOUER
        SDL_Surface *surfaceJouer = TTF_RenderText_Blended(police, "JOUER", couleurTexte);
        textureTexteJouer = SDL_CreateTextureFromSurface(renderer, surfaceJouer);
        SDL_QueryTexture(textureTexteJouer, NULL, NULL, &jouerW, &jouerH);
        SDL_FreeSurface(surfaceJouer); 
        // Texte pour QUITTER

        SDL_Surface *surfaceQuitter = TTF_RenderText_Blended(police, "QUITTER", couleurTexte);
        textureTexteQuitter = SDL_CreateTextureFromSurface(renderer, surfaceQuitter);
        SDL_QueryTexture(textureTexteQuitter, NULL, NULL, &quitterW, &quitterH);
        SDL_FreeSurface(surfaceQuitter);
        // Texte pour COOP

        SDL_Surface *surfaceCoop = TTF_RenderText_Blended(police, "COOP", couleurTexte);
        textureTexteCoop = SDL_CreateTextureFromSurface(renderer, surfaceCoop);
        SDL_QueryTexture(textureTexteCoop, NULL, NULL, &coopW, &coopH);
        SDL_FreeSurface(surfaceCoop);
        // Texte pour SOLO

        SDL_Surface *surfaceSolo = TTF_RenderText_Blended(police, "SOLO", couleurTexte);
        textureTexteSolo = SDL_CreateTextureFromSurface(renderer, surfaceSolo);
        SDL_QueryTexture(textureTexteSolo, NULL, NULL, &soloW, &soloH);
        SDL_FreeSurface(surfaceSolo);
    }

    // Définition des boutons 
    SDL_Rect boutonJouer   = { 220, 80, 200, 50 };
    SDL_Rect boutonSolo    = { 220, 160, 200, 50 };
    SDL_Rect boutonCoop    = { 220, 240, 200, 50 };
    SDL_Rect boutonQuitter = { 220, 320, 200, 50 };
    SDL_Rect boutonMusique = { windowW - 100, windowH - 100, 80, 80 };

    // Calcul pour centrer le texte dans les boutons
    SDL_Rect rectTexteJouer   = { boutonJouer.x + (boutonJouer.w - jouerW) / 2, boutonJouer.y + (boutonJouer.h - jouerH) / 2, jouerW, jouerH };
    SDL_Rect rectTexteSolo    = { boutonSolo.x + (boutonSolo.w - soloW) / 2, boutonSolo.y + (boutonSolo.h - soloH) / 2, soloW, soloH };
    SDL_Rect rectTexteCoop    = { boutonCoop.x + (boutonCoop.w - coopW) / 2, boutonCoop.y + (boutonCoop.h - coopH) / 2, coopW, coopH };
    SDL_Rect rectTexteQuitter = { boutonQuitter.x + (boutonQuitter.w - quitterW) / 2, boutonQuitter.y + (boutonQuitter.h - quitterH) / 2, quitterW, quitterH };

    int menuActif = 1;
    SDL_Event event;

    void resizeUI() {
        SDL_GetWindowSize(window, &windowW, &windowH);
        boutonJouer.x = windowW / 3;
        boutonJouer.y = windowH / 6;
        boutonJouer.w = windowW / 3;
        boutonJouer.h = windowH / 10;

        boutonSolo.x = windowW / 3;
        boutonSolo.y = windowH * 2 / 6;
        boutonSolo.w = windowW / 3;
        boutonSolo.h = windowH / 10;

        boutonCoop.x = windowW / 3;
        boutonCoop.y = windowH * 3 / 6;
        boutonCoop.w = windowW / 3;
        boutonCoop.h = windowH / 10;

        boutonQuitter.x = windowW / 3;
        boutonQuitter.y = windowH * 4 / 6;
        boutonQuitter.w = windowW / 3;
        boutonQuitter.h = windowH / 10;

        boutonMusique.w = 80;
        boutonMusique.h = 80;
        boutonMusique.x = windowW - (boutonMusique.w + 20); 
        boutonMusique.y = windowH - (boutonMusique.h + 20);

        rectTexteJouer.x = boutonJouer.x + (boutonJouer.w - jouerW) / 2;
        rectTexteJouer.y = boutonJouer.y + (boutonJouer.h - jouerH) / 2;

        rectTexteSolo.x = boutonSolo.x + (boutonSolo.w - soloW) / 2;
        rectTexteSolo.y = boutonSolo.y + (boutonSolo.h - soloH) / 2;

        rectTexteCoop.x = boutonCoop.x + (boutonCoop.w - coopW) / 2;
        rectTexteCoop.y = boutonCoop.y + (boutonCoop.h - coopH) / 2;

        rectTexteQuitter.x = boutonQuitter.x + (boutonQuitter.w - quitterW) / 2;
        rectTexteQuitter.y = boutonQuitter.y + (boutonQuitter.h - quitterH) / 2;
    }

    resizeUI();

    /* --- BOUCLE PRINCIPALE ---*/
    while (menuActif) {

        // Gestion de l'audio
        if (musique[indexMusique]) {
            Uint32 elapsed = SDL_GetTicks() - debutMusique;

            // Déclencher fade out avant la fin
            if (!fadeDeclenche && elapsed >= dureeBoucle - fadeOutTime) {
                Mix_FadeOutMusic(fadeOutTime);
                fadeDeclenche = 1;
            }

            // Si musique terminée ou arrêtée -> passer à la suivante
            if (!Mix_PlayingMusic()) {
                indexMusique = (indexMusique + 1) % 2; // alterne 0 <-> 1
                Mix_FadeInMusic(musique[indexMusique], 1, 10000);
                debutMusique = SDL_GetTicks();
                fadeDeclenche = 0;
            }
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                menuActif = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_f) {
                    fullscreen = !fullscreen;
                    if (fullscreen) {
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    } else {
                        SDL_SetWindowFullscreen(window, 0);
                    }
                    resizeUI();
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Point p = {mouseX, mouseY};

                // Clic JOUER
                if (mouseX >= boutonJouer.x && mouseX <= (boutonJouer.x + boutonJouer.w) &&
                    mouseY >= boutonJouer.y && mouseY <= (boutonJouer.y + boutonJouer.h)) {
                    printf("Bouton JOUER cliqué !\n");
                    lancer_jeu = 1; // On enregistre qu'on veut lancer le jeu
                    menuActif = 0; // on sort du menu
                }

                // Clic SOLO
                if (mouseX >= boutonSolo.x && mouseX <= (boutonSolo.x + boutonSolo.w) &&
                    mouseY >= boutonSolo.y && mouseY <= (boutonSolo.y + boutonSolo.h)) {
                    printf("Bouton SOLO cliqué !\n");
                }

                // Clic COOP
                if (mouseX >= boutonCoop.x && mouseX <= (boutonCoop.x + boutonCoop.w) &&
                    mouseY >= boutonCoop.y && mouseY <= (boutonCoop.y + boutonCoop.h)) {
                    printf("Bouton COOP cliqué !\n");
                }

                // Clic QUITTER
                if (mouseX >= boutonQuitter.x && mouseX <= (boutonQuitter.x + boutonQuitter.w) &&
                    mouseY >= boutonQuitter.y && mouseY <= (boutonQuitter.y + boutonQuitter.h)) {
                    printf("Bouton QUITTER cliqué !\n");
                    menuActif = 0;
                }

                // Clic musique
                if (SDL_PointInRect(&p, &boutonMusique)) {
                    if (Mix_PausedMusic()) {
                        Mix_ResumeMusic();
                        printf("Musique relancée\n");
                    } else {
                        Mix_PauseMusic();
                        printf("Musique en pause\n");
                    }
                }
            }
        }

        /* --- AFFICHAGE --- */
        SDL_RenderClear(renderer);

        if (backgroundTexture) {
            SDL_Rect dest = {0,0,windowW,windowH};
            SDL_RenderCopy(renderer, backgroundTexture, NULL, &dest);
        }

        SDL_SetRenderDrawColor(renderer, 200, 100, 0, 255);
        SDL_RenderFillRect(renderer, &boutonJouer);

        SDL_SetRenderDrawColor(renderer, 200, 100, 0, 255);
        SDL_RenderFillRect(renderer, &boutonSolo);

        SDL_SetRenderDrawColor(renderer, 200, 100, 0, 255);
        SDL_RenderFillRect(renderer, &boutonCoop);

        SDL_SetRenderDrawColor(renderer, 200, 100, 0, 255);
        SDL_RenderFillRect(renderer, &boutonQuitter);

        if (texLogoMusique) {
            // Dessin d'une bordure marron médiévale
            SDL_SetRenderDrawColor(renderer, 101, 67, 33, 255); 
            for(int i = 0; i < 5; i++) { 
                SDL_Rect border = {boutonMusique.x - i, boutonMusique.y - i, boutonMusique.w + (i*2), boutonMusique.h + (i*2)};
                SDL_RenderDrawRect(renderer, &border);
            }
            SDL_RenderCopy(renderer, texLogoMusique, NULL, &boutonMusique);
        }

        if (textureTexteJouer) SDL_RenderCopy(renderer, textureTexteJouer, NULL, &rectTexteJouer);
        if (textureTexteSolo) SDL_RenderCopy(renderer, textureTexteSolo, NULL, &rectTexteSolo);
        if (textureTexteCoop) SDL_RenderCopy(renderer, textureTexteCoop, NULL, &rectTexteCoop);
        if (textureTexteQuitter) SDL_RenderCopy(renderer, textureTexteQuitter, NULL, &rectTexteQuitter);

        SDL_RenderPresent(renderer);
    }

    /* --- NETTOYAGE ---*/
    if (textureTexteJouer) SDL_DestroyTexture(textureTexteJouer);
    if (textureTexteSolo) SDL_DestroyTexture(textureTexteSolo);
    if (textureTexteCoop) SDL_DestroyTexture(textureTexteCoop);
    if (textureTexteQuitter) SDL_DestroyTexture(textureTexteQuitter);
    if (police) TTF_CloseFont(police);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (texLogoMusique) SDL_DestroyTexture(texLogoMusique);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit(); 
    SDL_Quit();

    // Lance le jeu si on clique sur Jouer (à revoir car c'est une version simple)
    if (lancer_jeu == 1) system("./bin/fortheking");
    
    return EXIT_SUCCESS;
}
