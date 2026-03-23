#include <stdio.h>
#include <math.h> 
#include <time.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/carte.h"
#include "../lib/perso.h"
#include "../lib/affichage_infos.h"
#include "../lib/combat.h"
#include "../lib/pause_menu.h"
#include "../lib/affichage_carte.h"
#include "../lib/option.h"
#include "../lib/inventaire.h"
#include "../lib/affichage_commun.h"

#define TAILLE_CASE_MAXI 250
#define TAILLE_CASE_DEPART 150
#define TAILLE_CASE_MINI 100
#define RAYON_DECOUVERTE_BROUILLARD 5

typedef enum {
    CARTE,
    GAME_OVER,
    COMBAT
} EtatJeu_t;

int main(int argc,char *argv[]) {

    int plein_ecran_depart = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--plein-ecran") == 0)
            plein_ecran_depart = 1;
    }

    int relancer_menu = 0;
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

    TTF_Font* police2;

    if (( police2 = TTF_OpenFont("Fonts/Enchanted Land.otf", 24)) == NULL) {
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

    if (plein_ecran_depart)
        SDL_SetWindowFullscreen(pFenetre, SDL_WINDOW_FULLSCREEN_DESKTOP);

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
        "img/terrev1.png", "img/eauv2.png",
        "img/desertv1.png", "img/neigev3.png",
        "img/foretv1.png"
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
    SDL_Texture *texture_squelette = NULL;
    SDL_Surface *img_squelette = IMG_Load("img/squelette.png");
    if (img_squelette) {
        texture_squelette = SDL_CreateTextureFromSurface(renderer, img_squelette);
        SDL_SetTextureBlendMode(texture_squelette, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_squelette);
    }

    // Chargement du troll
    SDL_Texture *texture_troll = NULL;
    SDL_Surface *img_troll = IMG_Load("img/troll.png");
    if (img_troll) {
        texture_troll = SDL_CreateTextureFromSurface(renderer, img_troll);
        SDL_SetTextureBlendMode(texture_troll, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_troll);
    }

    SDL_Texture *textures_monstres[2] = {texture_squelette, texture_troll};


    // Chargement de la texture du brouillard
    SDL_Texture *texture_brouillard = NULL;
    SDL_Surface *img_brouillard = IMG_Load("img/brouillard.png");
    if (img_brouillard) {
        texture_brouillard = SDL_CreateTextureFromSurface(renderer, img_brouillard);
        SDL_SetTextureBlendMode(texture_brouillard, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_brouillard);
    }

    // Chargement de la texture des batiments

    char *nom_images_batiments[3] = {
        "img/campement.png", "img/tour_boss.png", "img/tombe.png"
    };

    SDL_Texture *textures_batiments[3] = {NULL};

    for (int i = 0; i < 3; i++) {
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

    // Chargement des sanctuaires
    char *nom_images_sanctuaires[4] = {
        "img/sanc_puissance.png", // Correspond à PUISSANCE (index 0)
        "img/sanc_rapidité.png",  // Correspond à RAPIDITE (index 1)
        "img/sanc_intelligence.png", // Correspond à INTELLIGENCE (index 2)
        "img/sanc_experience.png" // Correspond à EXPERIANCE (index 3)
    };

    SDL_Texture *textures_sanctuaires[4] = {NULL};

    for (int i = 0; i < 4; i++) {
        SDL_Surface *image_sanc = IMG_Load(nom_images_sanctuaires[i]);
        if (!image_sanc) {
            fprintf(stderr, "Attention : Image sanctuaire %s manquante !\n", nom_images_sanctuaires[i]);
        } else {
            textures_sanctuaires[i] = SDL_CreateTextureFromSurface(renderer, image_sanc);
            SDL_SetTextureBlendMode(textures_sanctuaires[i], SDL_BLENDMODE_BLEND);
            SDL_FreeSurface(image_sanc);
        }
    }

    // Chargement de l'image de vie 
    SDL_Texture *texture_vie = NULL;
    SDL_Surface *img_vie = IMG_Load("img/vie.png");
    if (img_vie) {
        texture_vie = SDL_CreateTextureFromSurface(renderer, img_vie);
        SDL_SetTextureBlendMode(texture_vie, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_vie);
    }
    
    // Initialisation des vies globales
    int vies_globales = 3;

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

    combat_t * combat_actuel = NULL;
    vainqueur_t vainqueur = PAS_DE_VAINQUEUR;

    SDL_Point point;

    inventaire_t inventaire_perso;

    //SDL_Texture *messageTexture = NULL;
    //char messageTexte[256] = "";

    case_t ** carte;
    
    carte = malloc(sizeof(case_t) * TAILLE_CARTE);

    int k;

    for (k = 0; k < TAILLE_CARTE; k++) {
        carte[k] = malloc(sizeof(case_t) * TAILLE_CARTE);
    }

    // Génération du monde
    srand((unsigned int)time(NULL));
    init_carte(carte);
    generer_eau(carte);
    generer_biomes(carte);
    ajout_obstacles(carte);
    placer_sanctuaires(carte);
    placer_monstres(carte);
    placer_batiments(carte);

    preparer_avant_affichage();
    initialiser_inventaire(&inventaire_perso);

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

    int clic_gauche = 0;

    EtatJeu_t etat = CARTE;

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

                    if (etat == GAME_OVER) {

                        if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                            running = 0;
                        }

                    } else {

                        // Déplacements au clavier
                        switch(e.key.keysym.scancode) {

                            // Menu de pause sur Echap
                            case SDL_SCANCODE_ESCAPE:
                                int windowW, windowH;
                                SDL_GetWindowSize(pFenetre, &windowW, &windowH);
                                
                                int choix = afficher_menu_pause(renderer, police, windowW, windowH);
                                
                                if (choix == 1) { // L'utilisateur a cliqué sur Quitter
                                    running = 0;  
                                    relancer_menu = 1;
                                }
                                
                                majAffichage = 1; // On force le rafraîchissement de la carte au retour
                                break;

                            case SDL_SCANCODE_W: 
                                if (perso->y - 1 >= 0 
                                    && deplacement_possible(carte, perso->x, perso->y - 1)
                                    && case_occupee(carte, perso->x, perso->y - 1, perso->x, perso->y) == FAUX
                                    && perso->pts_deplacements > 0) {
                                    majAffichage = 1;
                                    majBrouillard = 1;
                                    perso->y--;
                                    perso->pts_deplacements--;
                                }
                                break;
                                
                            case SDL_SCANCODE_A:
                                
                                if (perso->x - 1 >= 0 
                                    && deplacement_possible(carte, perso->x - 1, perso->y)
                                    && case_occupee(carte, perso->x - 1, perso->y, perso->x, perso->y) == FAUX
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
                                    && deplacement_possible(carte, perso->x, perso->y + 1)
                                    && case_occupee(carte, perso->x, perso->y + 1, perso->x, perso->y) == FAUX
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
                                    && deplacement_possible(carte, perso->x + 1, perso->y)
                                    && case_occupee(carte, perso->x + 1, perso->y, perso->x, perso->y) == FAUX
                                    && perso->pts_deplacements > 0
                                ) {
                                    majAffichage = 1;
                                    majBrouillard = 1;
                                    perso->x++;
                                    perso->pts_deplacements--;
                                }

                                break;

                            case SDL_SCANCODE_N:

                                if (perso->pts_deplacements <= 0) {
                                    restaurer_points_deplacements(perso);
                                    deplacer_monstres(renderer, carte, perso, &combat_actuel,&vies_globales);
                                    majAffichage = 1;
                                }

                                break;

                            default: break;
                        }

                    }

                    break;

                case SDL_MOUSEMOTION:

                    if (etat == CARTE) {
                        souris_vers_case(e.motion.x, e.motion.y,
                            &case_selection_x, &case_selection_y,
                            tailleCase, perso, renderer);
                        majAffichage = 1;
                    } else if (etat == COMBAT) {
                        point.x = e.motion.x;
                        point.y = e.motion.y;

                        combat_actuel->survole_bouton_leger = SDL_PointInRect(&point, &(combat_actuel->bouton_leger));
                        combat_actuel->survole_bouton_lourd = SDL_PointInRect(&point, &(combat_actuel->bouton_lourd));

                        majAffichage = 1;
                    }

                    break;

                case SDL_MOUSEBUTTONDOWN:

                    if (etat == CARTE) {

                        if (e.button.button == SDL_BUTTON_LEFT) {
                            clic_gauche = 1; 

                            int fen_w, fen_h;
                            SDL_GetRendererOutputSize(renderer, &fen_w, &fen_h);
                            
                            int largeur_menu = 300;
                            int hauteur_menu = 160;
                            int x_menu = 20; // Placé à gauche
                            int y_menu = (fen_h - hauteur_menu - 15);

                            SDL_Rect rect_interface = { x_menu, y_menu, largeur_menu, hauteur_menu };
                            SDL_Point point_clic = { e.button.x, e.button.y };

                            // Si le clic est dans l'interface, on ne fait rien pour la carte
                            if (SDL_PointInRect(&point_clic, &rect_interface)) {
                                majAffichage = 1; 
                            } else {
                                int carte_x = -1;
                                int carte_y = -1;
                                souris_vers_case(e.button.x, e.button.y, &carte_x, &carte_y, tailleCase, perso, renderer);

                                if (carte_x >= 0 && carte_y >= 0) {
                                    int portee = get_pers_movements_points(perso);
                                    int distReelle = -1;
                                    int cheminTrouve = chemin_valide(carte, TAILLE_CARTE, perso->x, perso->y, carte_x, carte_y, portee, perso, &distReelle);

                                    if (cheminTrouve && distReelle != -1) {

                                        if (case_occupee(carte, carte_x, carte_y, perso->x, perso->y) == VRAI) {
                                            monstre_t * monstre = carte[carte_y][carte_x].monstre;

                                            if (monstre != NULL && combat_actuel == NULL) {
                                                combat_actuel = creer_combat(perso, monstre);
                                                int choix = ouvrir_fenetre_choix(combat_actuel);

                                                if(choix == 1)
                                                {
                                                    ouvrir_fenetre_combat(pFenetre, renderer, &combat_actuel, carte, &vies_globales);
                                                    etat = COMBAT;
                                                } else if (choix == 0) {
                                                    detruire_combat(&combat_actuel);
                                                    etat = CARTE;
                                                }

                                                case_selection_x = carte_x;
                                                case_selection_y = carte_y;
                                            } else {
                                                case_selection_x = carte_x;
                                                case_selection_y = carte_y;
                                            }

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
                                majBrouillard = 1;
                            }
                            
                            majAffichage = 1;
                        }

                    } else if (etat == COMBAT) {

                        if (combat_actuel != NULL) {

                            if (e.button.button == SDL_BUTTON_LEFT) {
                                clic_gauche = 1;
                            }

                            point.x = e.button.x;
                            point.y = e.button.y;

                            if(combat_actuel->tour == TOUR_JOUEUR) {

                                if (SDL_PointInRect(&point, &(combat_actuel->bouton_leger))) {
                                    printf("Attaque légère\n");
                                    attaque_legere(combat_actuel);
                                    combat_actuel->tour = changer_tour(combat_actuel);
                                    printf("Le joueur a joué\n");
                                    printf("santé monstre : %d",combat_actuel->monstre->sante);
                                    

                                } else if (SDL_PointInRect(&point, &(combat_actuel->bouton_lourd))) {
                                    printf("Attaque lourde\n");
                                    attaque_lourde(combat_actuel);
                                    combat_actuel->tour = changer_tour(combat_actuel);
                                    printf("Le joueur a joué\n");
                                    printf("santé monstre : %d",combat_actuel->monstre->sante);
                                }

                            }

                            if (combat_actuel->tour == TOUR_MONSTRE) {
                                printf("\nTour du monstre\n");
                                choix_attaque_monstre(combat_actuel);
                                combat_actuel->tour = changer_tour(combat_actuel);
                                printf("Le monstre a joué\n");
                                printf("Santé joueur : %d\n",combat_actuel->perso->sante);

                                majAffichage = 1;
                            }

                            /* Vérification de la mort du joueur */
                            if(combat_actuel->perso->sante <= 0) {
                                
                                vies_globales--; 

                                if (vies_globales >= 0) {
                                    // S'il reste des vies, on ressuscite directement dans le combat
                                    afficher_message_combat(combat_actuel, "Une vie est utilisee...");

                                    // On lui redonne ses 75% de PV max
                                    combat_actuel->perso->sante = (int)(0.75f * combat_actuel->perso->sante_max);
                                    
                                    majAffichage = 1; 
                                } else {
                                    // Plus de vies du tout : Fin de l'aventure
                                    afficher_message_combat(combat_actuel, "Combat terminé.");
                                    
                                    vainqueur = MONSTRE_VAINQUEUR;
                                    combat_termine(renderer, &combat_actuel, carte, vainqueur, &vies_globales);
                                    detruire_combat(&combat_actuel);
                                    etat = CARTE;
                                }

                            } 
                            /* Vérification de la mort du monstre */
                            else if(combat_actuel->monstre->sante <= 0) {
                                vainqueur = JOUEUR_VAINQUEUR;
                                combat_termine(renderer, &combat_actuel, carte, vainqueur, &vies_globales);
                                detruire_combat(&combat_actuel);
                                etat = CARTE;
                            }

                        }

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

                    if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                        running = 0;
                    }

                    break;


            }
        }

        if (perso->mort == 1 && etat != GAME_OVER) {
            etat = GAME_OVER;
            majAffichage = 1;
        }

        if (majBrouillard) {
            devoiler_brouillard_rayon(carte, perso->x, perso->y, RAYON_DECOUVERTE_BROUILLARD);
            majBrouillard = 0;
        }

        if (majAffichage) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
            SDL_RenderClear(renderer);

            if (etat == COMBAT) {

                if (majAffichage) {
                    maj_affichage_fenetre_combat(combat_actuel, clic_gauche);
                    majAffichage = 0;
                }

                clic_gauche = 0;
            } else {
                /* On dessine toujours la carte */
                if (etat == CARTE || etat == GAME_OVER) {
                    afficher_carte_sdl(renderer, carte, textures_cases, textures_obstacles, texture_brouillard, textures_monstres,
                        textures_batiments, textures_sanctuaires, tailleCase, perso->x, perso->y, case_selection_x, case_selection_y,
                        perso);
                        
                    if (texture_perso) {
                        afficher_personnage(renderer, texture_perso, perso, tailleCase);
                    }

                    dessiner_interface_carte(renderer, police2, texture_perso, perso, clic_gauche, &majAffichage);
                    char info_a_afficher[50];
                    
                    /* Affichage du texte des points de déplacement */
                    get_info_personnage(perso, "Pts_deplacements", info_a_afficher);
                    mise_a_jour_texte(info_a_afficher, renderer, police, &texte, &texte_tex, &txtDestRect);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderCopy(renderer, texte_tex, NULL, &txtDestRect);

                    /* Affichage des vies */
                    if (texture_vie) {
                        int taille_coeur = 48; 
                        int espace_entre_coeurs = taille_coeur + 10;
                        int x_depart_coeurs = txtDestRect.x + txtDestRect.w + 25; 
                        int y_coeurs = txtDestRect.y + (txtDestRect.h - taille_coeur) / 2;

                        for (int i = 0; i < vies_globales; i++) {
                            SDL_Rect rect_vie = { x_depart_coeurs + (i * espace_entre_coeurs), y_coeurs, taille_coeur, taille_coeur }; 
                            SDL_RenderCopy(renderer, texture_vie, NULL, &rect_vie);
                        }
                    }
                } 
                
                /* Si fin aventure, on ajoute le filtre sombre et le message */
                if (etat == GAME_OVER) {
                    int w, h;
                    SDL_GetRendererOutputSize(renderer, &w, &h);

                    // Voile noir semi-transparent sur toute la carte
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160); 
                    SDL_Rect ecran_complet = {0, 0, w, h};
                    SDL_RenderFillRect(renderer, &ecran_complet);

                    // Boîte
                    int box_w = 400;
                    int box_h = 120;
                    SDL_Rect fond = { (w - box_w)/2, (h - box_h)/2, box_w, box_h };

                    SDL_SetRenderDrawColor(renderer, 25, 30, 25, 245); 
                    SDL_RenderFillRect(renderer, &fond);
                    
                    SDL_SetRenderDrawColor(renderer, 140, 150, 120, 255);
                    SDL_RenderDrawRect(renderer, &fond);
                    
                    SDL_Rect fond_int = { fond.x + 1, fond.y + 1, fond.w - 2, fond.h - 2 };
                    SDL_RenderDrawRect(renderer, &fond_int);

                    // Texte central
                    SDL_Color blanc = {255, 255, 255, 255};
                    SDL_Surface *surf = TTF_RenderUTF8_Blended(police, "L'aventure prend fin", blanc);
                    if (surf) {
                        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
                        SDL_Rect pos = { (w - surf->w)/2, (h - surf->h)/2, surf->w, surf->h };
                        SDL_RenderCopy(renderer, tex, NULL, &pos);
                        SDL_DestroyTexture(tex);
                        SDL_FreeSurface(surf);
                    }
                }

                SDL_RenderPresent(renderer);
                
                /* Transition vers le menu */
                if (etat == GAME_OVER) {
                    SDL_Delay(2000);     
                    relancer_menu = 1;   
                    running = 0;         
                }

                majAffichage = 0;
            }

        }
            
        clic_gauche = 0;
    }

    // Libération propre de la mémoire à la fermeture du jeu
    if (musique) Mix_FreeMusic(musique);
    for (int i = 0; i < NB_BIOMES; i++) {
        if (textures_cases[i]) SDL_DestroyTexture(textures_cases[i]);
    }
    if (texture_perso) SDL_DestroyTexture(texture_perso);
    if (texture_brouillard) SDL_DestroyTexture(texture_brouillard);
    if (texture_squelette) SDL_DestroyTexture(texture_squelette);
    if (texture_troll) SDL_DestroyTexture(texture_troll);
    for(int i = 0; i < 4; i++){
        if(textures_sanctuaires[i]) SDL_DestroyTexture(textures_sanctuaires[i]);
    }
    for (int i = 0; i < 3; i++) {
        if (textures_batiments[i]) SDL_DestroyTexture(textures_batiments[i]);
    }
    for (int i = 0; i < 4; i++) {
        if (textures_obstacles[i]) SDL_DestroyTexture(textures_obstacles[i]);
    }
    if (texte_tex) SDL_DestroyTexture(texte_tex);

    //if (messageTexture) SDL_DestroyTexture(messageTexture);

    TTF_CloseFont(police);
    TTF_CloseFont(police2);

    if (combat_actuel != NULL) {
        combat_termine(renderer, &combat_actuel, carte, PAS_DE_VAINQUEUR, &vies_globales);
        detruire_combat(&combat_actuel);
    }

    // Destruction en mémoire des monstres notamment sur la carte
    liberer_memoire_carte(&carte, TAILLE_CARTE);

    // Destruction du personnagec
    detruire_perso(&perso);

    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pFenetre);
    SDL_Quit();

    if (relancer_menu == 1) {
        system("./bin/menu");
    }

    return 0;
}

