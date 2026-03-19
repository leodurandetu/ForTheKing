#include "../lib/pause_menu.h"
#include <SDL2/SDL_mixer.h>

/* Reprise de ta fonction pour garder ton style intact */
static void dessinerBoutonPause(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect rectBtn, SDL_Rect rectTxt, SDL_Point mouse) {
    int survoler = SDL_PointInRect(&mouse, &rectBtn);

    if (survoler) {
        SDL_SetRenderDrawColor(renderer, 80, 75, 95, 230); // Plus clair au survol
    } else {
        SDL_SetRenderDrawColor(renderer, 50, 45, 65, 200); // Normal
    }
    SDL_RenderFillRect(renderer, &rectBtn);

    SDL_SetRenderDrawColor(renderer, 150, 145, 130, 255); // Bordure claire
    SDL_RenderDrawRect(renderer, &rectBtn);
    
    SDL_Rect bordureInt = {rectBtn.x + 2, rectBtn.y + 2, rectBtn.w - 4, rectBtn.h - 4};
    SDL_SetRenderDrawColor(renderer, 30, 25, 35, 255); // Bordure sombre
    SDL_RenderDrawRect(renderer, &bordureInt);

    if (survoler) {
        SDL_SetTextureColorMod(texture, 255, 215, 0); // Or
    } else {
        SDL_SetTextureColorMod(texture, 180, 180, 255); // Bleuté
    }
    SDL_RenderCopy(renderer, texture, NULL, &rectTxt);
    
    SDL_SetTextureColorMod(texture, 255, 255, 255);
}

int afficher_menu_pause(SDL_Renderer* renderer, TTF_Font* police, int windowW, int windowH) {
    int en_pause = 1;
    int choix = 0; // 0 = Continuer, 1 = Quitter
    int drag_volume = 0; 

    SDL_Color couleurBlanc = {255, 255, 255, 255};

    /* CAPTURE DE L'ÉCRAN ACTUEL */
    // On prend une photo de la carte pour s'en servir de fond
    SDL_Surface *capture = SDL_CreateRGBSurface(0, windowW, windowH, 32, 
                            0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, capture->pixels, capture->pitch);
    SDL_Texture *fondTexture = SDL_CreateTextureFromSurface(renderer, capture);
    SDL_FreeSurface(capture);
    /* ------------------------------------------- */

    /* Création des textes */
    SDL_Surface* sCont = TTF_RenderUTF8_Blended(police, "Continuer", couleurBlanc);
    SDL_Texture* tCont = SDL_CreateTextureFromSurface(renderer, sCont);
    SDL_Surface* sOpt = TTF_RenderUTF8_Blended(police, "Options : Volume", couleurBlanc);
    SDL_Texture* tOpt = SDL_CreateTextureFromSurface(renderer, sOpt);
    SDL_Surface* sQuit = TTF_RenderUTF8_Blended(police, "Quitter", couleurBlanc);
    SDL_Texture* tQuit = SDL_CreateTextureFromSurface(renderer, sQuit);

    int btnW = windowW / 3;
    int btnH = 50;
    int centreX = (windowW - btnW) / 2;
    int startY = windowH / 3;

    /* Positions des boutons */
    SDL_Rect btnContinuer = {centreX, startY, btnW, btnH};
    SDL_Rect rectTxtCont = {centreX + (btnW - sCont->w)/2, startY + (btnH - sCont->h)/2, sCont->w, sCont->h};

    SDL_Rect rectTxtOpt = {centreX + (btnW - sOpt->w)/2, startY + 80, sOpt->w, sOpt->h};
    SDL_Rect barreVolumeFond = {centreX, startY + 120, btnW, 20}; 
    
    SDL_Rect btnQuitter = {centreX, startY + 180, btnW, btnH};
    SDL_Rect rectTxtQuit = {centreX + (btnW - sQuit->w)/2, startY + 180 + (btnH - sQuit->h)/2, sQuit->w, sQuit->h};

    SDL_Event e;
    while (en_pause) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePos = {mouseX, mouseY};

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                choix = 1;
                en_pause = 0;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                choix = 0;
                en_pause = 0; 
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (SDL_PointInRect(&mousePos, &btnContinuer)) {
                    choix = 0;
                    en_pause = 0;
                } else if (SDL_PointInRect(&mousePos, &btnQuitter)) {
                    choix = 1;
                    en_pause = 0;
                } else if (SDL_PointInRect(&mousePos, &barreVolumeFond)) {
                    drag_volume = 1; 
                }
            }

            if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                drag_volume = 0; 
            }

            if (drag_volume && (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN)) {
                float pourcentage = (float)(mouseX - barreVolumeFond.x) / (float)barreVolumeFond.w;
                if (pourcentage < 0.0f) pourcentage = 0.0f;
                if (pourcentage > 1.0f) pourcentage = 1.0f;
                
                Mix_VolumeMusic((int)(pourcentage * MIX_MAX_VOLUME)); 
            }
        }

        /* Afficher l'image figée de la carte */
        SDL_RenderCopy(renderer, fondTexture, NULL, NULL);

        /* Assombrir par dessus avec le voile transparent */
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); 
        SDL_Rect fondComplet = {0, 0, windowW, windowH};
        SDL_RenderFillRect(renderer, &fondComplet);

        /* Afficher le bouton Continuer */
        dessinerBoutonPause(renderer, tCont, btnContinuer, rectTxtCont, mousePos);
        
        /* Afficher la zone Volume */
        SDL_RenderCopy(renderer, tOpt, NULL, &rectTxtOpt);
        
        SDL_SetRenderDrawColor(renderer, 50, 45, 65, 255);
        SDL_RenderFillRect(renderer, &barreVolumeFond);
        SDL_SetRenderDrawColor(renderer, 150, 145, 130, 255);
        SDL_RenderDrawRect(renderer, &barreVolumeFond);

        int volumeActuel = Mix_VolumeMusic(-1);
        float ratioVolume = (float)volumeActuel / (float)MIX_MAX_VOLUME;
        SDL_Rect barreVolumeRemplie = {barreVolumeFond.x, barreVolumeFond.y, (int)(barreVolumeFond.w * ratioVolume), barreVolumeFond.h};
        SDL_SetRenderDrawColor(renderer, 180, 180, 255, 255); 
        SDL_RenderFillRect(renderer, &barreVolumeRemplie);

        /* Afficher le bouton Quitter */
        dessinerBoutonPause(renderer, tQuit, btnQuitter, rectTxtQuit, mousePos);

        SDL_RenderPresent(renderer);
        SDL_Delay(7); 
    }

    /* Nettoyage */
    SDL_DestroyTexture(fondTexture); // On détruit l'image figée
    SDL_FreeSurface(sCont); SDL_DestroyTexture(tCont);
    SDL_FreeSurface(sOpt);  SDL_DestroyTexture(tOpt);
    SDL_FreeSurface(sQuit); SDL_DestroyTexture(tQuit);

    return choix;
}
