#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#include "objet.h"
#include "commun.h"

/* Structures et Enums */
#define TAILLE_INVENTAIRE 8

typedef struct {

    objet_t contenu[TAILLE_INVENTAIRE];

} inventaire_t;

/* Prototypes des fonctions */

booleen_t ajouter_objet_inventaire(inventaire_t * inventaire, objet_t objet);
booleen_t enlever_objet_inventaire(inventaire_t * inventaire, objet_type_t objet_type);
booleen_t utiliser_objet_inventaire(inventaire_t * inventaire, int indice_objet, perso_t * perso);
void initialiser_inventaire(inventaire_t * inventaire);
void liberer_memoire_inventaire(inventaire_t * inventaire);

#endif