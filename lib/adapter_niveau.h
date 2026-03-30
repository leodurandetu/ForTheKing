#ifndef ADAPTER_NIVEAU_H
#define ADAPTER_NIVEAU_H

#include "monstre.h"
#include "perso.h"

/**
 * @author Massoud
 * @brief Met à jour les statistiques du personnage lors d'un gain de niveau.
 * @details Calcule et applique les augmentations de caractéristiques (PV, attaque, etc.)
 * en fonction du nouveau niveau du joueur.
 * @param perso Pointeur vers la structure du personnage à améliorer.
 */
void appliquer_bonus_perso(perso_t *perso);

/**
 * @author Massoud
 * @brief Définit les statistiques d'un monstre selon son niveau actuel.
 * @details À utiliser lors de l'instanciation d'un monstre pour adapter sa difficulté 
 * (scaling) avant le combat.
 * @param monstre Pointeur vers le monstre dont on souhaite ajuster les stats.
 */
void appliquer_bonus_monstre(monstre_t *monstre);

/**
 * @author Massoud
 * @brief Génère aléatoirement le niveau d'un monstre selon sa catégorie.
 * @details Définit les plages de niveaux suivantes :
 * - TROLL     : Niveau 1 ou 2.
 * - SQUELETTE : Niveau 2 ou 3.
 * @param monstre Pointeur vers le monstre qui va recevoir le niveau.
 * @param type Le type de monstre permettant de déterminer la plage de niveau aléatoire.
 */
void niveau_monstre(monstre_t *monstre, type_monstre_t type);

#endif