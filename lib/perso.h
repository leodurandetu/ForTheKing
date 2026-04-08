/**
 * @file quetes.h
 * @author Massoud
 * @brief Définitions et structures pour le système de quêtes annexes.
 */
 

#ifndef PERSO_H
#define PERSO_H

#include <SDL2/SDL.h>
#include "inventaire.h"

/**
 * @file perso.h
 * @author Léo
 * @brief Définition du personnage joueur et de ses caractéristiques.
 */

/** * @author Léo
 * @brief Classes de personnages disponibles dans le jeu.
 * @details Chaque type possède des statistiques de base équilibrées différemment.
 */
typedef enum {
    BRUTE,
    CHASSEUR,
    ASSASSIN,
    MAGE
} perso_type_t;

/** * @author Léo
 * @brief Structure principale représentant le joueur.
 * @details Regroupe les statistiques de combat, la position, la progression et l'inventaire.
 * Le tag 'perso_s' permet les déclarations anticipées pour éviter les inclusions cycliques.
 */
typedef struct perso_s {
    /* Position sur la carte */
    int x; /**< Position horizontale sur la grille. */
    int y; /**< Position verticale sur la grille. */

    /* Statistiques de combat */
    int degats;       /**< Dégâts de base. */
    int force;        /**< Multiplicateur de puissance physique. */
    int sante;        /**< Points de vie actuels. */
    int sante_max;    /**< Capacité maximale de points de vie. */
    int intelligence; /**< Capacité magique ou bonus tactiques. */
    int rapidite;     /**< Détermine l'initiative ou l'esquive. */
    int evasion;      /**< Pourcentage de chance d'éviter un coup. */

    /* Gestion des tours et progression */
    int pts_deplacements; /**< Capital de points restants pour bouger durant le tour actuel. */
    int niveau;           /**< Niveau actuel du personnage. */
    int exp;              /**< Expérience accumulée. */
    int mort;             /**< État vital (0 pour vivant, 1 pour mort). */
    int pieces;           /**< Argent, permettant d'acheter des objets (soins, parchemins de téléportation). */
    int nb_victime;       /** Nombre de monstre tués. */
    /* Composants */
    inventaire_t inventaire; /**< Sac à dos contenant les objets. */
    perso_type_t type;       /**< Classe choisie au début de la partie. */

} perso_t;

/* --- Prototypes des fonctions --- */

/**
 * @author Léo
 * @brief Affiche le sprite du personnage sur la carte SDL.
 * @param renderer Le moteur de rendu SDL.
 * @param texture_perso La texture correspondant à la classe du personnage.
 * @param perso Pointeur vers l'instance du personnage à afficher.
 * @param tailleCase Dimension d'une cellule pour le calcul de la position à l'écran.
 */
void afficher_personnage(SDL_Renderer * renderer, SDL_Texture * texture_perso, perso_t * perso, int tailleCase);

/**
 * @author Léo
 * @brief Alloue et initialise un nouveau personnage selon sa classe.
 * @param persoType La classe sélectionnée par le joueur.
 * @param xDepart Coordonnée X de départ.
 * @param yDepart Coordonnée Y de départ.
 * @return perso_t* Un pointeur vers l'instance créée, ou NULL en cas d'échec.
 */
perso_t * init_perso(perso_type_t persoType, int xDepart, int yDepart);

/**
 * @author Léo
 * @brief Libère proprement la mémoire allouée pour le personnage et ses composants.
 * @param perso Pointeur de pointeur vers le personnage à désallouer.
 */
void detruire_perso(perso_t ** perso);

/**
 * @author Massoud
 * @brief Récupère le nombre maximal de points de mouvement autorisés pour le personnage.
 * @details Cette valeur sert de plafond lors de la restauration des points en début de tour.
 * @param perso Pointeur vers le personnage concerné.
 * @return int Le total des points de mouvement permis par la classe et le niveau.
 */
int get_pers_movements_points(perso_t *perso);

/**
 * @author Léo
 * @brief Réinitialise les points de déplacements du personnage.
 * @details Généralement appelée au début de chaque nouveau tour de jeu.
 * @param perso Pointeur vers le personnage à restaurer.
 */
void restaurer_points_deplacements(perso_t * perso);

#endif
