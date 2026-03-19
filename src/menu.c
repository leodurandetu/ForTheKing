#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "../lib/menu.h"
#include "../lib/perso.h"

// Layout = mise en page 

const char *NOM_SLOT[] = { "", "Mage", "Assassin", "Brute", "Chasseur"};
const char *CHEMIN_IMG[] = { NULL, "img/mage.png","img/assassin.png","img/brute.png","img/chasseur.png" };

/* Dessine un bouton */
void dessinerBouton(SDL_Renderer *renderer, SDL_Texture *texture,
                    SDL_Rect rectBtn, SDL_Rect rectTxt, SDL_Point mouse) {
    int survoler = SDL_PointInRect(&mouse, &rectBtn);

    if (survoler)
        SDL_SetRenderDrawColor(renderer, 80, 75, 95, 230);
    else
        SDL_SetRenderDrawColor(renderer, 50, 45, 65, 200);
    SDL_RenderFillRect(renderer, &rectBtn);

    SDL_SetRenderDrawColor(renderer, 150, 145, 130, 255);
    SDL_RenderDrawRect(renderer, &rectBtn);

    SDL_Rect bordureInt = {rectBtn.x + 2, rectBtn.y + 2, rectBtn.w - 4, rectBtn.h - 4};
    SDL_SetRenderDrawColor(renderer, 30, 25, 35, 255);
    SDL_RenderDrawRect(renderer, &bordureInt);

    if (survoler)
        SDL_SetTextureColorMod(texture, 255, 215, 0);
    else
        SDL_SetTextureColorMod(texture, 180, 180, 255);
    SDL_RenderCopy(renderer, texture, NULL, &rectTxt);

    SDL_SetTextureColorMod(texture, 255, 255, 255);
}

/* Dessine un panneau sombre avec double bordure */
void dessinerPanneau(SDL_Renderer *renderer, SDL_Rect r) {
    SDL_SetRenderDrawColor(renderer, 20, 25, 20, 220);
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, 130, 140, 120, 255);
    for (int i = 0; i < 2; i++) {
        SDL_Rect b = {r.x - i, r.y - i, r.w + i * 2, r.h + i * 2};
        SDL_RenderDrawRect(renderer, &b);
    }
}

/* Charge une texture depuis un fichier puis rend le blanc transparent */
SDL_Texture *chargerTexture(SDL_Renderer *renderer, const char *chemin) {
    SDL_Surface *surf = IMG_Load(chemin);
    if (!surf) {
        fprintf(stderr, "Erreur chargement image %s : %s\n", chemin, IMG_GetError());
        return NULL;
    }
    SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255, 255, 255));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surf);
    return tex;
}

