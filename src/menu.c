#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// Fonction pour dessiner les boutons 
void dessinerBouton(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect rectBtn, SDL_Rect rectTxt, SDL_Point mouse) {
    int survoler = SDL_PointInRect(&mouse, &rectBtn);

    //  Fond du bouton (sombre, légèrement bleuté/violet)
    if (survoler) {
        SDL_SetRenderDrawColor(renderer, 80, 75, 95, 230); // Plus clair au survol
    } else {
        SDL_SetRenderDrawColor(renderer, 50, 45, 65, 200); // revient au normal
    }
    SDL_RenderFillRect(renderer, &rectBtn);

    //  Bordure extérieure claire (Gris/Beige)
    SDL_SetRenderDrawColor(renderer, 150, 145, 130, 255);
    SDL_RenderDrawRect(renderer, &rectBtn);
    
    // Bordure intérieure sombre 
    SDL_Rect bordureInt = {rectBtn.x + 2, rectBtn.y + 2, rectBtn.w - 4, rectBtn.h - 4};
    SDL_SetRenderDrawColor(renderer, 30, 25, 35, 255);
    SDL_RenderDrawRect(renderer, &bordureInt);

    // Dessiner le texte par-dessus
    if (survoler) {
        SDL_SetTextureColorMod(texture, 255, 215, 0); // Or si survolé
    } else {
        SDL_SetTextureColorMod(texture, 180, 180, 255); // Bleuté clair 
    }
    SDL_RenderCopy(renderer, texture, NULL, &rectTxt);
    
    // Réinitialiser la texture
    SDL_SetTextureColorMod(texture, 255, 255, 255);
}

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
    }
    
    int indexMusique = 0;
    Uint32 debutMusique = SDL_GetTicks();
    int fadeDeclenche = 0;
    Uint32 dureeBoucle = 30000;  
    Uint32 fadeOutTime = 5000;

    int fullscreen = 0;
    int windowW = 1024; // Un peu plus grand pour bien voir le panneau
    int windowH = 768;

    SDL_Window *window = SDL_CreateWindow(
        "Menu Principal",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowW,
        windowH,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        fprintf(stderr, "Erreur création fenêtre : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // --- TEXTURES IMAGES ---
    SDL_Texture *backgroundTexture = NULL;
    SDL_Surface *backgroundSurface = IMG_Load("img/menu.jpg");
    if (backgroundSurface) {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);
    }

    SDL_Texture *texLogoMusique = NULL;
    SDL_Surface *surfLogo = IMG_Load("img/logomusique.png");
    if (surfLogo) {
        texLogoMusique = SDL_CreateTextureFromSurface(renderer, surfLogo);
        SDL_FreeSurface(surfLogo);
    }

    // --- POLICES ET TEXTES ---
    TTF_Font *policeMenu = TTF_OpenFont("Fonts/Enchanted Land.otf", 36); 
    TTF_Font *policeTitre = TTF_OpenFont("Fonts/Enchanted Land.otf", 90);

    SDL_Color couleurTexte = {255, 255, 255, 255};
    
    SDL_Texture *texJouer = NULL, *texContinuer = NULL, *texParam = NULL, *texQuitter = NULL;
    SDL_Texture *texSolo = NULL, *texCoop = NULL, *texTitre = NULL, *texVolume = NULL;
    SDL_Texture *texFsOff = NULL, *texFsOn = NULL, *texCroix = NULL;
    
    int wJouer=0, hJouer=0, wContinuer=0, hContinuer=0, wParam=0, hParam=0, wQuitter=0, hQuitter=0;
    int wSolo=0, hSolo=0, wCoop=0, hCoop=0, wTitre=0, hTitre=0, wVolume=0, hVolume=0;
    int wFsOff=0, hFsOff=0, wFsOn=0, hFsOn=0, wCroix=0, hCroix=0;

    if (policeMenu && policeTitre) {
        SDL_Surface *surf = TTF_RenderText_Blended(policeMenu, "Nouvelle partie", couleurTexte);
        texJouer = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texJouer, NULL, NULL, &wJouer, &hJouer); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Continuer", couleurTexte); 
        texContinuer = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texContinuer, NULL, NULL, &wContinuer, &hContinuer); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Options", couleurTexte); 
        texParam = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texParam, NULL, NULL, &wParam, &hParam); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Quitter", couleurTexte);
        texQuitter = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texQuitter, NULL, NULL, &wQuitter, &hQuitter); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Aventure Solo", couleurTexte);
        texSolo = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texSolo, NULL, NULL, &wSolo, &hSolo); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Coop en ligne", couleurTexte);
        texCoop = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texCoop, NULL, NULL, &wCoop, &hCoop); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeTitre, "FOR THE KING", couleurTexte);
        texTitre = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texTitre, NULL, NULL, &wTitre, &hTitre); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Volume Musique", couleurTexte);
        texVolume = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texVolume, NULL, NULL, &wVolume, &hVolume); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Plein ecran : OFF", couleurTexte);
        texFsOff = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texFsOff, NULL, NULL, &wFsOff, &hFsOff); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Plein ecran : ON", couleurTexte);
        texFsOn = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texFsOn, NULL, NULL, &wFsOn, &hFsOn); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "X", couleurTexte);
        texCroix = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texCroix, NULL, NULL, &wCroix, &hCroix); SDL_FreeSurface(surf);
    }

    SDL_Rect btnJouer, btnContinuer, btnParam, btnQuitter, btnSolo, btnCoop, btnMusique, barreVolumeFond, btnFullscreen, btnFermerOptions;
    SDL_Rect rectTxtJouer, rectTxtContinuer, rectTxtParam, rectTxtQuitter, rectTxtSolo, rectTxtCoop, rectTxtTitre, rectTxtVolume, rectTxtFullscreen, rectTxtCroix;
    SDL_Rect panneauPrincipal, panneauSecondaire, panneauOptions;

    int menuActif = 1;
    int afficher_sous_menu_jouer = 0; 
    int afficher_sous_menu_options = 0; 
    int drag_volume = 0; 
    SDL_Event event;

    // Boucle principale
    while (menuActif) {

        // --- MISE A JOUR DES DIMENSIONS ---
        SDL_GetWindowSize(window, &windowW, &windowH);
        
        int btnWidth = windowW / 4; 
        int btnHeight = 45; 
        
        // Placement à gauche de l'écran
        int titleX = windowW / 8;
        int startX = titleX + (wTitre / 2) - (btnWidth / 2); 
        int startY = windowH / 2.5;
        int espacement = btnHeight + 8; // peu d'espace entre les boutons

        // Boutons principaux
        btnJouer     = (SDL_Rect){startX, startY, btnWidth, btnHeight};
        btnContinuer = (SDL_Rect){startX, startY + espacement, btnWidth, btnHeight};
        btnParam     = (SDL_Rect){startX, startY + espacement * 2, btnWidth, btnHeight};
        btnQuitter   = (SDL_Rect){startX, startY + espacement * 3, btnWidth, btnHeight};

        // Sous-boutons "Jouer"
        btnSolo = (SDL_Rect){btnJouer.x + btnJouer.w + 50, btnJouer.y, btnWidth, btnHeight};
        btnCoop = (SDL_Rect){btnSolo.x, btnSolo.y + espacement, btnWidth, btnHeight};

        // Sous-menu "Options"
        rectTxtVolume = (SDL_Rect){btnParam.x + btnParam.w + 50 + (btnWidth - wVolume)/2, btnParam.y, wVolume, hVolume};
        barreVolumeFond = (SDL_Rect){btnParam.x + btnParam.w + 50, btnParam.y + espacement, btnWidth, 20};
        btnFullscreen = (SDL_Rect){btnParam.x + btnParam.w + 50, btnParam.y + espacement + 40, btnWidth, btnHeight}; 

        // Bouton Musique (Mute global en bas à droite)
        btnMusique = (SDL_Rect){windowW - 80, windowH - 80, 60, 60};

        int padding = 20;
        
        // Panneau Principal
        panneauPrincipal = (SDL_Rect){
            startX - padding, 
            startY - padding, 
            btnWidth + (padding * 2), 
            (espacement * 3) + btnHeight + (padding * 2)
        };
        
        // Panneau Secondaire (Jouer)
        panneauSecondaire = (SDL_Rect){
            btnSolo.x - padding, 
            btnSolo.y - padding, 
            btnWidth + (padding * 2), 
            espacement + btnHeight + (padding * 2) 
        };

        // Panneau Options (Volume + Plein Ecran)
        panneauOptions = (SDL_Rect){
            barreVolumeFond.x - padding, 
            rectTxtVolume.y - padding, 
            btnWidth + (padding * 2), 
            espacement + 40 + btnHeight + (padding * 2) 
        };

        // La petite croix pour fermer le menu options (en haut à droite du panneau)
        btnFermerOptions = (SDL_Rect){panneauOptions.x + panneauOptions.w - 35, panneauOptions.y + 5, 30, 30};
        rectTxtCroix = (SDL_Rect){btnFermerOptions.x + (btnFermerOptions.w - wCroix)/2, btnFermerOptions.y + (btnFermerOptions.h - hCroix)/2, wCroix, hCroix};

        // Centrage des textes dans les boutons principaux
        rectTxtJouer = (SDL_Rect){btnJouer.x + (btnJouer.w - wJouer)/2, btnJouer.y + (btnJouer.h - hJouer)/2, wJouer, hJouer};
        rectTxtContinuer = (SDL_Rect){btnContinuer.x + (btnContinuer.w - wContinuer)/2, btnContinuer.y + (btnContinuer.h - hContinuer)/2, wContinuer, hContinuer};
        rectTxtParam = (SDL_Rect){btnParam.x + (btnParam.w - wParam)/2, btnParam.y + (btnParam.h - hParam)/2, wParam, hParam};
        rectTxtQuitter = (SDL_Rect){btnQuitter.x + (btnQuitter.w - wQuitter)/2, btnQuitter.y + (btnQuitter.h - hQuitter)/2, wQuitter, hQuitter};
        
        // Textes sous-menu jouer
        rectTxtSolo = (SDL_Rect){btnSolo.x + (btnSolo.w - wSolo)/2, btnSolo.y + (btnSolo.h - hSolo)/2, wSolo, hSolo};
        rectTxtCoop = (SDL_Rect){btnCoop.x + (btnCoop.w - wCoop)/2, btnCoop.y + (btnCoop.h - hCoop)/2, wCoop, hCoop};
        
        // Titre aligné au-dessus du menu
        rectTxtTitre = (SDL_Rect){titleX, startY - 120, wTitre, hTitre};

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePos = {mouseX, mouseY};

        // --- GESTION AUDIO ---
        if (musique[indexMusique]) {
            Uint32 elapsed = SDL_GetTicks() - debutMusique;
            if (!fadeDeclenche && elapsed >= dureeBoucle - fadeOutTime) {
                Mix_FadeOutMusic(fadeOutTime);
                fadeDeclenche = 1;
            }
            if (!Mix_PlayingMusic()) {
                indexMusique = (indexMusique + 1) % 2;
                Mix_FadeInMusic(musique[indexMusique], 1, 10000);
                debutMusique = SDL_GetTicks();
                fadeDeclenche = 0;
            }
        }

        // --- GESTION EVENEMENTS ---
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) menuActif = 0;
            
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f) {
                fullscreen = !fullscreen;
                SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                SDL_Point p = {event.button.x, event.button.y};

                if (SDL_PointInRect(&p, &btnJouer)) {
                    afficher_sous_menu_jouer = !afficher_sous_menu_jouer;
                    afficher_sous_menu_options = 0; 
                }

                if (SDL_PointInRect(&p, &btnContinuer)) {
                    lancer_jeu = 1;
                    menuActif = 0;
                }

                if (SDL_PointInRect(&p, &btnParam)) {
                    afficher_sous_menu_options = !afficher_sous_menu_options;
                    afficher_sous_menu_jouer = 0; 
                }

                if (SDL_PointInRect(&p, &btnQuitter)) {
                    menuActif = 0;
                }

                if (afficher_sous_menu_jouer) {
                    if (SDL_PointInRect(&p, &btnSolo) || SDL_PointInRect(&p, &btnCoop)) {
                        lancer_jeu = 1;
                        menuActif = 0;
                    }
                }

                if (afficher_sous_menu_options) {
                    if (SDL_PointInRect(&p, &btnFermerOptions)) {
                        afficher_sous_menu_options = 0; // Ferme le sous-menu quand on clique sur X
                    }
                    if (SDL_PointInRect(&p, &barreVolumeFond)) {
                        drag_volume = 1; 
                    }
                    if (SDL_PointInRect(&p, &btnFullscreen)) {
                        fullscreen = !fullscreen;
                        SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                    }
                }

                if (SDL_PointInRect(&p, &btnMusique)) {
                    if (Mix_PausedMusic()) Mix_ResumeMusic();
                    else Mix_PauseMusic();
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                drag_volume = 0; 
            }

            // Gestion du drag de la barre de volume
            if (drag_volume && (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN)) {
                float pourcentage = (float)(mouseX - barreVolumeFond.x) / (float)barreVolumeFond.w;
                if (pourcentage < 0.0f) pourcentage = 0.0f;
                if (pourcentage > 1.0f) pourcentage = 1.0f;
                
                Mix_VolumeMusic((int)(pourcentage * MIX_MAX_VOLUME)); 
            }
        }

        // --- AFFICHAGE ---
        SDL_RenderClear(renderer);

        // Fond global
        if (backgroundTexture) {
            SDL_Rect dest = {0, 0, windowW, windowH};
            SDL_RenderCopy(renderer, backgroundTexture, NULL, &dest);
        }

        // --- DESSIN PANNEAU PRINCIPAL ---
        SDL_SetRenderDrawColor(renderer, 20, 25, 20, 220);
        SDL_RenderFillRect(renderer, &panneauPrincipal);
        SDL_SetRenderDrawColor(renderer, 130, 140, 120, 255);
        for(int i = 0; i < 2; i++) { 
            SDL_Rect b = {panneauPrincipal.x - i, panneauPrincipal.y - i, panneauPrincipal.w + (i*2), panneauPrincipal.h + (i*2)};
            SDL_RenderDrawRect(renderer, &b);
        }

        // Dessin des boutons dans le panneau
        if (texJouer) dessinerBouton(renderer, texJouer, btnJouer, rectTxtJouer, mousePos);
        if (texContinuer) dessinerBouton(renderer, texContinuer, btnContinuer, rectTxtContinuer, mousePos);
        if (texParam) dessinerBouton(renderer, texParam, btnParam, rectTxtParam, mousePos);
        if (texQuitter) dessinerBouton(renderer, texQuitter, btnQuitter, rectTxtQuitter, mousePos);

        // --- DESSIN SOUS-MENU JOUER ---
        if (afficher_sous_menu_jouer) {
            SDL_SetRenderDrawColor(renderer, 20, 25, 20, 220);
            SDL_RenderFillRect(renderer, &panneauSecondaire);
            SDL_SetRenderDrawColor(renderer, 130, 140, 120, 255);
            for(int i = 0; i < 2; i++) {
                SDL_Rect b = {panneauSecondaire.x - i, panneauSecondaire.y - i, panneauSecondaire.w + (i*2), panneauSecondaire.h + (i*2)};
                SDL_RenderDrawRect(renderer, &b);
            }

            if (texSolo) dessinerBouton(renderer, texSolo, btnSolo, rectTxtSolo, mousePos);
            if (texCoop) dessinerBouton(renderer, texCoop, btnCoop, rectTxtCoop, mousePos);
        }

        // --- DESSIN SOUS-MENU OPTIONS ---
        if (afficher_sous_menu_options) {
            SDL_SetRenderDrawColor(renderer, 20, 25, 20, 220);
            SDL_RenderFillRect(renderer, &panneauOptions);
            SDL_SetRenderDrawColor(renderer, 130, 140, 120, 255);
            for(int i = 0; i < 2; i++) {
                SDL_Rect b = {panneauOptions.x - i, panneauOptions.y - i, panneauOptions.w + (i*2), panneauOptions.h + (i*2)};
                SDL_RenderDrawRect(renderer, &b);
            }

            // Dessin du bouton Croix "X"
            dessinerBouton(renderer, texCroix, btnFermerOptions, rectTxtCroix, mousePos);

            // Affichage du texte "Volume Musique"
            SDL_RenderCopy(renderer, texVolume, NULL, &rectTxtVolume);

            // Affichage fond de la barre
            SDL_SetRenderDrawColor(renderer, 50, 45, 65, 255);
            SDL_RenderFillRect(renderer, &barreVolumeFond);
            SDL_SetRenderDrawColor(renderer, 150, 145, 130, 255);
            SDL_RenderDrawRect(renderer, &barreVolumeFond);

            // Affichage remplissage de la barre
            int volumeActuel = Mix_VolumeMusic(-1);
            float ratioVolume = (float)volumeActuel / (float)MIX_MAX_VOLUME;
            SDL_Rect barreVolumeRemplie = {barreVolumeFond.x, barreVolumeFond.y, (int)(barreVolumeFond.w * ratioVolume), barreVolumeFond.h};
            SDL_SetRenderDrawColor(renderer, 180, 180, 255, 255); // Bleuté clair
            SDL_RenderFillRect(renderer, &barreVolumeRemplie);

            // Affichage du bouton Plein écran
            if (fullscreen) {
                rectTxtFullscreen = (SDL_Rect){btnFullscreen.x + (btnFullscreen.w - wFsOn)/2, btnFullscreen.y + (btnFullscreen.h - hFsOn)/2, wFsOn, hFsOn};
                dessinerBouton(renderer, texFsOn, btnFullscreen, rectTxtFullscreen, mousePos);
            } else {
                rectTxtFullscreen = (SDL_Rect){btnFullscreen.x + (btnFullscreen.w - wFsOff)/2, btnFullscreen.y + (btnFullscreen.h - hFsOff)/2, wFsOff, hFsOff};
                dessinerBouton(renderer, texFsOff, btnFullscreen, rectTxtFullscreen, mousePos);
            }
        }

        // --- Musique logo ---
        if (texLogoMusique) {
            SDL_RenderCopy(renderer, texLogoMusique, NULL, &btnMusique);
        }

        // Titre avec une légère ombre
        if (texTitre) {
            SDL_Rect ombre = {rectTxtTitre.x + 3, rectTxtTitre.y + 3, rectTxtTitre.w, rectTxtTitre.h};
            SDL_SetTextureColorMod(texTitre, 0, 0, 0);
            SDL_SetTextureAlphaMod(texTitre, 180);
            SDL_RenderCopy(renderer, texTitre, NULL, &ombre);
            
            SDL_SetTextureColorMod(texTitre, 255, 255, 255);
            SDL_SetTextureAlphaMod(texTitre, 255);
            SDL_RenderCopy(renderer, texTitre, NULL, &rectTxtTitre);
        }

        SDL_RenderPresent(renderer);
    }

    // --- NETTOYAGE ---
    if (texJouer) SDL_DestroyTexture(texJouer);
    if (texContinuer) SDL_DestroyTexture(texContinuer);
    if (texParam) SDL_DestroyTexture(texParam);
    if (texQuitter) SDL_DestroyTexture(texQuitter);
    if (texSolo) SDL_DestroyTexture(texSolo);
    if (texCoop) SDL_DestroyTexture(texCoop);
    if (texTitre) SDL_DestroyTexture(texTitre);
    if (texVolume) SDL_DestroyTexture(texVolume);
    if (texFsOff) SDL_DestroyTexture(texFsOff);
    if (texFsOn) SDL_DestroyTexture(texFsOn);
    if (texCroix) SDL_DestroyTexture(texCroix);
    if (policeMenu) TTF_CloseFont(policeMenu);
    if (policeTitre) TTF_CloseFont(policeTitre);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (texLogoMusique) SDL_DestroyTexture(texLogoMusique);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit(); 
    SDL_Quit();

    if (lancer_jeu == 1) system("./bin/fortheking");
    
    return EXIT_SUCCESS;
}
