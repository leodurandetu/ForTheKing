#ifndef CARTE_H
#define CARTE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "couleur.h"
#include "monstre.h"
#include "perso.h"
#include "maths.h"
#include "commun.h"
#include "combat.h"
#include "carte_types.h"
#include "option.h"

/* --- Initialisation et Génération --- */

/**
 * @author Léo
 * @brief Alloue et initialise la structure de données de la carte.
 * @param carte Pointeur de pointeur vers la matrice de cases à initialiser en mémoire.
 */
void init_carte(case_t ** carte);

/**
 * @author Massoud
 * @brief Remplit la carte avec les zones d'eau de base.
 * @param carte La matrice de cases représentant la carte du jeu.
 */
void generer_eau(case_t ** carte);

/**
 * @author Saandi
 * @brief Génère les différents biomes (Désert, Forêt, etc.) sur la terre ferme.
 * @param carte La matrice de cases représentant la carte du jeu.
 */
void generer_biomes(case_t ** carte);

/**
 * @author Léo
 * @brief Place les points d'intérêt (Magasins, Boss) sur la carte.
 * @param carte La matrice de cases représentant la carte du jeu.
 */
void placer_batiments(case_t ** carte);

/**
 * @author Massoud
 * @brief Place les bonus de statistiques (Sanctuaires) sur la carte.
 * @param carte La matrice de cases représentant la carte du jeu.
 */
void placer_sanctuaires(case_t ** carte);

/**
 * @author Saandi
 * @brief Remplit une zone rectangulaire de la carte avec un biome spécifique.
 * @param carte La matrice de cases représentant la carte du jeu.
 * @param biome Le type de biome à appliquer à la zone.
 * @param dep Coordonnées du point de départ (coin supérieur gauche).
 * @param arr Coordonnées du point d'arrivée (coin inférieur droit).
 */
void remplir_zone(case_t ** carte, biome_t biome, coordonnee_t dep, coordonnee_t arr);

/**
 * @author Massoud
 * @brief Ajoute des obstacles (arbres, montagnes) selon le biome des cases.
 * @param carte La matrice de cases représentant la carte du jeu.
 */
void ajout_obstacles(case_t ** carte);


/* --- Gestion du Brouillard et Mémoire --- */

/**
 * @author Saandi & Léo
 * @brief Dévoile les cases autour d'une position selon un rayon donné.
 * @param carte La matrice de cases représentant la carte du jeu.
 * @param x Coordonnée X du centre du dévoilement.
 * @param y Coordonnée Y du centre du dévoilement.
 * @param rayon Rayon de visibilité exprimé en nombre de cases.
 */
void devoiler_brouillard_rayon(case_t ** carte, int x, int y, int rayon);

/**
 * @author Saandi & Léo
 * @brief Réinitialise le brouillard sur toute la carte (tout devient invisible).
 * @param carte La matrice de cases représentant la carte du jeu.
 */
void remplir_brouillard(case_t ** carte);

/**
 * @author Léo
 * @brief Libère proprement toute la mémoire allouée pour la carte.
 * @param carte Pointeur vers le tableau 2D (triple pointeur pour la libération totale).
 * @param taille_carte Dimension (largeur/hauteur) de la carte carrée.
 */
void liberer_memoire_carte(case_t *** carte, int taille_carte);


/* --- Logique des Entités (Monstres) --- */

/**
 * @author Léo & Massoud
 * @brief Répartition initiale des monstres sur la carte au début du jeu.
 * @param carte La matrice de cases représentant la carte du jeu.
 */
void placer_monstres(case_t ** carte);

/**
 * @author Massoud
 * @brief Fait apparaître un groupe spécifique de monstres sur des cases données.
 * @param carte La matrice de cases représentant la carte du jeu.
 * @param type Le type de monstre à instancier.
 * @param cases_dispos Tableau contenant les coordonnées des emplacements libres.
 * @param nb_cases_dispos Nombre total d'emplacements disponibles dans le tableau.
 */
void faire_apparaitre_groupe(case_t ** carte, type_monstre_t type, coordonnee_t cases_dispos[], int nb_cases_dispos);

/**
 * @author Léo
 * @brief Gère l'IA de déplacement des monstres et déclenche les combats si nécessaire.
 * @param rendererPrincipal Le moteur de rendu SDL pour l'affichage de la scène.
 * @param carte La matrice de cases représentant la carte du jeu.
 * @param perso Pointeur vers la structure du personnage joueur.
 * @param combat_actuel Pointeur vers le contexte de combat si un affrontement est déclenché.
 * @param vies_globales Pointeur vers l'entier gérant le stock de vies du joueur.
 */