/* Dessine un slot de selection de personnage */
void dessinerSlot(SDL_Renderer *renderer, TTF_Font *policeNom,
                         TTF_Font *policeGrand, SDL_Rect rect,
                         SlotPerso *slot, SDL_Point mouse,
                         SDL_Texture *textures_perso[NB_SLOTS]) {
    int survole = SDL_PointInRect(&mouse, &rect);
    Uint32 now  = SDL_GetTicks();

    /* Calcul alpha flash sur 300ms */
    Uint8 flash_alpha = 0;
    if (slot->flash_debut > 0) {
        Uint32 elapsed = now - slot->flash_debut;
        if (elapsed < 300)
            flash_alpha = (Uint8)(255 - elapsed * 255 / 300);
        else
            slot->flash_debut = 0;
    }

    /* Fond du slot */
    if (slot->type != SLOT_VIDE)
        SDL_SetRenderDrawColor(renderer, 30, 28, 55, 230);
    else if (survole)
        SDL_SetRenderDrawColor(renderer, 70, 65, 90, 230);
    else
        SDL_SetRenderDrawColor(renderer, 40, 35, 55, 210);
    SDL_RenderFillRect(renderer, &rect);

    /* Bordure doree si survol ou perso present, grise sinon */
    if (survole || slot->type != SLOT_VIDE)
        SDL_SetRenderDrawColor(renderer, 200, 170, 60, 255);
    else
        SDL_SetRenderDrawColor(renderer, 150, 145, 130, 255);
    for (int i = 0; i < 3; i++) {
        SDL_Rect b = {rect.x - i, rect.y - i, rect.w + i * 2, rect.h + i * 2};
        SDL_RenderDrawRect(renderer, &b);
    }

    SDL_Rect bi = {rect.x + 3, rect.y + 3, rect.w - 6, rect.h - 6};
    SDL_SetRenderDrawColor(renderer, 20, 15, 25, 255);
    SDL_RenderDrawRect(renderer, &bi);

    if (slot->type == SLOT_VIDE) {

        /* Gros "+" centre dans le slot */
        SDL_Color col = survole
            ? (SDL_Color){255, 215,   0, 255}
            : (SDL_Color){180, 180, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(policeGrand, "+", col);
        if (s) {
            SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
            int tw, th;
            SDL_QueryTexture(t, NULL, NULL, &tw, &th);
            SDL_Rect dst = {
                rect.x + (rect.w - tw) / 2,
                rect.y + (rect.h - th) / 2,
                tw, th
            };
            SDL_RenderCopy(renderer, t, NULL, &dst);
            SDL_DestroyTexture(t);
            SDL_FreeSurface(s);
        }

    } else {

        /* Image du personnage avec facteur d'echelle par classe
         * pour compenser le blanc autour de chaque sprite */
        SDL_Texture *img = textures_perso[slot->type];
        if (img) {
            float facteur = 1.0f;
            if (slot->type == SLOT_ASSASSIN) facteur = 1.55f;
            if (slot->type == SLOT_CHASSEUR) facteur = 1.45f;
            if (slot->type == SLOT_BRUTE) facteur = 1.1f; // Taille personnage

            int taille = (int)((rect.h - 55) * facteur);

            SDL_Rect dst = {
                rect.x + (rect.w - taille) / 2,
                rect.y + (rect.h - 45 - taille) / 2 + 5,
                taille,
                taille
            };
            SDL_RenderCopy(renderer, img, NULL, &dst);
        }

        /* Nom de la classe en bas du slot */
        SDL_Color colNom = {255, 215, 0, 255};
        SDL_Surface *sn = TTF_RenderText_Blended(policeNom, NOM_SLOT[slot->type], colNom);
        if (sn) {
            SDL_Texture *tn = SDL_CreateTextureFromSurface(renderer, sn);
            int tnw, tnh;
            SDL_QueryTexture(tn, NULL, NULL, &tnw, &tnh);
            SDL_Rect dn = {
                rect.x + (rect.w - tnw) / 2,
                rect.y + rect.h - tnh - 10,
                tnw, tnh
            };
            SDL_RenderCopy(renderer, tn, NULL, &dn);
            SDL_DestroyTexture(tn);
            SDL_FreeSurface(sn);
        }
    }

    /* Flash blanc au clic */
    if (flash_alpha > 0) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, flash_alpha);
        SDL_RenderFillRect(renderer, &rect);
    }
}

