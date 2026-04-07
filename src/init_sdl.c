#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/init_sdl.h"

int init_sdl_global(){

    /* Initialisation globale de la SDL Vidéo et Audio */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Echec de l'initialisation de la SDL : %s\n", SDL_GetError());
        return -1;
    }

    /* Initialisation du TTF pour le rendu du texte */
    if(TTF_Init() == -1) 
    {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return -1;
    }
    
    /* Retourne 0 si toutes les initialisations ont réussi */
    return 0; 
}

Mix_Music* init_sdl_musique() {
    
    /* Configuration et ouverture du périphérique audio */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur d'ouverture audio (SDL_mixer) : %s\n", Mix_GetError());
        return NULL; 
    }

    /* Chargement de la piste musicale principale en mémoire */
    Mix_Music *musique = Mix_LoadMUS("audio/Main_theme.mp3");

    /* On signale si le fichier n'a pas pu être chargé pour éviter un crash ultérieur */
    if (!musique) {
        fprintf(stderr, "Attention : Musique principale introuvable (%s)\n", Mix_GetError());
        return NULL; 
    }

    return musique;
}

TTF_Font* init_ttf_medium(){
    TTF_Font* police;

    /* Chargement de la police d'écriture en taille moyenne (24) pour les textes standards */
    if ((police = TTF_OpenFont("Fonts/Enchanted Land.otf", 24)) == NULL) {
        fprintf(stderr, "Erreur : Impossible de charger la police 'Enchanted Land' (taille 24) : %s\n", TTF_GetError());
        return NULL;
    }
    
    return police;
}

TTF_Font* init_ttf_min() {
    TTF_Font* police;

    /* Chargement de la police d'écriture en taille moyenne (24) pour les textes standards */
    if ((police = TTF_OpenFont("Fonts/Enchanted Land.otf", 20)) == NULL) {
        fprintf(stderr, "Erreur : Impossible de charger la police 'Enchanted Land' (taille 20) : %s\n", TTF_GetError());
        return NULL;
    }
    
    return police;
}

TTF_Font* init_ttf_max(){
    
    TTF_Font* police;

    /* Chargement de la police d'écriture en grande taille (32) pour les titres ou éléments majeurs */
    if ((police = TTF_OpenFont("Fonts/Enchanted Land.otf", 32)) == NULL) {
        fprintf(stderr, "Erreur : Impossible de charger la police 'Enchanted Land' (taille 32) : %s\n", TTF_GetError());
        return NULL;
    }
        
    return police;
}