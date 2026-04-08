#ifndef AFFICHAGE_CARTE_H
#define AFFICHAGE_CARTE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "monstre.h"
#include "perso.h"
#include "carte.h"
#include "maths.h"
#include "affichage_commun.h"
#include "niveau.h"

/* --- Prototypes des fonctions de rendu géométrique --- */

/**
 * @author Massoud
 * @brief Affiche un hexagone avec une texture et une teinte de couleur appliquée.
 * @param renderer Le moteur de rendu SDL.
 * @param cx Coordonnée X du centre de l'hexagone.
 * @param cy Coordonnée Y du centre de l'hexagone.
 * @param rayon Distance entre le centre et un sommet.
 * @param texture Pointeur vers la texture SDL à plaquer.
 * @param couleur Couleur de modulation (teinte) à appliquer.
 */
void afficher_hex_texture(SDL_Renderer* renderer, float cx, float cy, float rayon, SDL_Texture* texture, SDL_Color couleur);

/**
 * @author Massoud
 * @brief Dessine un contour doré décoratif (style double trait) autour d'une case.
 * @param r Le moteur de rendu SDL.
 * @param cx Coordonnée X du centre.
 * @param cy Coordonnée Y du centre.
 * @param rayon Taille de l'hexagone.
 * @param alpha Niveau d'opacité du doré (0-255).
 */
void dessiner_contour_double_dore(SDL_Renderer* r, float cx, float cy, float rayon, int alpha);

/**
 * @author Massoud
 * @brief Dessine un contour épais personnalisé (style 'For The King').
 * @param r Le moteur de rendu SDL.
 * @param cx Coordonnée X du centre.
 * @param cy Coordonnée Y du centre.
 * @param rayon Taille de l'hexagone.
 * @param epaisseur Largeur du trait de contour.
 * @param couleur Couleur du contour.
 */
void dessiner_contour_ftk(SDL_Renderer* r, float cx, float cy, float rayon, int epaisseur, SDL_Color couleur);

/**
 * @author Massoud
 * @brief Affiche un hexagone plein avec une couleur simple (sans texture).
 * @param renderer Le moteur de rendu SDL.
 * @param cx Coordonnée X du centre.
 * @param cy Coordonnée Y du centre.
 * @param rayon Taille de l'hexagone.
 * @param couleur Couleur de remplissage.
 */
void afficher_hex_couleur(SDL_Renderer* renderer, float cx, float cy, float rayon, SDL_Color couleur);


/* --- Prototypes des fonctions de gestion de scène --- */

/**
 * @author Massoud & Léo
 * @brief Fonction principale gérant tout l'affichage du monde et de ses entités.
 * @details Parcourt la matrice de la carte pour afficher biomes, obstacles, brouillard,
 * monstres, bâtiments et sanctuaires en fonction de la position du joueur.
 * @param renderer Le moteur de rendu SDL.
 * @param carte Matrice de structures case_t représentant le monde.
 * @param textures_cases Tableau des textures de sol par biome.
 * @param textures_obstacles Tableau des textures d'éléments de décor.
 * @param texture_brouillard Texture recouvrant les zones non explorées.
 * @param textures_monstres Tableau des sprites des ennemis.
 * @param textures_batiments Tableau des textures des structures interactives.
 * @param textures_sanctuaires Tableau des textures des bonus de map.
 * @param tailleCase Taille d'affichage d'une case.
 * @param persX Coordonnée X actuelle du personnage.
 * @param persY Coordonnée Y actuelle du personnage.
 * @param case_selection_x Coordonnée X de la case actuellement survolée/sélectionnée.
 * @param case_selection_y Coordonnée Y de la case actuellement survolée/sélectionnée.
 * @param perso Pointeur vers la structure du personnage pour ses infos d'affichage.
 */
void afficher_carte_sdl(SDL_Renderer * renderer,
    case_t ** carte,
    SDL_Texture * textures_cases[NB_BIOMES],
    SDL_Texture * textures_obstacles[4], 
    SDL_Texture * texture_brouillard,
    SDL_Texture * textures_monstres[2],
    SDL_Texture * textures_batiments[3],
    SDL_Texture * textures_sanctuaires[4],
    int tailleCase,
    int persX, int persY,
    int case_selection_x, int case_selection_y,
    perso_t *perso);

/**
 * @author Léo
 * @brief Nettoie le renderer et prépare les états de rendu pour la nouvelle frame.
 */
void preparer_avant_affichage();

/**
 * @author Léo
 * @brief Dessine les éléments de l'interface utilisateur (HUD) par-dessus la carte.
 * @param renderer Le moteur de rendu SDL.
 * @param ffont Police de caractères pour les textes.
 * @param portrait Texture de l'avatar du personnage.
 * @param perso Pointeur vers les données du personnage (PV, XP, etc.).
 * @param clic_gauche État du clic souris.
 * @param maj_affichage Pointeur d'état permettant de forcer le rafraîchissement graphique.
 */
void dessiner_interface_carte(SDL_Renderer *renderer, TTF_Font* ffont, SDL_Texture* portrait, perso_t * perso, int clic_gauche, int * maj_affichage);

/**
 * @author Massoud
 * @brief Affiche un message et un filtre sombre lors de la fin de l'aventure (perso mort).
 * @param renderer Le moteur de rendu SDL
 * @param police La police de caractères pour les textes.
 */
void afficher_fin_aventure(SDL_Renderer *renderer, TTF_Font* police);

#endif