int main() {
    int lancer_jeu = 0;

    /* Initialisation SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Erreur SDL : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Erreur IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur SDL_mixer : %s\n", Mix_GetError());
        return -1;
    }

    /* Chargement des musiques */
    Mix_Music *musique[2];
    musique[0] = Mix_LoadMUS("audio/Soundtrack1_menu.mp3");
    musique[1] = Mix_LoadMUS("audio/Soundtrack2_menu.mp3");

    if (!musique[0] || !musique[1])
        fprintf(stderr, "Erreur chargement musique : %s\n", Mix_GetError());

    int    indexMusique  = 0;
    Uint32 debutMusique  = SDL_GetTicks();
    int    fadeDeclenche = 0;
    Uint32 dureeBoucle   = 30000;
    Uint32 fadeOutTime   = 5000;

    /* Creation de la fenetre */
    int fullscreen = 0;
    int windowW = 1024;
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

    /* Chargement des images de fond et logo */
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

    /* Chargement des images de personnages (index 0 = SLOT_VIDE = NULL) */
    SDL_Texture *textures_perso[NB_SLOTS];
    textures_perso[SLOT_VIDE] = NULL;
    for (int i = 1; i < NB_SLOTS; i++)
        textures_perso[i] = chargerTexture(renderer, CHEMIN_IMG[i]);

    /* Chargement des polices */
    TTF_Font *policeMenu  = TTF_OpenFont("Fonts/Enchanted Land.otf",36);
    TTF_Font *policeTitre = TTF_OpenFont("Fonts/Enchanted Land.otf",90);
    TTF_Font *policeGrand = TTF_OpenFont("Fonts/Enchanted Land.otf",120); // Gros "+" 
    TTF_Font *policePerso = TTF_OpenFont("Fonts/Enchanted Land.otf",28); // Noms des slots 

    SDL_Color couleurTexte = {255, 255, 255, 255};

    /* Textures textes : menu principal */
    SDL_Texture *texJouer = NULL, *texContinuer = NULL, *texParam = NULL, *texQuitter = NULL;
    SDL_Texture *texTitre = NULL, *texVolume = NULL;
    SDL_Texture *texFsOff = NULL, *texFsOn = NULL, *texCroix = NULL;

    int wJouer = 0, hJouer = 0, wContinuer = 0, hContinuer = 0, wParam = 0, hParam = 0, wQuitter = 0, hQuitter = 0;
    int wTitre = 0, hTitre = 0, wVolume = 0, hVolume = 0;
    int wFsOff = 0, hFsOff = 0, wFsOn = 0, hFsOn = 0, wCroix = 0, hCroix = 0;

    /* Textures textes : ecran de selection */
    SDL_Texture *texLancer = NULL, *texRetour = NULL, *texChoixTitre = NULL;
    int wLancer = 0, hLancer = 0, wRetour = 0, hRetour = 0 , wChoixTitre = 0, hChoixTitre = 0;

    if (policeMenu && policeTitre) {
        SDL_Surface *surf;

        surf = TTF_RenderText_Blended(policeMenu, "Nouvelle partie", couleurTexte);
        texJouer = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texJouer, NULL, NULL, &wJouer, &hJouer); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Continuer", couleurTexte);
        texContinuer = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texContinuer, NULL, NULL, &wContinuer, &hContinuer); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Options", couleurTexte);
        texParam = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texParam, NULL, NULL, &wParam, &hParam); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "Quitter", couleurTexte);
        texQuitter = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texQuitter, NULL, NULL, &wQuitter, &hQuitter); SDL_FreeSurface(surf);

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

        surf = TTF_RenderText_Blended(policeMenu, "Lancer l'aventure !", couleurTexte);
        texLancer = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texLancer, NULL, NULL, &wLancer, &hLancer); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeMenu, "< Retour", couleurTexte);
        texRetour = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texRetour, NULL, NULL, &wRetour, &hRetour); SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(policeTitre, "Choisissez vos heros", couleurTexte);
        texChoixTitre = SDL_CreateTextureFromSurface(renderer, surf); SDL_QueryTexture(texChoixTitre, NULL, NULL, &wChoixTitre, &hChoixTitre); SDL_FreeSurface(surf);
    }

    /* Declarations des rectangles */
    SDL_Rect btnJouer, btnContinuer, btnParam, btnQuitter;
    SDL_Rect btnMusique, barreVolumeFond, btnFullscreen, btnFermerOptions;
    SDL_Rect rectTxtJouer, rectTxtContinuer, rectTxtParam, rectTxtQuitter;
    SDL_Rect rectTxtTitre, rectTxtVolume;
    SDL_Rect rectTxtFullscreen, rectTxtCroix;
    SDL_Rect panneauPrincipal, panneauOptions;
    SDL_Rect btnCases[3], btnLancer, btnRetour;

    /* Slots personnages */
    SlotPerso slots[3] = {
        {SLOT_VIDE, 0},
        {SLOT_VIDE, 0},
        {SLOT_VIDE, 0}
    };

    /* Variables d'etat */
    EtatMenu etat = ETAT_PRINCIPAL;
    int afficher_options = 0;
    int drag_volume = 0;
    int menuActif = 1;

    SDL_Event event;

    /* Boucle principale */
    while (menuActif) {

        SDL_GetWindowSize(window, &windowW, &windowH);

        /* Calcul du layout(mise en page) commun */
        int btnWidth = windowW / 4;
        int btnHeight = 45;
        int espacement = btnHeight + 8;
        int padding = 20;
        int titleX = windowW / 8;
        int startX = titleX + (wTitre / 2) - (btnWidth / 2);
        int startY = windowH / 2;

        btnJouer = (SDL_Rect){startX, startY, btnWidth, btnHeight};
        btnContinuer = (SDL_Rect){startX, startY + espacement,   btnWidth, btnHeight};
        btnParam = (SDL_Rect){startX, startY + espacement*2, btnWidth, btnHeight};
        btnQuitter = (SDL_Rect){startX, startY + espacement*3, btnWidth, btnHeight};

        /* Options */
        rectTxtVolume = (SDL_Rect){btnParam.x + btnParam.w + 50 + (btnWidth - wVolume)/2, btnParam.y, wVolume, hVolume};
        barreVolumeFond = (SDL_Rect){btnParam.x + btnParam.w + 50, btnParam.y + espacement, btnWidth, 20};
        btnFullscreen = (SDL_Rect){btnParam.x + btnParam.w + 50, btnParam.y + espacement + 40, btnWidth, btnHeight};

        btnMusique = (SDL_Rect){windowW - 80, windowH - 80, 60, 60};

        /* Panneaux */
        panneauPrincipal = (SDL_Rect){
            startX - padding, startY - padding,
            btnWidth + (padding*2), (espacement*3) + btnHeight + (padding*2)};

        panneauOptions = (SDL_Rect){
            barreVolumeFond.x - padding, rectTxtVolume.y - padding,
            btnWidth + (padding*2), espacement + 40 + btnHeight + (padding*2)};

        btnFermerOptions = (SDL_Rect){panneauOptions.x + panneauOptions.w - 35, panneauOptions.y + 5, 30, 30};
        rectTxtCroix = (SDL_Rect){
            btnFermerOptions.x + (btnFermerOptions.w - wCroix)/2,
            btnFermerOptions.y + (btnFermerOptions.h - hCroix)/2,
            wCroix, hCroix};

        /* Centrage des textes dans les boutons */
        rectTxtJouer = (SDL_Rect){btnJouer.x     + (btnJouer.w     - wJouer)/2,     btnJouer.y     + (btnJouer.h     - hJouer)/2,     wJouer,     hJouer};
        rectTxtContinuer = (SDL_Rect){btnContinuer.x + (btnContinuer.w - wContinuer)/2, btnContinuer.y + (btnContinuer.h - hContinuer)/2, wContinuer, hContinuer};
        rectTxtParam = (SDL_Rect){btnParam.x     + (btnParam.w     - wParam)/2,     btnParam.y     + (btnParam.h     - hParam)/2,     wParam,     hParam};
        rectTxtQuitter = (SDL_Rect){btnQuitter.x   + (btnQuitter.w   - wQuitter)/2,   btnQuitter.y   + (btnQuitter.h   - hQuitter)/2,   wQuitter,   hQuitter};
        rectTxtTitre = (SDL_Rect){titleX, startY - 180, wTitre, hTitre};

        /* Layout des 3 slots de selection */
        int slotW = 200, slotH = 280, slotGap = 40;
        int totalW = slotW * 3 + slotGap * 2;
        int slotStartX = (windowW - totalW) / 2;
        int slotY = (windowH - slotH) / 2 - 20;

        for (int i = 0; i < 3; i++)
            btnCases[i] = (SDL_Rect){slotStartX + i * (slotW + slotGap), slotY, slotW, slotH};

        btnRetour = (SDL_Rect){(windowW/2 - btnWidth)/2, slotY + slotH + 40, btnWidth, btnHeight};
        btnLancer = (SDL_Rect){(windowW/2 - btnWidth)/2 + windowW/4, slotY + slotH + 40, btnWidth, btnHeight};

        /* Position souris */
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePos = {mouseX, mouseY};

        /* Gestion audio avec fondu enchaine */
        if (musique[indexMusique]) {
            Uint32 elapsed = SDL_GetTicks() - debutMusique;
            if (!fadeDeclenche && elapsed >= dureeBoucle - fadeOutTime) {
                Mix_FadeOutMusic(fadeOutTime);
                fadeDeclenche = 1;
            }
            if (!Mix_PlayingMusic()) {
                indexMusique = (indexMusique + 1) % 2;
                Mix_FadeInMusic(musique[indexMusique], 1, 10000);
                debutMusique  = SDL_GetTicks();
                fadeDeclenche = 0;
            }
        }

        /* Gestion des evenements */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                menuActif = 0;

            /* Raccourci plein ecran */
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f) {
                fullscreen = !fullscreen;
                SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }

            /* Echap : retour en arriere selon l'etat */
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                if (etat == ETAT_SELECTION_PERSO) etat = ETAT_PRINCIPAL;
                else menuActif = 0;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                SDL_Point p = {event.button.x, event.button.y};

                /* Clics menu principal */
                if (etat == ETAT_PRINCIPAL) {
                    if (SDL_PointInRect(&p, &btnJouer)) {
                        for (int i = 0; i < 3; i++) { slots[i].type = SLOT_VIDE; slots[i].flash_debut = 0; }
                        etat = ETAT_SELECTION_PERSO;
                        afficher_options = 0;
                    }
                    if (SDL_PointInRect(&p, &btnContinuer)) { lancer_jeu = 1; menuActif = 0; }
                    if (SDL_PointInRect(&p, &btnParam))
                        afficher_options = !afficher_options;
                    if (SDL_PointInRect(&p, &btnQuitter))
                        menuActif = 0;

                    if (afficher_options) {
                        if (SDL_PointInRect(&p, &btnFermerOptions)) afficher_options = 0;
                        if (SDL_PointInRect(&p, &barreVolumeFond))  drag_volume = 1;
                        if (SDL_PointInRect(&p, &btnFullscreen)) {
                            fullscreen = !fullscreen;
                            SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                        }
                    }
                }

                /* Clics ecran selection personnages */
                else if (etat == ETAT_SELECTION_PERSO) {
                    for (int i = 0; i < 3; i++) {
                        if (SDL_PointInRect(&p, &btnCases[i])) {
                            slots[i].flash_debut = SDL_GetTicks();
                            /* Cycle : VIDE -> MAGE -> ASSASSIN -> BRUTE -> CHASSEUR -> VIDE */
                            slots[i].type = (slots[i].type + 1) % NB_SLOTS;
                        }
                    }
                    if (SDL_PointInRect(&p, &btnRetour)) etat = ETAT_PRINCIPAL;
                    if (SDL_PointInRect(&p, &btnLancer)) {
                        int has_perso = 0;
                        for (int i = 0; i < 3; i++)
                            if (slots[i].type != SLOT_VIDE) has_perso = 1;
                        if (has_perso) { lancer_jeu = 1; menuActif = 0; }
                    }
                }

                /* Bouton musique toujours accessible */
                if (SDL_PointInRect(&p, &btnMusique)) {
                    if (Mix_PausedMusic()) Mix_ResumeMusic();
                    else Mix_PauseMusic();
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
                drag_volume = 0;

            /* Drag(glisser) de la barre de volume */
            if (drag_volume && (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN)) {
                float pourcentage = (float)(mouseX - barreVolumeFond.x) / (float)barreVolumeFond.w;
                if (pourcentage < 0.0f) pourcentage = 0.0f;
                if (pourcentage > 1.0f) pourcentage = 1.0f;
                Mix_VolumeMusic((int)(pourcentage * MIX_MAX_VOLUME));
            }
        }

        /* Affichage */
        SDL_RenderClear(renderer);

        /* Fond */
        if (backgroundTexture) {
            SDL_Rect dest = {0, 0, windowW, windowH};
            SDL_RenderCopy(renderer, backgroundTexture, NULL, &dest);
        }

        /* Titre (masque sur l'ecran de selection pour liberer de la place) */
        if (texTitre && etat != ETAT_SELECTION_PERSO) {
            SDL_Rect ombre = {rectTxtTitre.x + 3, rectTxtTitre.y + 3, rectTxtTitre.w, rectTxtTitre.h};
            SDL_SetTextureColorMod(texTitre, 0, 0, 0);
            SDL_SetTextureAlphaMod(texTitre, 180);
            SDL_RenderCopy(renderer, texTitre, NULL, &ombre);
            SDL_SetTextureColorMod(texTitre, 255, 255, 255);
            SDL_SetTextureAlphaMod(texTitre, 255);
            SDL_RenderCopy(renderer, texTitre, NULL, &rectTxtTitre);
        }

        /* Affichage menu principal */
        if (etat == ETAT_PRINCIPAL) {

            dessinerPanneau(renderer, panneauPrincipal);
            if (texJouer)     dessinerBouton(renderer, texJouer,     btnJouer,     rectTxtJouer,     mousePos);
            if (texContinuer) dessinerBouton(renderer, texContinuer, btnContinuer, rectTxtContinuer, mousePos);
            if (texParam)     dessinerBouton(renderer, texParam,     btnParam,     rectTxtParam,     mousePos);
            if (texQuitter)   dessinerBouton(renderer, texQuitter,   btnQuitter,   rectTxtQuitter,   mousePos);

            /* Panneau options */
            if (afficher_options) {
                dessinerPanneau(renderer, panneauOptions);
                dessinerBouton(renderer, texCroix, btnFermerOptions, rectTxtCroix, mousePos);

                SDL_RenderCopy(renderer, texVolume, NULL, &rectTxtVolume);

                SDL_SetRenderDrawColor(renderer, 50, 45, 65, 255);
                SDL_RenderFillRect(renderer, &barreVolumeFond);
                SDL_SetRenderDrawColor(renderer, 150, 145, 130, 255);
                SDL_RenderDrawRect(renderer, &barreVolumeFond);

                int volumeActuel = Mix_VolumeMusic(-1);
                float ratioVolume = (float)volumeActuel / (float)MIX_MAX_VOLUME;
                SDL_Rect barreVolumeRemplie = {barreVolumeFond.x, barreVolumeFond.y, (int)(barreVolumeFond.w * ratioVolume), barreVolumeFond.h};
                SDL_SetRenderDrawColor(renderer, 180, 180, 255, 255);
                SDL_RenderFillRect(renderer, &barreVolumeRemplie);

                if (fullscreen) {
                    rectTxtFullscreen = (SDL_Rect){btnFullscreen.x + (btnFullscreen.w - wFsOn)/2,  btnFullscreen.y + (btnFullscreen.h - hFsOn)/2,  wFsOn,  hFsOn};
                    dessinerBouton(renderer, texFsOn, btnFullscreen, rectTxtFullscreen, mousePos);
                } else {
                    rectTxtFullscreen = (SDL_Rect){btnFullscreen.x + (btnFullscreen.w - wFsOff)/2, btnFullscreen.y + (btnFullscreen.h - hFsOff)/2, wFsOff, hFsOff};
                    dessinerBouton(renderer, texFsOff, btnFullscreen, rectTxtFullscreen, mousePos);
                }
            }
        }

        /* Affichage ecran de selection des personnages */
        else if (etat == ETAT_SELECTION_PERSO) {

            /* Titre de l'ecran */
            if (texChoixTitre) {
                int tx = (windowW - wChoixTitre) / 2;
                int ty = slotY - hChoixTitre - 30;
                SDL_Rect om = {tx + 3, ty + 3, wChoixTitre, hChoixTitre};
                SDL_SetTextureColorMod(texChoixTitre, 0, 0, 0);
                SDL_SetTextureAlphaMod(texChoixTitre, 160);
                SDL_RenderCopy(renderer, texChoixTitre, NULL, &om);
                SDL_Rect dr = {tx, ty, wChoixTitre, hChoixTitre};
                SDL_SetTextureColorMod(texChoixTitre, 255, 255, 255);
                SDL_SetTextureAlphaMod(texChoixTitre, 255);
                SDL_RenderCopy(renderer, texChoixTitre, NULL, &dr);
                SDL_SetTextureColorMod(texChoixTitre, 255, 255, 255);
            }

            /* Les 3 slots */
            for (int i = 0; i < 3; i++)
                dessinerSlot(renderer, policePerso, policeGrand, btnCases[i], &slots[i], mousePos, textures_perso);

            /* Bouton Retour */
            SDL_Rect rTxtRetour = {
                btnRetour.x + (btnRetour.w - wRetour)/2,
                btnRetour.y + (btnRetour.h - hRetour)/2,
                wRetour, hRetour};
            dessinerBouton(renderer, texRetour, btnRetour, rTxtRetour, mousePos);

            /* Bouton Lancer : grise si aucun perso selectionne */
            int has_perso = 0;
            for (int i = 0; i < 3; i++)
                if (slots[i].type != SLOT_VIDE) has_perso = 1;

            SDL_Rect rTxtLancer = {
                btnLancer.x + (btnLancer.w - wLancer)/2,
                btnLancer.y + (btnLancer.h - hLancer)/2,
                wLancer, hLancer};

            if (has_perso) {
                dessinerBouton(renderer, texLancer, btnLancer, rTxtLancer, mousePos);
            } else {
                SDL_SetRenderDrawColor(renderer, 35, 30, 40, 160);
                SDL_RenderFillRect(renderer, &btnLancer);
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 180);
                SDL_RenderDrawRect(renderer, &btnLancer);
                SDL_SetTextureColorMod(texLancer, 100, 100, 100);
                SDL_RenderCopy(renderer, texLancer, NULL, &rTxtLancer);
                SDL_SetTextureColorMod(texLancer, 255, 255, 255);
            }
        }

        /* Logo musique toujours visible */
        if (texLogoMusique)
            SDL_RenderCopy(renderer, texLogoMusique, NULL, &btnMusique);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    /* Nettoyage des textures */
    if (texJouer)          SDL_DestroyTexture(texJouer);
    if (texContinuer)      SDL_DestroyTexture(texContinuer);
    if (texParam)          SDL_DestroyTexture(texParam);
    if (texQuitter)        SDL_DestroyTexture(texQuitter);
    if (texTitre)          SDL_DestroyTexture(texTitre);
    if (texVolume)         SDL_DestroyTexture(texVolume);
    if (texFsOff)          SDL_DestroyTexture(texFsOff);
    if (texFsOn)           SDL_DestroyTexture(texFsOn);
    if (texCroix)          SDL_DestroyTexture(texCroix);
    if (texLancer)         SDL_DestroyTexture(texLancer);
    if (texRetour)         SDL_DestroyTexture(texRetour);
    if (texChoixTitre)     SDL_DestroyTexture(texChoixTitre);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (texLogoMusique)    SDL_DestroyTexture(texLogoMusique);

    for (int i = 1; i < NB_SLOTS; i++)
        if (textures_perso[i]) SDL_DestroyTexture(textures_perso[i]);

    /* Nettoyage des polices */
    if (policeMenu)  TTF_CloseFont(policeMenu);
    if (policeTitre) TTF_CloseFont(policeTitre);
    if (policeGrand) TTF_CloseFont(policeGrand);
    if (policePerso) TTF_CloseFont(policePerso);

    /* Nettoyage audio */
    for (int i = 0; i < 2; i++)
        if (musique[i]) Mix_FreeMusic(musique[i]);
    Mix_CloseAudio();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    if (lancer_jeu == 1)
        system("./bin/fortheking");

    return EXIT_SUCCESS;
}
