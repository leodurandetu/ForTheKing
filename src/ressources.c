#include "../lib/ressources.h"
#include <stdio.h>
#include <string.h>     
#include "../lib/init_sdl.h"

int charger_ressources(SDL_Renderer* renderer, ressources_jeu_t* res, perso_type_t type_perso) {
    
    /* Initialisation de la structure à zéro par sécurité */
    memset(res, 0, sizeof(ressources_jeu_t));
    
    /* Chargement des polices d'écriture */
    res->police_max = init_ttf_max();
    res->police_min = init_ttf_min();

    /* Chargement des textures correspondant aux différents biomes */
    char *nom_images[NB_BIOMES] = {
        "img/terrev1.png", "img/eauv2.png",
        "img/desertv1.png", "img/neigev3.png",
        "img/foretv1.png"
    };
    for (int i = 0; i < NB_BIOMES; i++) {
        SDL_Surface *image = IMG_Load(nom_images[i]);
        if (!image) return -1; 
        res->cases[i] = SDL_CreateTextureFromSurface(renderer, image);
        SDL_FreeSurface(image);
    }

    /* Chargement de la texture des personnages avec détourage du fond blanc */
    const char *chemins_perso[] = {
                    "img/brute.png",    // BRUTE = 0
                    "img/chasseur.png", // CHASSEUR = 1
                    "img/assassin.png", // ASSASSIN = 2
                    "img/mage.png"      // MAGE = 3
    };

    SDL_Surface *img_perso = IMG_Load(chemins_perso[type_perso]);
    if (img_perso) {
        SDL_SetColorKey(img_perso, SDL_TRUE, SDL_MapRGB(img_perso->format, 255, 255, 255));
        res->perso = SDL_CreateTextureFromSurface(renderer, img_perso);
        SDL_SetTextureBlendMode(res->perso, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_perso);
    }

    /* Chargement des entités ennemies (Index 0 : Squelette, Index 1 : Troll) */
    SDL_Surface *img_squelette = IMG_Load("img/squelette.png");
    if (img_squelette) {
        res->monstres[0] = SDL_CreateTextureFromSurface(renderer, img_squelette);
        SDL_SetTextureBlendMode(res->monstres[0], SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_squelette);
    }

    SDL_Surface *img_troll = IMG_Load("img/troll.png");
    if (img_troll) {
        res->monstres[1] = SDL_CreateTextureFromSurface(renderer, img_troll);
        SDL_SetTextureBlendMode(res->monstres[1], SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_troll);
    }

    SDL_Surface *img_yeti = IMG_Load("img/yeti.png");
    if (img_yeti) {
        res->monstres[2] = SDL_CreateTextureFromSurface(renderer, img_yeti);
        SDL_SetTextureBlendMode(res->monstres[1], SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_yeti);
    }

    /* Chargement de la texture gérant le brouillard */
    SDL_Surface *img_brouillard = IMG_Load("img/brouillard.png");
    if (img_brouillard) {
        res->brouillard = SDL_CreateTextureFromSurface(renderer, img_brouillard);
        SDL_SetTextureBlendMode(res->brouillard, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_brouillard);
    }

    /* Chargement des textures liées aux bâtiments interactifs */
    char *nom_images_batiments[3] = {
        "img/campement.png", "img/tour_boss.png", "img/tombe.png"
    };
    for (int i = 0; i < 3; i++) {
        SDL_Surface *img_batiment = IMG_Load(nom_images_batiments[i]);
        if (img_batiment) {
            res->batiments[i] = SDL_CreateTextureFromSurface(renderer, img_batiment);
            SDL_SetTextureBlendMode(res->batiments[i], SDL_BLENDMODE_BLEND);
            SDL_FreeSurface(img_batiment);
        }
    }

    /* Chargement des éléments de décor agissant comme obstacles */
    char *nom_images_obstacles[4] = {
        "img/arbres.png", "img/montagnes.png",
        "img/cactus.png", "img/boue.png" 
    };
    for (int i = 0; i < 4; i++) {
        SDL_Surface *image_obs = IMG_Load(nom_images_obstacles[i]);
        if (image_obs) {
            res->obstacles[i] = SDL_CreateTextureFromSurface(renderer, image_obs);
            SDL_SetTextureBlendMode(res->obstacles[i], SDL_BLENDMODE_BLEND);
            SDL_FreeSurface(image_obs);
        } else {
            fprintf(stderr, "Attention : Image obstacle %s manquante !\n", nom_images_obstacles[i]);
        }
    }

    /* Chargement des textures des différents sanctuaires d'amélioration */
    char *nom_images_sanctuaires[4] = {
        "img/sanc_puissance.png", 
        "img/sanc_rapidité.png",  
        "img/sanc_intelligence.png", 
        "img/sanc_experience.png" 
    };
    for (int i = 0; i < 4; i++) {
        SDL_Surface *image_sanc = IMG_Load(nom_images_sanctuaires[i]);
        if (image_sanc) {
            res->sanctuaires[i] = SDL_CreateTextureFromSurface(renderer, image_sanc);
            SDL_SetTextureBlendMode(res->sanctuaires[i], SDL_BLENDMODE_BLEND);
            SDL_FreeSurface(image_sanc);
        } else {
            fprintf(stderr, "Attention : Image sanctuaire %s manquante !\n", nom_images_sanctuaires[i]);
        }
    }

    /* Chargement de l'icône représentant les points de vie */
    SDL_Surface *img_vie = IMG_Load("img/coeur.png");
    if (img_vie) {
        res->vie = SDL_CreateTextureFromSurface(renderer, img_vie);
        SDL_SetTextureBlendMode(res->vie, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_vie);
    }
    
    
    return 0; // succès
}

void liberer_ressources(ressources_jeu_t* res) {
    /* Libération de la mémoire pour les textures individuelles */
    if (res->perso) SDL_DestroyTexture(res->perso);
    if (res->brouillard) SDL_DestroyTexture(res->brouillard);
    if (res->vie) SDL_DestroyTexture(res->vie);
    
    /* Libération de la mémoire pour les tableaux de textures */
    for (int i = 0; i < NB_BIOMES; i++) {
        if (res->cases[i]) SDL_DestroyTexture(res->cases[i]);
    }
    for (int i = 0; i < 2; i++) {
        if (res->monstres[i]) SDL_DestroyTexture(res->monstres[i]);
    }
    for (int i = 0; i < 3; i++) {
        if (res->batiments[i]) SDL_DestroyTexture(res->batiments[i]);
    }
    for (int i = 0; i < 4; i++) {
        if (res->sanctuaires[i]) SDL_DestroyTexture(res->sanctuaires[i]);
        if (res->obstacles[i]) SDL_DestroyTexture(res->obstacles[i]);
    }

    /* Fermeture et libération des polices d'écriture TTF */
    if (res->police_max) TTF_CloseFont(res->police_max);
    if (res->police_min) TTF_CloseFont(res->police_min);
}
