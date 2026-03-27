#ifndef PERLIN_H
#define PERLIN_H

/**
 * @file perlin.h
 * @author Massoud
 * @brief Générateur de bruit de Perlin pour la création procédurale de la carte.
 * @note Basé sur l'algorithme original de Ken Perlin (1983). 
 * Cet algorithme est utilisé pour générer des reliefs naturels 
 * de manière cohérente et structurée.
 */

/* --- Prototypes des fonctions --- */

/**
 * @author Massoud
 * @brief Initialise le générateur de bruit de Perlin.
 * @details Prépare les tables de permutation et les gradients nécessaires 
 * au calcul. Doit être appelée une seule fois (souvent au lancement du programme) 
 * avant toute utilisation de la fonction de calcul 2D.
 * @param graine Valeur entière (graine aléatoire) permettant de générer une suite 
 * déterministe. Une même graine produira toujours le même motif de carte.
 */
void perlin_init(unsigned int graine);

/**
 * @author Massoud
 * @brief Calcule la valeur du bruit de Perlin pour un point en deux dimensions.
 * @details Génère une valeur de bruit lisse. Pour varier la précision 
 * du relief, il convient d'ajuster la fréquence des coordonnées fournies.
 * @param x Coordonnée horizontale sur la grille.
 * @param y Coordonnée verticale sur la grille.
 * @return float Valeur du bruit comprise théoriquement dans l'intervalle [-1.0, 1.0].
 */
float perlin_2d(float x, float y);

#endif