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
#include "../lib/perlin_noise.h"
#include "../lib/ressources.h"
#include "../lib/init_sdl.h"
#include "../lib/sauvegarde.h"
#include "../lib/sanctuaire_menu.h"

#define TAILLE_CASE_MAXI 250
#define TAILLE_CASE_DEPART 150
#define TAILLE_CASE_MINI 100
#define RAYON_DECOUVERTE_BROUILLARD 5

typedef enum {
    CARTE,
    GAME_OVER,
    COMBAT
} etat_jeu_t;

int main(int argc,char *argv[]) {
    int plein_ecran_depart = 0;
    int charger_sauvegarde = 0;
    int relancer_menu = 0;
    perso_type_t classe_choisie = MAGE; // valeur par défaut

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--plein-ecran") == 0) {
            plein_ecran_depart = 1;
        } else if (strcmp(argv[i], "--classe") == 0 && i + 1 < argc) {
            i++;
            if      (strcmp(argv[i], "assassin") == 0) classe_choisie = ASSASSIN;
            else if (strcmp(argv[i], "brute")    == 0) classe_choisie = BRUTE;
            else if (strcmp(argv[i], "chasseur") == 0) classe_choisie = CHASSEUR;
            else                                        classe_choisie = MAGE;
        } else if (strcmp(argv[i], "--charger-sauvegarde") == 0) {
            charger_sauvegarde = 1;
        }
    }

    // --- Initialisation globale du sdl ---
    if (init_sdl_global() != 0) {
        return EXIT_FAILURE;
    }

    SDL_Surface * texte = NULL;
    SDL_Texture * texte_tex = NULL;
    SDL_Rect txtDestRect;

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

    // --- Chargement des ressources ---
    ressources_jeu_t ressources = {0};

    if (charger_ressources(renderer, &ressources, classe_choisie) != 0) {
        fprintf(stderr, "Erreur critique lors du chargement des ressources.\n");
        liberer_ressources(&ressources);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pFenetre);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // --- Chargement musique ---
    Mix_Music *musique = init_sdl_musique();

    TTF_Font* police = ressources.police_max;
    TTF_Font* police2 = ressources.police_min;

    // Initialisation des vies globales
    int vies_globales = 3;

    combat_t * combat_actuel = NULL;
    vainqueur_t vainqueur = PAS_DE_VAINQUEUR;

    SDL_Point point;
    
    case_t ** carte;
    carte = calloc(TAILLE_CARTE, sizeof(case_t *)); 

    int k;

    for (k = 0; k < TAILLE_CARTE; k++) {
        carte[k] = calloc(TAILLE_CARTE, sizeof(case_t));
    }

    monstre_t * boss_final = creer_boss_final();
    perso_t * perso = NULL;

    // Génération du monde
    perlin_init((unsigned int)time(NULL));
    init_carte(carte);

    if (charger_sauvegarde == 1) {
        perso = malloc(sizeof(perso_t));
        charger_partie("sauvegarde_01.txt", perso, carte);
    } else {
        generer_eau(carte);
        generer_biomes(carte);
        ajout_obstacles(carte);
        placer_sanctuaires(carte);
        placer_monstres(carte);
        placer_batiments(carte);
    }

    preparer_avant_affichage();

    if (musique) Mix_FadeInMusic(musique, 1, 3000);

    if (charger_sauvegarde == 0) {
        int xApparition;
        int yApparition;

        coords_case_libre(carte, &xApparition, &yApparition);
        perso = init_perso(classe_choisie, xApparition, yApparition);

        remplir_brouillard(carte);
        devoiler_brouillard_rayon(carte, perso->x, perso->y, RAYON_DECOUVERTE_BROUILLARD);
    }

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

    etat_jeu_t etat = CARTE;

    int nb_fuites = 0;

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
                                deplacer_perso_si_possible(perso, carte, 0, -1, &majAffichage, &majBrouillard);
                                break;

                            case SDL_SCANCODE_A:
                                deplacer_perso_si_possible(perso, carte, -1, 0, &majAffichage, &majBrouillard);
                                break;

                            case SDL_SCANCODE_S:
                                deplacer_perso_si_possible(perso, carte, 0, 1, &majAffichage, &majBrouillard);
                                break;

                            case SDL_SCANCODE_D:
                                deplacer_perso_si_possible(perso, carte, 1, 0, &majAffichage, &majBrouillard);
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

                                    /* Si la case cliquée est un sanctuaire adjacent au joueur,
                                     * on ouvre directement le menu sans déplacement */
                                    int dx_hex[2][6] = {
                                        { +1,  0, -1, -1,  0, +1 },
                                        { +1,  0, -1, -1,  0, +1 }
                                    };
                                    int dy_hex[2][6] = {
                                        {  0, +1,  0, -1, -1, -1 },
                                        { +1, +1, +1,  0, -1,  0 }
                                    };
                                    int est_voisin_sanctuaire = 0;
                                    for (int dir = 0; dir < 6; dir++) {
                                        int vx = perso->x + dx_hex[perso->x % 2][dir];
                                        int vy = perso->y + dy_hex[perso->x % 2][dir];
                                        if (vx == carte_x && vy == carte_y &&
                                            (carte[carte_y][carte_x].sanctuaires != PAS_DE_SANCTUAIRE)) {
                                            est_voisin_sanctuaire = 1;
                                            break;
                                        }
                                    }
                                    if (est_voisin_sanctuaire) {
                                        afficher_menu_sanctuaire(renderer, police2,
                                            perso, carte, carte_x, carte_y, tailleCase);
                                        majAffichage = 1;
                                    } else {

                                    int portee = get_pers_movements_points(perso);
                                    int distReelle = -1;
                                    int cheminTrouve = chemin_valide(carte, TAILLE_CARTE, perso->x, perso->y, carte_x, carte_y, portee, perso, &distReelle);

                                    if (cheminTrouve && distReelle != -1) {

                                        if (case_occupee(carte, carte_x, carte_y, perso->x, perso->y) == VRAI) {
                                            monstre_t * monstre = carte[carte_y][carte_x].monstre;

                                            if (monstre != NULL && combat_actuel == NULL) {
                                                combat_actuel = creer_combat(perso, monstre);
                                                TTF_Font *gFont = TTF_OpenFont("Fonts/Enchanted Land.otf", 20);

                                                // Conversion coordonnées carte → pixels écran
                                                int windowW, windowH;
                                                SDL_GetWindowSize(pFenetre, &windowW, &windowH);

                                                int centre_x = windowW / 2;
                                                int centre_y = windowH / 2;

                                                // Position pixel du personnage à l'écran (il est toujours centré)
                                                int perso_px = centre_x;
                                                int perso_py = centre_y;

                                                // Taille du sprite à l'écran (même logique que dans afficher_personnage)
                                                int sprite_w = tailleCase;
                                                int sprite_h = tailleCase;

                                                int choix;

                                                choix = afficher_option(renderer, gFont,
                                                                            perso_px, perso_py,
                                                                            sprite_w, sprite_h,
                                                                            nb_fuites >= MAX_FUITE);
                                                if(choix == 1)
                                                {
                                                    ouvrir_fenetre_combat(pFenetre, renderer, carte[carte_y][carte_x].biome, &combat_actuel, carte, &vies_globales);
                                                    etat = COMBAT;
                                                    nb_fuites = 0;
                                                } else if (choix == 0) {
                                                    detruire_combat(&combat_actuel);
                                                    etat = CARTE;
                                                    nb_fuites++;
                                                }

                                                case_selection_x = carte_x;
                                                case_selection_y = carte_y;
                                            } else if (carte[carte_y][carte_x].batiment.type == TOUR_DU_BOSS) {
                                                combat_actuel = creer_combat(perso, boss_final);
                                                TTF_Font *gFont = TTF_OpenFont("Fonts/Enchanted Land.otf", 20);

                                                // Conversion coordonnées carte → pixels écran
                                                int windowW, windowH;
                                                SDL_GetWindowSize(pFenetre, &windowW, &windowH);

                                                int centre_x = windowW / 2;
                                                int centre_y = windowH / 2;

                                                // Position pixel du personnage à l'écran (il est toujours centré)
                                                int perso_px = centre_x;
                                                int perso_py = centre_y;

                                                // Taille du sprite à l'écran (même logique que dans afficher_personnage)
                                                int sprite_w = tailleCase;
                                                int sprite_h = tailleCase;

                                                int choix;

                                                choix = afficher_option(renderer, gFont,
                                                                            perso_px, perso_py,
                                                                            sprite_w, sprite_h,
                                                                            nb_fuites >= MAX_FUITE);
                                                if(choix == 1)
                                                {
                                                    ouvrir_fenetre_combat(pFenetre, renderer, carte[carte_y][carte_x].biome, &combat_actuel, carte, &vies_globales);
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
                                    } // fin else (pas un sanctuaire voisin)
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

                            /* Vérification de la mort du monstre */
                            if(combat_actuel->monstre->sante <= 0) {
                                vainqueur = JOUEUR_VAINQUEUR;
                                combat_termine(renderer, &combat_actuel, carte, vainqueur, &vies_globales);
                                detruire_combat(&combat_actuel);
                                etat = CARTE;
                            } else {
                            
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
                
                    afficher_carte_sdl(renderer, carte, ressources.cases, ressources.obstacles, ressources.brouillard, ressources.monstres,
                        ressources.batiments, ressources.sanctuaires, tailleCase, perso->x, perso->y, case_selection_x, case_selection_y,
                        perso);
                        
                    if (ressources.perso) {
                        afficher_personnage(renderer, ressources.perso, perso, tailleCase);
                    }

                    dessiner_interface_carte(renderer, police2, ressources.perso, perso, clic_gauche, &majAffichage);
                    char info_a_afficher[50];
                    
                    /* Affichage du texte des points de déplacement */
                    get_info_personnage(perso, "Pts_deplacements", info_a_afficher);
                    mise_a_jour_texte(info_a_afficher, renderer, police, &texte, &texte_tex, &txtDestRect);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderCopy(renderer, texte_tex, NULL, &txtDestRect);

                    /* Affichage des vies */
                    if (ressources.vie) {
                        int taille_coeur = 48; 
                        int espace_entre_coeurs = taille_coeur + 10;
                        int x_depart_coeurs = txtDestRect.x + txtDestRect.w + 25; 
                        int y_coeurs = txtDestRect.y + (txtDestRect.h - taille_coeur) / 2;

                        for (int i = 0; i < vies_globales; i++) {
                            SDL_Rect rect_vie = { x_depart_coeurs + (i * espace_entre_coeurs), y_coeurs, taille_coeur, taille_coeur }; 
                            SDL_RenderCopy(renderer, ressources.vie, NULL, &rect_vie);
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

    // Sauvegarde dans un fichier
    sauvegarder_partie("sauvegarde_01.txt", perso, carte);

    // --- Nettoyage des ressources ---
    liberer_ressources(&ressources);
    
    // Libération de la musique 
    if (musique) Mix_FreeMusic(musique);

    if (texte_tex) SDL_DestroyTexture(texte_tex);


    if (combat_actuel != NULL) {
        combat_termine(renderer, &combat_actuel, carte, PAS_DE_VAINQUEUR, &vies_globales);
        detruire_combat(&combat_actuel);
    }

    if (boss_final != NULL) {
        free(boss_final);
    }

    // Destruction en mémoire des monstres notamment sur la carte
    liberer_memoire_carte(&carte, TAILLE_CARTE);

    // Destruction du personnage
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
