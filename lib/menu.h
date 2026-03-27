#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

/**
 * @file menu.h
 * @author Massoud
 * @brief Gestion de l'interface des menus et de la sélection des personnages.
 */

/**
 * @author Massoud
 * @brief États de navigation dans l'interface du menu.
 */
typedef enum {
    ETAT_PRINCIPAL,        /**< Écran d'accueil (Jouer, Quitter). */
    ETAT_SELECTION_PERSO   /**< Écran de choix de la classe du personnage. */
} EtatMenu;

/**
 * @author Massoud
 * @brief Types de classes disponibles pour les emplacements (slots) de personnage.
 */
typedef enum {
    SLOT_VIDE = 0,
    SLOT_MAGE,
    SLOT_ASSASSIN,
    SLOT_BRUTE,
    SLOT_CHASSEUR,
    NB_SLOTS
} SlotType;

/**
 * @author Massoud
 * @brief Données d'un emplacement de sélection de personnage.
 * @details Gère le type de classe et le chronomètre pour l'effet visuel de flash au clic.
 */
typedef struct {
    SlotType type;        /**< Type de classe sélectionnée. */
    Uint32   flash_debut; /**< Timer pour l'effet de sélection (0 si inactif). */
} SlotPerso;

/* --- Tableaux de ressources --- */
extern const char *NOM_SLOT[];   /**< Libellés textuels des classes. */
extern const char *CHEMIN_IMG[]; /**< Chemins d'accès aux fichiers images. */

/* --- Prototypes des fonctions de rendu --- */

/**
 * @author Massoud
 * @brief Dessine un bouton interactif avec gestion du survol.
 * @param renderer Le moteur de rendu SDL.
 * @param texture La texture visuelle du bouton.
 * @param rectBtn Rectangle définissant la zone cliquable du bouton.
 * @param rectTxt Rectangle pour le positionnement du texte interne.
 * @param mouse Position actuelle du curseur de la souris.
 */
void dessinerBouton(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect rectBtn, SDL_Rect rectTxt, SDL_Point mouse);

/**
 * @author Massoud
 * @brief Dessine un panneau d'interface stylisé avec une double bordure.
 * @param renderer Le moteur de rendu SDL.
 * @param r Rectangle définissant les dimensions du panneau.
 */
void dessinerPanneau(SDL_Renderer *renderer, SDL_Rect r);

/**
 * @author Massoud
 * @brief Charge une image et applique une clé de couleur pour la transparence.
 * @details Utilisé pour rendre le fond (souvent blanc) d'un sprite transparent.
 * @param renderer Le moteur de rendu SDL.
 * @param chemin Chemin du fichier image à charger.
 * @return SDL_Texture* La texture résultante ou NULL en cas d'erreur.
 */
SDL_Texture *chargerTexture(SDL_Renderer *renderer, const char *chemin);

/**
 * @author Massoud
 * @brief Affiche un emplacement de sélection de personnage.
 * @param renderer Le moteur de rendu SDL.
 * @param policeNom Police pour le nom de la classe.
 * @param policeGrand Police pour les indicateurs visuels.
 * @param rect Zone d'affichage du slot.
 * @param slot Données de l'emplacement à afficher.
 * @param mouse Position du curseur pour l'interaction.
 * @param textures_perso Tableau des textures de personnages chargées.
 */
void dessinerSlot(SDL_Renderer *renderer, TTF_Font *policeNom, TTF_Font *policeGrand, SDL_Rect rect, SlotPerso *slot, SDL_Point mouse, SDL_Texture *textures_perso[NB_SLOTS]);

#endif
