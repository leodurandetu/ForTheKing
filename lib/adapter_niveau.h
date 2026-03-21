#ifndef ADAPTER_NIVEAU_H
#define ADAPTER_NIVEAU_H

#include "perso.h"
#include "monstre.h"

/* Applique les bonus de stats au personnage
 * selon son niveau. À appeler lors d'une
 * montée de niveau. */
void appliquer_bonus_perso(perso_t *perso);

/* Applique les bonus de stats au monstre
 * selon son niveau. À appeler à la création
 * du monstre. */
void appliquer_bonus_monstre(monstre_t *monstre);

/* Attribue un niveau aléatoire au monstre
 * selon son type :
 * - TROLL     : niveau 1 ou 2
 * - SQUELETTE : niveau 2 ou 3 */
void niveau_monstre(monstre_t *monstre, type_monstre_t type);

#endif