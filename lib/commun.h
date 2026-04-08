#ifndef COMMUN_H
#define COMMUN_H

/**
 * @file commun.h
 * @author Léo, Saandi & Massoud
 * @brief Définitions de base partagées par l'ensemble des modules du projet.
 */

/**
 * @author Léo, Saandi & Massoud
 * @brief Type énuméré simulant un type booléen.
 */
typedef enum {
    FAUX, /**< Représente la valeur logique fausse (0). */
    VRAI  /**< Représente la valeur logique vraie (1). */
} booleen_t;

/**
 * @author Léo
 * @brief Type énuméré représentant l'état du jeu.
 */
typedef enum {
    CARTE, /**< Lorsqu'on est sur la carte. */
    GAME_OVER, /**< Lorsqu'on a fini l'aventure (perso mort). */
    COMBAT /**< Lorsqu'on est en combat contre un monstre. */
} etat_jeu_t;

#define MAX_FUITE 3 /* le nombre de fuites au maximum avant de ne plus avoir le choix d'attaquer */

#endif
