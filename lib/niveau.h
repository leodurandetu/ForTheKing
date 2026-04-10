#ifndef NIVEAU_H
#define NIVEAU_H

#include "perso.h"
#include "monstre.h"

/**
 * @file niveau.h
 * @author Massoud & Léo
 * @brief Gestion de la progression, de l'expérience et de la montée de niveau.
 */

/* --- Prototypes des fonctions --- */

/**
 * @author Massoud & Léo
 * @brief Calcule le montant d'expérience requis pour atteindre un niveau donné.
 * @details Définit le seuil nécessaire en suivant une courbe de progression (linéaire ou exponentielle).
 * @param niveau Le niveau cible pour lequel on souhaite connaître le palier d'XP.
 * @return int Le nombre de points d'expérience à atteindre.
 */
int xp_necessaire(int niveau);

/**
 * @author Massoud & Léo
 * @brief Vérifie si le personnage a accumulé assez d'XP pour monter de niveau.
 * @details Compare l'XP actuelle au seuil requis. En cas de succès, incrémente le niveau 
 * et déclenche la mise à jour des statistiques du personnage.
 * @param perso Pointeur vers le personnage à contrôler.
 */
void verif_xp(perso_t *perso);

/**
 * @author Massoud & Léo
 * @brief Gère l'attribution des gains suite à la défaite d'un monstre.
 * @details Calcule et ajoute l'expérience au personnage, puis appelle verif_xp() 
 * pour actualiser son état de progression.
 * @param perso Pointeur vers le personnage qui reçoit l'expérience.
 * @param monstre Pointeur vers la structure du monstre.
 */
void monstre_tue(perso_t *perso, monstre_t *monstre);

#endif
