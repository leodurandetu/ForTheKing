#include "../lib/combat.h"

/* Leo
 * Cette fonction permet d'ouvrir
 * une fenêtre de combat, et donc
 * également de commencer un combat
 * en parallèle.
 */
void ouvrir_fenetre_combat(SDL_Window * pFenetrePrincipal, SDL_Renderer * rendererPrincipal, biome_t biome,
    combat_t ** combat, case_t ** carte, int *vies_globale1s, SDL_Texture * portraitPerso) {

    /*Initialisation du décodeur MP3 et du système audio SDL_mixer*/
    Mix_Init(MIX_INIT_MP3);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Mix_OpenAudio error: %s\n", Mix_GetError());
    }

    /*Chargement des bruitages d'attaque selon le type de personnage/*/
    switch((*combat)->perso->type){
        case BRUTE : 
            (*combat)->son_attaque_legere = Mix_LoadWAV("audio/attaque_marteau.mp3");
            (*combat)->son_attaque_lourde = Mix_LoadWAV("audio/attaque_marteau.mp3");
            break;
        case CHASSEUR : 
            (*combat)->son_attaque_legere = Mix_LoadWAV("audio/attaque_fleche.mp3");
            (*combat)->son_attaque_lourde = Mix_LoadWAV("audio/attaque_fleche.mp3");
            break;
        case ASSASSIN : 
            (*combat)->son_attaque_legere = Mix_LoadWAV("audio/attaque_epee.mp3");
            (*combat)->son_attaque_lourde = Mix_LoadWAV("audio/attaque_epee.mp3");
            break;
        case MAGE : 
            (*combat)->son_attaque_legere = Mix_LoadWAV("audio/explosion.mp3");
            (*combat)->son_attaque_lourde = Mix_LoadWAV("audio/explosion.mp3");
            break;
        default : 
            (*combat)->son_attaque_legere = Mix_LoadWAV("audio/sound_effect.mp3");
            (*combat)->son_attaque_lourde = Mix_LoadWAV("audio/sound_effect.mp3");
            break;
    }
    
    (*combat)->pFenetre = pFenetrePrincipal;

    if (!((*combat)->pFenetre)) {
        fprintf(stderr, "Erreur fenêtre : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Création du moteur de rendu (accéléré par la carte graphique)
    (*combat)->renderer = rendererPrincipal;
    if (!((*combat)->renderer)) {
        fprintf(stderr, "Erreur renderer : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Chargement du sprite du personnage
    (*combat)->texture_perso = portraitPerso;
 
    // Chargement du sprite du monstre en fonction de son type(troll et squelette pour le moment)
    (*combat)->texture_monstre = NULL;
    SDL_Surface *img_monstre = NULL;

    if ((*combat)->monstre->type == SQUELETTE) {
        img_monstre = IMG_Load("img/squelette.png");
    } else if ((*combat)->monstre->type == TROLL) {
        img_monstre = IMG_Load("img/troll.png");
    } else if ((*combat)->monstre->type == YETI) {
        img_monstre = IMG_Load("img/yeti.png");
    } else if ((*combat)->monstre->type == VER_GEANT) {
        img_monstre = IMG_Load("img/ver_geant.png");
    } else if ((*combat)->monstre->type == BOSS_FINAL) {
        img_monstre = IMG_Load("img/boss_final.png");
    }

    if (img_monstre) {
        // SDL_SetColorKey(img_monstre, SDL_TRUE, SDL_MapRGB(img_monstre->format, 255, 255, 255));
        (*combat)->texture_monstre = SDL_CreateTextureFromSurface((*combat)->renderer, img_monstre);
        SDL_SetTextureBlendMode((*combat)->texture_monstre, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_monstre);
    } else {
        fprintf(stderr, "Erreur chargement image monstre : %s\n", IMG_GetError());
    }

    (*combat)->biome = biome;

    char *fond_biome;

    if ((*combat)->monstre->type == BOSS_FINAL) {
        fond_biome = "img/fond_chateau.png";
    } else if (biome == TERRE) {
        fond_biome = "img/fond_terre.png";
    } else if (biome == DESERT) {
        fond_biome = "img/fond_desert.png";
    } else if (biome == NEIGE) {
        fond_biome = "img/fond_neige.png";
    } else {
        fond_biome = "img/fond_foret.png";
    }
    
    // Chargement du sprite du fond d'écran
    (*combat)->texture_fond_ecran = NULL;
    SDL_Surface *img_fond = IMG_Load(fond_biome);
    if (img_fond) {
        (*combat)->texture_fond_ecran = (SDL_CreateTextureFromSurface((*combat)->renderer, img_fond));
        SDL_SetTextureBlendMode((*combat)->texture_fond_ecran, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_fond);
    }

    // Chargement des symboles de l'attaque legère
    (*combat)->texture_attaque_legere = NULL;
    SDL_Surface *img_legere = NULL;
    switch((*combat)->perso->type){
        case BRUTE : img_legere = IMG_Load("img/attaque_massue_leger.png");break;
        case CHASSEUR : img_legere = IMG_Load("img/attaque_fleche_leger.png");break;
        case ASSASSIN : img_legere = IMG_Load("img/attaque_epee_leger.png");break;
        case MAGE : img_legere = IMG_Load("img/attaque_mage_leger.png");break;
        default : img_legere = IMG_Load("img/attaque_l.png");break;
    }
    if (img_legere) {
        (*combat)->texture_attaque_legere = SDL_CreateTextureFromSurface((*combat)->renderer, img_legere);
        SDL_SetTextureBlendMode((*combat)->texture_attaque_legere, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_legere);
    }

    // Chargement des symboles de l'attaque lourde
    (*combat)->texture_attaque_lourde = NULL;
    SDL_Surface *img_lourde = NULL;
    switch((*combat)->perso->type){
        case BRUTE : img_lourde = IMG_Load("img/attaque_massue_lourd.png");break;
        case CHASSEUR : img_lourde = IMG_Load("img/attaque_fleche_lourd.png");break;
        case ASSASSIN : img_lourde = IMG_Load("img/attaque_epee_lourd.png");break;
        case MAGE : img_lourde = IMG_Load("img/attaque_mage_lourd.png");break;
        default : img_lourde = IMG_Load("img/attaque_L1.png");break;
    } 
    if (img_lourde) {
        (*combat)->texture_attaque_lourde = SDL_CreateTextureFromSurface((*combat)->renderer, img_lourde);
        SDL_SetTextureBlendMode((*combat)->texture_attaque_lourde, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_lourde);
    }

    TTF_Init();
    (*combat)->font = TTF_OpenFont("Fonts/Enchanted Land.otf", 24);

    SDL_Color blanc = {255, 255, 255, 255}; // blanc

    // Texte bouton léger
    SDL_Surface *surf_leger = TTF_RenderUTF8_Blended((*combat)->font, "Attaque Légère", blanc);

    if (surf_leger) {
        (*combat)->texture_texte_leger = SDL_CreateTextureFromSurface((*combat)->renderer, surf_leger);
        (*combat)->rect_texte_leger.w = surf_leger->w;
        (*combat)->rect_texte_leger.h = surf_leger->h;
        SDL_FreeSurface(surf_leger);
    }

    // Texte bouton lourd
    SDL_Surface *surf_lourd = TTF_RenderUTF8_Blended((*combat)->font, "Attaque Lourde", blanc);
    if (surf_lourd) {
        (*combat)->texture_texte_lourd = SDL_CreateTextureFromSurface((*combat)->renderer, surf_lourd);
        (*combat)->rect_texte_lourd.w = surf_lourd->w;
        (*combat)->rect_texte_lourd.h = surf_lourd->h;
        SDL_FreeSurface(surf_lourd);
    }

    (*combat)->tour = TOUR_JOUEUR;
}

/* Leo
 * Cette fonction permet de mettre à
 * jour l'XP du personnage après
 * un combat ainsi que supprimer le
 * monstre sur la carte si il a perdu
 */
void combat_termine(SDL_Renderer * rendererPrincipal, combat_t ** combat, case_t ** carte, vainqueur_t vainqueur,int *vies_globales) {
    perso_t * perso = (*combat)->perso;

    /* si le joueur est le vainqueur,
     * on supprime le monstre de la carte,
     * et on lui donne de l'XP
     */
    if (vainqueur == JOUEUR_VAINQUEUR) {
        monstre_t * combat_monstre = (*combat)->monstre;
        int y = combat_monstre->y;
        int x = combat_monstre->x;
        type_monstre_t type = combat_monstre->type;
        perso->nb_victime++;

        if (combat_monstre != NULL) {
            monstre_tue(perso, combat_monstre);
        }

        if (combat_monstre != NULL) {

            if (x >= 0 && y >= 0 && x < TAILLE_CARTE && y < TAILLE_CARTE) {
                monstre_t * monstre = carte[y][x].monstre;

                if (monstre != NULL) {
                    carte[y][x].batiment.type = TOMBE;
                    carte[y][x].monstre = NULL;
                    free(monstre);
                }

            }

        }


        /* 1 chance sur 3 d'avoir un kit de soins */
        if (rand() % 3 == 0) {
            objet_t kit_de_soins = creer_objet(rendererPrincipal, KIT_DE_SOINS, 1);
            ajouter_objet_inventaire(&(perso->inventaire), kit_de_soins);
        }

        /* on gagne un certain nombre de pièces */
        int nb_pieces = 0;

        switch(type) {

            case TROLL:
                /* 1 à 3 pièces */
                nb_pieces = 1 + (rand() % 3);
                break;

            case SQUELETTE:
                /* 1 à 5 pièces */
                nb_pieces = 1 + (rand() % 5);
                break;

            case VER_GEANT:
                /* 2 à 6 pièces */
                nb_pieces = 2 + (rand() % 5);
                break;

            case YETI:
                /* 3 à 7 pièces */
                nb_pieces = 3 + (rand() % 5);
                break;

            default:
                nb_pieces = 0;
                break;

        }

        ajouter_pieces(rendererPrincipal, perso, nb_pieces);
    } else if (vainqueur == MONSTRE_VAINQUEUR || perso->sante <= 0) {
        // Si on arrive ici, c'est que le combat est perdu et qu'il n'y a plus de vies.
        // La résurrection est gérée directement pendant le combat.
        perso->sante = 0;
        perso->mort = 1; 
    }

}

/* Massoud 
 * Cette fonction est faite pour alterner le tour.
*/
tour_t changer_tour(combat_t *combat){
    return (combat->tour == TOUR_JOUEUR) ? TOUR_MONSTRE : TOUR_JOUEUR;
}


/* Massoud
 * Fonctions d'attaque du combat.
 * Chaque attaque génère un pourcentage aléatoire, puis applique
 * ce pourcentage aux dégâts de base de l'attaquant pour calculer
 * les dégâts réels infligés à la cible.
 * La santé de la cible est ensuite diminuée, sans jamais descendre sous 0.
 *
 * Attaque légère :
 *   - Joueur  : entre 20 % et 40 % des dégâts.
 *   - Monstre : entre 5 % et 30 % des dégâts.
 *
 * Attaque lourde :
 *   - Joueur  : entre 30 % et 60 % des dégâts.
 *   - Monstre : entre 20 % et 50 % des dégâts.
 */

void attaque_legere(combat_t *combat){
    /* Lecture du bruitage d'attaque légère */
    if (combat->son_attaque_legere)
        Mix_PlayChannel(-1, combat->son_attaque_legere, 0);
    
    int pourcentage = 20 + rand() % 21; // 20% à 40%
    int vrai_degats = (pourcentage * combat->perso->degats)/100;

    combat->monstre->sante -= vrai_degats;

    if (combat->monstre->sante < 0){
        combat->monstre->sante = 0;
    }
}

void attaque_lourde(combat_t *combat){
    /* Lecture du bruitage d'attaque lourde */
    if (combat->son_attaque_lourde) {
        Mix_PlayChannel(-1, combat->son_attaque_lourde, 0);
    }
    
    /* Nombre aléatoire entre 1 et 3 
    Si le nombre est != 1, l'attaque réussit
    2 chances sur 3 de réussir l'attaque lourde
    */
    if (((rand() % 3) + 1) != 1) {
        int pourcentage = 30 + rand() % 31; // 30% à 60%
        int vrai_degats = (pourcentage * combat->perso->degats)/100;

        combat->monstre->sante -= vrai_degats;

        if (combat->monstre->sante < 0){
            combat->monstre->sante = 0;
        }

    }

}

void attaque_legere_monstre(combat_t *combat){
    int pourcentage = 5 + rand() % 26; // 5% à 30%
    int vrai_degats = (pourcentage * combat->monstre->degats)/100;

    combat->perso->sante -= vrai_degats;

    if(combat->perso->sante < 0){
        combat->perso->sante = 0;
    }
}

void attaque_lourde_monstre(combat_t *combat){
    int pourcentage = 20 + rand() % 31; // 20% à 50%
    int vrai_degats = (pourcentage * combat->monstre->degats)/100;

    combat->perso->sante -= vrai_degats;

    if(combat->perso->sante < 0){
        combat->perso->sante = 0;
    }
}

/* Massoud
 * Cette fonction choisit l'attaque du monstre en fonction de la situation
 * du combat, en gardant une part de hasard pour rendre le comportement
 * moins prévisible.
 *
 * - Si le monstre a peu de vie (≤ 30 % de sa santé maximale), il devient
 *   plus agressif et privilégie l'attaque lourde.
 * - Si le joueur a beaucoup de vie (> 60 % de sa santé maximale), le
 *   monstre attaque fort (préférence pour l'attaque lourde).
 * - Si le joueur a peu de vie (≤ 30 % de sa santé maximale), le monstre
 *   agit plus prudemment (préférence pour l'attaque légère).
 * - Dans tous les autres cas, le monstre choisit de façon équilibrée
 *   entre attaque légère et lourde, avec 50 % de chance pour chacune.
 */
void choix_attaque_monstre(combat_t *combat){
    int hasard = rand() % 100;

    int sante_joueur = combat->perso->sante;
    int sante_max_joueur = combat->perso->sante_max;

    int sante_monstre = combat->monstre->sante;
    int sante_max_monstre = combat->monstre->sante_max;

    /* Si le monstre a peu de vie -> il devient agressif */
    if(sante_monstre <= (sante_max_monstre * 30) / 100){
        if(hasard < 80){
            attaque_lourde_monstre(combat);
        } else {
            attaque_legere_monstre(combat);
        }
    }

    /* Si le joueur a beaucoup de vie -> le monstre attaque fort */
    else if(sante_joueur > (sante_max_joueur * 60) / 100){
        if(hasard < 70){
            attaque_lourde_monstre(combat);
        } else {
            attaque_legere_monstre(combat);
        }
    }

    /* Si le joueur a peu de vie -> le monstre joue plus prudemment */
    else if(sante_joueur <= (sante_max_joueur * 30) / 100){
        if(hasard < 70){
            attaque_legere_monstre(combat);
        } else {
            attaque_lourde_monstre(combat);
        }
    }

    /* Comportement équilibré */
    else {
        if(hasard < 50){
            attaque_legere_monstre(combat);
        } else {
            attaque_lourde_monstre(combat);
        }
    }
}
/*Saandi*/
/**
* Fonction qui crée un combat
*/
combat_t* creer_combat(perso_t *perso, monstre_t *monstre) {
    combat_t *combat = calloc(1,sizeof(combat_t));

    combat->perso = perso;
    combat->monstre = monstre;
    combat->pFenetre = NULL;
    combat->renderer = NULL;
    combat->texture_monstre = NULL;
    combat->texture_perso = NULL;
    combat->texture_fond_ecran = NULL;
    combat->texture_attaque_legere = NULL;
    combat->texture_attaque_lourde = NULL;
    combat->texture_texte_leger = NULL;
    combat->texture_texte_lourd = NULL;
    combat->font = NULL;
    combat->survole_bouton_leger = 0;
    combat->survole_bouton_lourd = 0;
    combat->son_attaque_legere = NULL;
    combat->son_attaque_lourde = NULL;
    
    return combat;
}

/* Leo
 * Cette fonction libère la mémoire
 * d'un combat qui est terminé
 */
void detruire_combat(combat_t ** combat) {

    if (combat != NULL && *combat != NULL) {

        /* Libération du sons pour l'attaque legère */
        if ((*combat)->son_attaque_legere != NULL) {
            Mix_FreeChunk((*combat)->son_attaque_legere);
            (*combat)->son_attaque_legere = NULL;
        }

        /* Libération du sons pour l'attaque lourde */
        if ((*combat)->son_attaque_lourde != NULL) {
            Mix_FreeChunk((*combat)->son_attaque_lourde);
            (*combat)->son_attaque_lourde = NULL;
        }
        
        if ((*combat)->texture_attaque_legere != NULL) {
            SDL_DestroyTexture((*combat)->texture_attaque_legere);
            (*combat)->texture_attaque_legere = NULL;
        }

        if ((*combat)->texture_attaque_lourde != NULL) {
            SDL_DestroyTexture((*combat)->texture_attaque_lourde);
            (*combat)->texture_attaque_lourde = NULL;
        }

        if ((*combat)->texture_monstre != NULL) {
            SDL_DestroyTexture((*combat)->texture_monstre);
            (*combat)->texture_monstre = NULL;
        }

        if ((*combat)->texture_fond_ecran != NULL) {
            SDL_DestroyTexture((*combat)->texture_fond_ecran);
            (*combat)->texture_fond_ecran = NULL;
        }

        if (((*combat)->texture_texte_leger) != NULL) {
            SDL_DestroyTexture((*combat)->texture_texte_leger);
            (*combat)->texture_texte_leger = NULL;
        }

        if (((*combat)->texture_texte_lourd) != NULL) {
            SDL_DestroyTexture((*combat)->texture_texte_lourd);
            (*combat)->texture_texte_lourd = NULL;
        }

        if ((*combat)->font != NULL) {
            TTF_CloseFont((*combat)->font);
            (*combat)->font = NULL;
        }

        free(*combat);
        *combat = NULL;
    }

}
