#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


/* Etats du menu */
typedef enum {
    ETAT_PRINCIPAL,
    ETAT_SELECTION_PERSO
} EtatMenu;

/* Types de slots de personnage (cycle au clic) */
typedef enum {
    SLOT_VIDE = 0,
    SLOT_MAGE,
    SLOT_ASSASSIN,
    SLOT_BRUTE,
    SLOT_CHASSEUR,
    NB_SLOTS
} SlotType;

/* Donnees d'un slot de selection */
typedef struct {
    SlotType type;
    Uint32   flash_debut; /* 0 = inactif */
} SlotPerso;

/* Tableaux noms et chemins des personnages */
extern const char *NOM_SLOT[];
extern const char *CHEMIN_IMG[];

/* Dessine un bouton  */
void dessinerBouton(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect rectBtn, SDL_Rect rectTxt, SDL_Point mouse);

/* Dessine un panneau sombre avec double bordure */
void dessinerPanneau(SDL_Renderer *renderer, SDL_Rect r);

/* Charge une texture depuis un fichier puis rend le blanc transparent */
SDL_Texture *chargerTexture(SDL_Renderer *renderer, const char *chemin);

/* Dessine un slot de selection de personnage */
void dessinerSlot(SDL_Renderer *renderer, TTF_Font *policeNom, TTF_Font *policeGrand, SDL_Rect rect, SlotPerso *slot, SDL_Point mouse, SDL_Texture *textures_perso[NB_SLOTS]);

#endif
