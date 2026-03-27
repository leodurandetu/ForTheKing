#ifndef AFFICHAGE_COMMUN_H
#define AFFICHAGE_COMMUN_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "perso.h"

/**
 * @author Saandi & Léo
 * @brief Définit le sens d'affichage des éléments de l'inventaire.
 */
typedef enum {
    INV_HORIZONTAL,
    INV_VERTICAL
} orientation_inv_t;

/* --- Prototypes des fonctions --- */

/**
 * @author Saandi & Léo
 * @brief Crée une texture SDL à partir d'une chaîne de caractères.
 * @param renderer Le moteur de rendu SDL.
 * @param police La police TTF à utiliser pour générer le texte.
 * @param texte La chaîne de caractères à transformer en texture.
 * @param couleur La couleur du texte.
 * @param rectangle_sortie Pointeur vers un SDL_Rect qui récupérera les dimensions automatiques du texte généré.
 * @return SDL_Texture* La texture créée, ou NULL en cas d'erreur d'allocation ou de rendu.
 */
SDL_Texture* creer_texte(SDL_Renderer* renderer, TTF_Font* police, const char* texte, SDL_Color couleur, SDL_Rect* rectangle_sortie);

/**
 * @author Saandi & Léo
 * @brief Dessine une barre de progression (ex: PV, Expérience) avec un label textuel.
 * @param r Le moteur de rendu SDL.
 * @param f La police pour afficher le label.
 * @param label Le nom de la statistique (ex: "PV").
 * @param val La valeur actuelle de la statistique.
 * @param max La valeur maximale de la statistique pour calculer le ratio de remplissage.
 * @param zone Le rectangle (position et taille) définissant l'emplacement de la barre à l'écran.
 * @param coul_barre La couleur de remplissage de la barre.
 */
void dessiner_barre(SDL_Renderer* r, TTF_Font* f, const char* label, int val, int max, SDL_Rect zone, SDL_Color coul_barre);

/**
 * @author Saandi & Léo
 * @brief Affiche un message d'alerte ou d'information centré sur l'écran.
 * @param renderer Le moteur de rendu SDL.
 * @param font La police de caractères.
 * @param message Le texte à afficher.
 * @param fenetre_w Largeur de la fenêtre pour le calcul du centrage.
 * @param fenetre_h Hauteur de la fenêtre pour le calcul du centrage.
 * @param texture Pointeur vers la texture SDL (permet de réutiliser ou mettre à jour la texture existante).
 * @param ancienMessage Chaîne de caractères permettant de vérifier si le message a changé pour éviter une régénération inutile de texture.
 */
void afficherMessageCentre(SDL_Renderer *renderer, TTF_Font *font, const char *message, int fenetre_w, int fenetre_h, SDL_Texture **texture, char *ancienMessage);

/**
 * @author Saandi & Léo
 * @brief Gère l'affichage complet et l'interaction de l'inventaire du personnage.
 * @param renderer Le moteur de rendu SDL.
 * @param font La police pour le nom des objets.
 * @param perso Pointeur vers le personnage possédant l'inventaire.
 * @param fond_interface Rectangle définissant la zone de fond de l'inventaire.
 * @param rect_nom Rectangle de destination pour le titre de l'inventaire.
 * @param clic_gauche État du bouton gauche de la souris pour les interactions.
 * @param maj_affichage Pointeur permettant de notifier une modification graphique (ex: après consommation d'un objet).
 * @param orientation Choix entre un affichage en ligne (horizontal) ou en colonne (vertical).
 * @param start_droite_x Coordonnée de départ X si l'affichage doit être décalé sur la droite.
 */
void dessiner_inventaire(SDL_Renderer * renderer, TTF_Font * font, perso_t * perso, SDL_Rect fond_interface, SDL_Rect rect_nom, int clic_gauche, int * maj_affichage, orientation_inv_t orientation, int start_droite_x);

#endif
