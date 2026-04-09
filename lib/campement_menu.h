#ifndef CAMPEMENT_MENU_H
#define CAMPEMENT_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../lib/perso.h"
#include "../lib/carte.h"

/**
 * @enum resultat_campement_t
 * @brief Liste les actions possibles en sortie du menu campement.
 */
typedef enum {
    CAMPEMENT_REPOS,   /**< Le joueur a choisi de se reposer pour regagner des PV. */
    CAMPEMENT_SOINS,   /**< Le joueur a choisi d'acheter des objets de soins. */
    CAMPEMENT_QUITTE   /**< Le joueur a cliqué à côté sans action. */
} resultat_campement_t;


/**
 * @author Massoud
 * @brief Gère l'affichage et l'interaction avec le menu du campement.
 * @details Affiche un panneau interactif à côté de la case du campement. La fonction est 
 * bloquante tant que l'utilisateur n'a pas sélectionné une option ou quitté le menu(cliqué en dehors du menu = quitter). 
 * @param renderer Le moteur de rendu SDL utilisé pour dessiner le menu.
 * @param font La police de caractères utilisée pour les textes et les boutons.
 * @param perso Pointeur vers le personnage (pour calculs de position ou de stats).
 * @param camp_x Coordonnée X de la case du campement sur la carte.
 * @param camp_y Coordonnée Y de la case du campement sur la carte.
 * @param tailleCase Dimension actuelle des cases pour le calcul de positionnement du menu.
 * @return resultat_campement_t L'action choisie par le joueur parmi les membres de l'énumération.
 */
resultat_campement_t afficher_menu_campement(
    SDL_Renderer *renderer,
    TTF_Font     *font,
    perso_t      *perso,
    int           camp_x,
    int           camp_y,
    int           tailleCase
);

#endif /* CAMPEMENT_MENU_H */