void deplacer_monstres(SDL_Renderer * rendererPrincipal, case_t ** carte, perso_t * perso, combat_t ** combat_actuel, int *vies_globales);

/**
 * @author Massoud
 * @brief Vérifie si une case adjacente contient un monstre.
 * @param carte La matrice de cases représentant la carte du jeu.
 * @param cx Coordonnée X de la case à tester.
 * @param cy Coordonnée Y de la case à tester.
 * @return booleen_t Retourne VRAI si au moins un voisin est un monstre, sinon FAUX.
 */
booleen_t a_un_voisin_monstre(case_t ** carte, int cx, int cy);


/* --- Tests et Recherche de Chemin --- */

/**
 * @author Léo
 * @brief Vérifie si le personnage peut marcher sur cette case (selon le biome et les obstacles).
 * @param carte La matrice de cases représentant la carte du jeu.
 * @param x Coordonnée X de la case cible.
 * @param y Coordonnée Y de la case cible.
 * @return booleen_t VRAI si le passage est possible, FAUX si la case est bloquée.
 */
booleen_t deplacement_possible(case_t ** carte, int x, int y);

/**
 * @author Léo
 * @brief Vérifie si la case est déjà occupée par une entité (joueur ou monstre).
 * @param carte La matrice de cases représentant la carte du jeu.
 * @param x Coordonnée X de la case à tester.
 * @param y Coordonnée Y de la case à tester.
 * @param persX Coordonnée X actuelle du personnage (pour éviter de se bloquer soi-même).
 * @param persY Coordonnée Y actuelle du personnage.
 * @return booleen_t VRAI si la case contient déjà une entité, FAUX sinon.
 */
booleen_t case_occupee(case_t ** carte, int x, int y, int persX, int persY);

/**
 * @author Léo
 * @brief Trouve les coordonnées d'une case aléatoire libre sur la carte.
 * @param carte La matrice de cases représentant la carte du jeu.
 * @param x Pointeur vers l'entier qui recevra la coordonnée X trouvée.
 * @param y Pointeur vers l'entier qui recevra la coordonnée Y trouvée.
 */
void coords_case_libre(case_t ** carte, int *x, int *y);

/**
 * @author Léo & Massoud
 * @brief Convertit les coordonnées pixels de la souris en coordonnées de la grille de jeu.
 * @param mouseX Position X du curseur de la souris.
 * @param mouseY Position Y du curseur de la souris.
 * @param carte_x Pointeur vers l'entier qui recevra l'indice X de la case.
 * @param carte_y Pointeur vers l'entier qui recevra l'indice Y de la case.
 * @param tailleCase Dimension d'une case à l'affichage.
 * @param perso Pointeur vers le personnage (pour le calcul du décalage de la caméra).
 * @param renderer Le moteur de rendu SDL.
 */
void souris_vers_case(int mouseX, int mouseY, int *carte_x, int *carte_y, int tailleCase, perso_t *perso, SDL_Renderer *renderer);

/**
 * @author Léo
 * @brief Algorithme de recherche de chemin pour vérifier si une cible est atteignable.
 * @param carte La matrice de cases représentant la carte du jeu.
 * @param taille_carte Dimension de la carte.
 * @param xDepart Coordonnée X de départ.
 * @param yDepart Coordonnée Y de départ.
 * @param xCible Coordonnée X de la destination.
 * @param yCible Coordonnée Y de la destination.
 * @param pts_deplacement_max Limite de mouvement autorisée.
 * @param perso Pointeur vers le personnage.
 * @param distance Pointeur vers l'entier qui recevra la longueur du chemin calculé.
 * @return int Retourne 1 si un chemin valide existe, 0 sinon.
 */
int chemin_valide(case_t **carte, int taille_carte, int xDepart, int yDepart, int xCible, int yCible, int pts_deplacement_max, perso_t *perso, int *distance);

/**
 * @author Léo
 * @brief Deplace le personnage d'un certain décalage si possible, ne fait rien sinon.
 * @param perso Pointeur vers le personnage à déplacer.
 * @param carte La matrice de cases représentant la carte du jeu.
 * @param dx Le décalage sur l'axe des abscisses
 * @param dy Le décalage sur l'axe des ordonnées
 * @param majAffichage Pointeur sur l'entier dans le main qui dit si on doit mettre à jour l'affichage
 * @param majBrouillard Pointeur sur l'entier dans le main qui dit si on doit mettre à jour le brouillard
 */
void deplacer_perso_si_possible(perso_t * perso, case_t ** carte, int dx, int dy, int * majAffichage, int * majBrouillard);

#endif
