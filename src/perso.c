#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../lib/perso.h"
#include "../lib/carte.h"

/* Leo */
/*
 * Cette fonction crée un personnage avec des coordonnées de départ,
 * et avec un type donné. Les attributs sont sélectionnés en fonction
 * du type du personnage.
 * Elle renvoie un pointeur sur le personnage créé.
 */
perso_t * init_perso(perso_type_t persoType, int xDepart, int yDepart)
{
    perso_t * perso = malloc(sizeof(perso_t));

    perso->x = xDepart;
    perso->y = yDepart;

    perso->type = persoType;

    switch (persoType)
    {

        case BRUTE:
            perso->force = 32;
            perso->sante = 50;
            perso->intelligence = 4;
            perso->rapidite = 8;
            perso->evasion = 3;
            break;

        case CHASSEUR:
            perso->force = 20;
            perso->sante = 40;
            perso->intelligence = 13;
            perso->rapidite = 19;
            perso->evasion = 15;
            break;

        case ASSASSIN:
            perso->force = 16;
            perso->sante = 32;
            perso->intelligence = 13;
            perso->rapidite = 24;
            perso->evasion = 24;
            break;

        case MAGE:
            perso->force = 12;
            perso->sante = 36;
            perso->intelligence = 26;
            perso->rapidite = 17;
            perso->evasion = 8;
            break;

    }

    return perso;
}

/* Leo */
/*
 * Cette fonction détruit et libère la place mémoire d'un personnage
 */
void detruire_perso(perso_t ** perso) {

    if (perso != NULL && *perso != NULL) {
        free(*perso);
        *perso = NULL;
    }

}

/* Massoud */
/*
 * Cette fonction permet d'afficher le personnage
 * sur la carte avec SDL2 
 */
void afficher_personnage(SDL_Renderer * renderer, SDL_Texture * texture_perso, perso_t * perso, int tailleCase, int estSelectionne) {
    
    int fenetre_taille_x, fenetre_taille_y;
    SDL_GetRendererOutputSize(renderer, & fenetre_taille_x, & fenetre_taille_y);

    float rayon = tailleCase / 2.0f;
    float hex_w = 2.0f * rayon;
    float hex_h = sqrtf(3) * rayon;
    float espacement_colonnes = hex_w * 0.75f;

    float cx = perso -> x * espacement_colonnes + hex_w / 2.0f;
    float cy = perso -> y * hex_h + (perso -> x % 2 ? hex_h / 2.0f : 0) + hex_h / 2.0f;

    /* Caméra centrée sur le joueur */
    float camX = cx - hex_w / 2.0f;
    float camY = cy - hex_h / 2.0f;
    int decalageX = (int)(fenetre_taille_x / 2 - camX - hex_w / 2.0f);
    int decalageY = (int)(fenetre_taille_y / 2 - camY - hex_h / 2.0f);

    float cx_finale = cx + decalageX;
    float cy_finale = cy + decalageY;

    /* Ajout du halo du selection */
    if (estSelectionne) {
        /* Couleur Cyan très claire, avec un alpha de 128 (moitié transparent) */
        SDL_Color couleurHalo = { 100, 255, 255, 128 };
        afficher_hex_couleur(renderer, cx_finale, cy_finale, rayon + 3.0f, couleurHalo);
    }

    /* Taill du personnage */
    float echelle = 0.65f; // On ajustera encore !

    SDL_Rect dstRect;
    dstRect.w = (int) (hex_w * echelle);
    dstRect.h = (int) (hex_w * echelle); // Carré
    /* Centrage automatique */
    dstRect.x = (int)(cx_finale - dstRect.w / 2);
    dstRect.y = (int)(cy_finale - dstRect.h / 2);

    SDL_RenderCopy(renderer, texture_perso, NULL, & dstRect);
}
