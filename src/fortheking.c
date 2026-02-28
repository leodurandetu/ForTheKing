#include <stdio.h>
#include <math.h> // Indispensable pour sqrtf() (maths hexagonales)
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "../lib/carte.h"
#include "../lib/perso.h"

case_t carte[TAILLE_CARTE][TAILLE_CARTE];

#define TAILLE_CASE_MAXI 150
#define TAILLE_CASE_MINI 32
#define RAYON_DECOUVERTE_BROUILLARD 8

int main() {
    printf("For The King!\n");

    // Initialisation globale de la SDL (Vidéo + Audio)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Echec de l'initialisation de la SDL : %s\n", SDL_GetError());
        return -1;
    }

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

    // Génération du monde
    srand((unsigned int)time(NULL));
    init_carte(carte);
    generer_eau(carte);
    generer_biomes(carte);
    placer_monstres(carte);

    if (musique) Mix_FadeInMusic(musique, 1, 3000);

    perso_t *perso = init_perso(MAGE, TAILLE_CARTE / 2, TAILLE_CARTE / 2); 

    devoiler_brouillard_rayon(carte, perso->x, perso->y, RAYON_DECOUVERTE_BROUILLARD);

    int tailleCase = 50;
    int case_selection_x = -1;
    int case_selection_y = -1;
    int perso_selectionne = 0;

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
                            
                            if (perso->y - 1 >= 0) {
                                majAffichage = 1;
                                majBrouillard = 1;
                                perso->y--;
                            }

                            break;
                        case SDL_SCANCODE_A:
                            
                            if (perso->x - 1 >= 0) {
                                majAffichage = 1;
                                majBrouillard = 1;
                                perso->x--;
                            }

                            break;

                        case SDL_SCANCODE_S: 
                            
                            if (perso->y + 1 < TAILLE_CARTE) {
                                majAffichage = 1;
                                majBrouillard = 1;
                                perso->y++;
                            }

                            break;

                        case SDL_SCANCODE_D:

                            if (perso->x + 1 < TAILLE_CARTE) {
                                majAffichage = 1;
                                majBrouillard = 1;
                                perso->x++;
                            }

                            break;

                        default: break;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        int fenetre_taille_x, fenetre_taille_y;
                        SDL_GetRendererOutputSize(renderer, &fenetre_taille_x, &fenetre_taille_y);

                        // Mathématiques de la grille "flat-top"
                        float rayon = tailleCase / 2.0f;                    
                        float hex_w = 2.0f * rayon;                        
                        float hex_h = sqrtf(3) * rayon;                    
                        float espacement_colonnes = hex_w * 0.75f; 

                        // Décalage de la caméra centrée sur le joueur
                        float camX = perso->x * espacement_colonnes;
                        float camY = perso->y * hex_h + (perso->x % 2 ? hex_h / 2 : 0);
                        int decalageX = (int)(fenetre_taille_x / 2 - camX - hex_w / 2);
                        int decalageY = (int)(fenetre_taille_y / 2 - camY - hex_h / 2);

                        // Détection du clic hexagonal par calcul de distance
                        int carte_x = -1, carte_y = -1;
                        float dist_min_carree = rayon * rayon; 

                        // Approximation de la zone pour ne pas vérifier les milliers de cases
                        int approx_x = (int)((e.button.x - decalageX) / espacement_colonnes);
                        int approx_y = (int)((e.button.y - decalageY) / hex_h);

                        // On teste un carré de 5x5 autour de la souris
                        for (int i = approx_y - 2; i <= approx_y + 2; i++) {
                            for (int j = approx_x - 2; j <= approx_x + 2; j++) {
                                if (i >= 0 && i < TAILLE_CARTE && j >= 0 && j < TAILLE_CARTE) {
                                    
                                    float cx = j * espacement_colonnes + hex_w / 2 + decalageX;
                                    float cy = i * hex_h + (j % 2 ? hex_h / 2 : 0) + hex_h / 2 + decalageY;
                                    
                                    // Théorème de Pythagore pour trouver le centre le plus proche
                                    float dx = e.button.x - cx;
                                    float dy = e.button.y - cy;
                                    float dist_carree = dx * dx + dy * dy;

                                    if (dist_carree < dist_min_carree) {
                                        dist_min_carree = dist_carree;
                                        carte_x = j;
                                        carte_y = i;
                                    }
                                }
                            }
                        }

                        // Logique de sélection une fois la case trouvée
                        if (carte_x >= 0 && carte_y >= 0) {
                            if (carte_x == perso->x && carte_y == perso->y) {
                                perso_selectionne = !perso_selectionne;
                                case_selection_x = -1; case_selection_y = -1;
                            } else if (perso_selectionne) {

                                if (carte[carte_y][carte_x].estVisible) {
                                    perso->x = carte_x; perso->y = carte_y;
                                    perso_selectionne = 0;
                                    case_selection_x = -1; case_selection_y = -1;
                                } else {
                                    case_selection_x = carte_x; case_selection_y = carte_y;
                                    perso_selectionne = 0;
                                }

                            } else {
                                case_selection_x = carte_x; case_selection_y = carte_y;
                                perso_selectionne = 0;
                            }
                        } else {
                            // Clic en dehors de la carte
                            case_selection_x = -1; case_selection_y = -1;
                            perso_selectionne = 0;
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
            afficher_carte_sdl(renderer, carte, textures_cases, texture_brouillard, texture_monstre, tailleCase,
                perso->x, perso->y, case_selection_x, case_selection_y, perso_selectionne);
                
            if (texture_perso) {
                afficher_personnage(renderer, texture_perso, perso, tailleCase, perso_selectionne);
            }

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
