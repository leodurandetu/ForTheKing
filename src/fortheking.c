#include <stdio.h>
#include <math.h> 
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/carte.h"
#include "../lib/perso.h"
#include "../lib/affichage_infos.h"

case_t carte[TAILLE_CARTE][TAILLE_CARTE];

#define TAILLE_CASE_MAXI 250
#define TAILLE_CASE_DEPART 150
#define TAILLE_CASE_MINI 100
#define RAYON_DECOUVERTE_BROUILLARD 8

int main() {
    printf("For The King!\n");


    // Initialisation globale de la SDL (Vidéo + Audio)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Echec de l'initialisation de la SDL : %s\n", SDL_GetError());
        return -1;
    }

    if(TTF_Init() == -1) 
    {
        fprintf(stderr, "Erreur TTF_Init: %s\n", TTF_GetError());
        return -1;
    }

    TTF_Font* police;

    if (( police = TTF_OpenFont("Fonts/Enchanted Land.otf", 32)) == NULL) {
        fprintf( stderr , " erreur chargement font \n");
        exit( EXIT_FAILURE );
    }

    SDL_Surface * texte = NULL;
    SDL_Texture * texte_tex = NULL;
    SDL_Rect txtDestRect;

    // Préparation du son
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur SDL_mixer : %s\n", Mix_GetError());
        return -1;
    }

    Mix_Music *musique = Mix_LoadMUS("audio/Main_theme.mp3");

    if (!musique) {
        fprintf(stderr, "Attention: Musique introuvable (%s)\n", Mix_GetError());
    }

    Uint32 debutMusique = SDL_GetTicks();
    int dureeBoucle = 30000;
    int fadeOutTime = 5000;
    int fadeDeclenche = 0;

    // Création de la fenêtre
    SDL_Window* pFenetre = SDL_CreateWindow("For The King!", 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!pFenetre) {
        fprintf(stderr, "Erreur fenêtre : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Création du moteur de rendu (accéléré par la carte graphique)
    SDL_Renderer *renderer = SDL_CreateRenderer(pFenetre, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur renderer : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    char *nom_images[NB_BIOMES] = {
        "img/terre.png", "img/eau.png",
        "img/desert.png", "img/neige.png",
        "img/foret.png"
    };

    SDL_Texture *textures_cases[NB_BIOMES];

    // Chargement sécurisé des textures de la carte
    for (int i = 0; i < NB_BIOMES; i++) {
        SDL_Surface *image = IMG_Load(nom_images[i]);
        if (!image) {
            fprintf(stderr, "Erreur critique : Image %s manquante !\n", nom_images[i]);
            exit(EXIT_FAILURE);
        }

        textures_cases[i] = SDL_CreateTextureFromSurface(renderer, image);
        SDL_FreeSurface(image); // On vide la RAM une fois l'image envoyée à la carte graphique (optionnel mais mieux)
    }

    // Chargement du sprite du personnage
    SDL_Texture *texture_perso = NULL;
    SDL_Surface *img_perso = IMG_Load("img/mage.png");
    if (img_perso) {
        // Rend la couleur blanche (255, 255, 255) transparente
        SDL_SetColorKey(img_perso, SDL_TRUE, SDL_MapRGB(img_perso->format, 255, 255, 255));
        texture_perso = SDL_CreateTextureFromSurface(renderer, img_perso);
        SDL_SetTextureBlendMode(texture_perso, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_perso);
    }

    // Chargement du sprite du monstre (unique pour l'instant)
    SDL_Texture *texture_monstre = NULL;
    SDL_Surface *img_monstre = IMG_Load("img/squelette.png");
    if (img_monstre) {
        texture_monstre = SDL_CreateTextureFromSurface(renderer, img_monstre);
        SDL_SetTextureBlendMode(texture_monstre, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_monstre);
    }

    // Chargement de la texture du brouillard
    SDL_Texture *texture_brouillard = NULL;
    SDL_Surface *img_brouillard = IMG_Load("img/brouillard.png");
    if (img_brouillard) {
        texture_brouillard = SDL_CreateTextureFromSurface(renderer, img_brouillard);
        SDL_SetTextureBlendMode(texture_brouillard, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_brouillard);
    }

    // Chargement de la texture des batiments

    char *nom_images_batiments[2] = {
        "img/campement.png", "img/tour_boss.png"
    };

    SDL_Texture *textures_batiments[2] = {NULL};

    for (int i = 0; i < 4; i++) {
        SDL_Surface *img_batiment = IMG_Load(nom_images_batiments[i]);

        if (img_batiment) {
            textures_batiments[i] = SDL_CreateTextureFromSurface(renderer, img_batiment);
            SDL_SetTextureBlendMode(textures_batiments[i], SDL_BLENDMODE_BLEND);
            SDL_FreeSurface(img_batiment);
        }
    }

    // Chargement des obstacles
    char *nom_images_obstacles[4] = {
        "img/arbres.png", "img/montagnes.png",
        "img/cactus.png", "img/boue.png" 
    };

    SDL_Texture *textures_obstacles[4] = {NULL};

    for (int i = 0; i < 4; i++) {
        SDL_Surface *image_obs = IMG_Load(nom_images_obstacles[i]);
        if (!image_obs) {
            fprintf(stderr, "Attention : Image obstacle %s manquante !\n", nom_images_obstacles[i]);
        } else {
            textures_obstacles[i] = SDL_CreateTextureFromSurface(renderer, image_obs);
            SDL_SetTextureBlendMode(textures_obstacles[i], SDL_BLENDMODE_BLEND); // Active la transparence
            SDL_FreeSurface(image_obs);
        }
    }

    // Génération du monde
    srand((unsigned int)time(NULL));
    init_carte(carte);
    generer_eau(carte);
    generer_biomes(carte);
    ajout_obstacles(carte);
    placer_monstres(carte);
    placer_batiments(carte);

    if (musique) Mix_FadeInMusic(musique, 1, 3000);

    int xApparition;
    int yApparition;

    coords_case_libre(carte, &xApparition, &yApparition);

    perso_t *perso = init_perso(MAGE, xApparition, yApparition); 

    devoiler_brouillard_rayon(carte, perso->x, perso->y, RAYON_DECOUVERTE_BROUILLARD);

    int tailleCase = TAILLE_CASE_DEPART;
    int case_selection_x = -1;
    int case_selection_y = -1;

    int running = 1;
    int majAffichage = 1; // Forcé à 1 pour afficher l'écran dès le lancement

    /*
     * Si majBrouillard = 1, alors on dévoile
     * le brouillard à proximité du joueur
     */
    int majBrouillard = 0;

    while (running) {
        SDL_Event e;

        // Gestion de la boucle musicale
        if (musique) {
            Uint32 elapsed = SDL_GetTicks() - debutMusique;
            if (!fadeDeclenche && elapsed >= dureeBoucle - fadeOutTime) {
                Mix_FadeOutMusic(fadeOutTime);
                fadeDeclenche = 1;
            }
            if (!Mix_PlayingMusic()) {
                Mix_FadeInMusic(musique, 1, 3000);
                debutMusique = SDL_GetTicks();
                fadeDeclenche = 0;
            }
        }

        // Traitement des actions du joueur
        while (SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_KEYDOWN:
                    // Déplacements au clavier
                    switch(e.key.keysym.scancode) {

                        case SDL_SCANCODE_W: 
                            
                            if (perso->y - 1 >= 0 
                                && deplacement_possible(carte, perso, perso->x, perso->y - 1)
                                && case_occupee(carte, perso->x, perso->y - 1) == FAUX
                                && perso->pts_deplacements > 0
                            ) {
                                majAffichage = 1;
                                majBrouillard = 1;
                                perso->y--;
                                perso->pts_deplacements--;
                            }

                            break;
                        case SDL_SCANCODE_A:
                            
                            if (perso->x - 1 >= 0 
                                && deplacement_possible(carte, perso, perso->x - 1, perso->y)
                                && case_occupee(carte, perso->x - 1, perso->y) == FAUX
                                && perso->pts_deplacements > 0
                            ) {
                                majAffichage = 1;
                                majBrouillard = 1;
                                perso->x--;
                                perso->pts_deplacements--;
                            }

                            break;

                        case SDL_SCANCODE_S: 
                            
                            if (perso->y + 1 < TAILLE_CARTE 
                                && deplacement_possible(carte, perso, perso->x, perso->y + 1)
                                && case_occupee(carte, perso->x, perso->y + 1) == FAUX
                                && perso->pts_deplacements > 0
                            ) {
                                majAffichage = 1;
                                majBrouillard = 1;
                                perso->y++;
                                perso->pts_deplacements--;
                            }

                            break;

                        case SDL_SCANCODE_D:

                            if (perso->x + 1 < TAILLE_CARTE 
                                && deplacement_possible(carte, perso, perso->x + 1, perso->y)
                                && case_occupee(carte, perso->x + 1, perso->y) == FAUX
                                && perso->pts_deplacements > 0
                            ) {
                                majAffichage = 1;
                                majBrouillard = 1;
                                perso->x++;
                                perso->pts_deplacements--;
                            }

                            break;

                        case SDL_SCANCODE_N:
                            restaurer_points_deplacements(perso);
                            majAffichage = 1;

                            break;

                        default: break;
                    }
                    break;

                case SDL_MOUSEMOTION:

                    souris_vers_case(e.motion.x, e.motion.y,
                        &case_selection_x, &case_selection_y,
                        tailleCase, perso, renderer);

                    majAffichage = 1;

                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        int carte_x = -1;
                        int carte_y = -1;
                        souris_vers_case(e.button.x, e.button.y, &carte_x, &carte_y, tailleCase, perso, renderer);

                        if (carte_x >= 0 && carte_y >= 0) {
                            int portee = get_pers_movements_points(perso);
                            int distReelle = -1;
                            int cheminTrouve = chemin_valide(carte, perso->x, perso->y, carte_x, carte_y, portee, perso, &distReelle);

                            if (cheminTrouve && distReelle != -1 && distReelle <= portee) {

                                if (case_occupee(carte, carte_x, carte_y) == VRAI) {
                                    case_selection_x = carte_x;
                                    case_selection_y = carte_y;
                                } else {
                                    perso->x = carte_x; 
                                    perso->y = carte_y;
                                    perso->pts_deplacements -= distReelle; 
                                    case_selection_x = -1; 
                                    case_selection_y = -1;
                                }

                            } else {
                                case_selection_x = carte_x; 
                                case_selection_y = carte_y;
                            }
                        
                        }

                        majAffichage = 1;
                        majBrouillard = 1;
                    }
                    break;
                    
                case SDL_MOUSEWHEEL:
                    // Zoom molette
                    tailleCase += e.wheel.y * 5; 
                    if (tailleCase > TAILLE_CASE_MAXI) tailleCase = TAILLE_CASE_MAXI;
                    if (tailleCase < TAILLE_CASE_MINI) tailleCase = TAILLE_CASE_MINI;
                    majAffichage = 1;
                    break;

                case SDL_WINDOWEVENT:
                    // Si on redimensionne la fenêtre, on force le réaffichage
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_EXPOSED) {
                        majAffichage = 1;
                    }
                    break;
            }
        }

        if (majBrouillard) {
            devoiler_brouillard_rayon(carte, perso->x, perso->y, RAYON_DECOUVERTE_BROUILLARD);
            majBrouillard = 0;
        }

        // Mise à jour de l'écran uniquement si nécessaire 
        if (majAffichage) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
            SDL_RenderClear(renderer);

            // Dessine la carte et les contours dorés
            afficher_carte_sdl(renderer, carte, textures_cases, textures_obstacles, texture_brouillard, texture_monstre,
                textures_batiments, tailleCase, perso->x, perso->y, case_selection_x, case_selection_y,
                perso);
                
            if (texture_perso) {
                afficher_personnage(renderer, texture_perso, perso, tailleCase);
            }

            char info_a_afficher[50];

            get_info_personnage(perso, "Pts_deplacements", info_a_afficher);
            
            mise_a_jour_texte(info_a_afficher, renderer, police, &texte, &texte_tex, &txtDestRect);
            /* Ajout du texte en noir */
            SDL_SetRenderDrawColor ( renderer , 0, 0, 0, 255);
            SDL_RenderCopy ( renderer , texte_tex , NULL , &txtDestRect );

            SDL_RenderPresent(renderer);
            majAffichage = 0;
        }

    }

    // Libération propre de la mémoire à la fermeture du jeu
    if (musique) Mix_FreeMusic(musique);
    for (int i = 0; i < NB_BIOMES; i++) {
        if (textures_cases[i]) SDL_DestroyTexture(textures_cases[i]);
    }
    if (texture_perso) SDL_DestroyTexture(texture_perso);
    if (texture_brouillard) SDL_DestroyTexture(texture_brouillard);
    if (texture_monstre) SDL_DestroyTexture(texture_monstre);
    for (int i = 0; i < 2; i++) {
        if (textures_batiments[i]) SDL_DestroyTexture(textures_batiments[i]);
    }
    for (int i = 0; i < 4; i++) {
        if (textures_obstacles[i]) SDL_DestroyTexture(textures_obstacles[i]);
    }
    if (texte_tex) SDL_DestroyTexture(texte_tex);

    TTF_CloseFont(police);

    // Destruction en mémoire des monstres notamment sur la carte
    liberer_memoire_carte(carte);

    // Destruction du personnage
    detruire_perso(&perso);

    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pFenetre);
    SDL_Quit();

    return 0;
}
