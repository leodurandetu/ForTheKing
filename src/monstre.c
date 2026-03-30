#include <stdlib.h>
#include <stdio.h>
#include "../lib/monstre.h"

/* Leo */
/*
 * Cette fonction crée un monstre avec des coordonnées
 * x et y données. Ce monstre aura des attributs
 * générés aléatoirement.
 */
monstre_t * creer_monstre_aleatoire(type_monstre_t type, int x, int y) {
    monstre_t *monstre = malloc(sizeof(monstre_t));

    if (!monstre) {
        printf("Erreur d'allocation mémoire lors de la création d'un monstre.");
        return NULL;
    }

    monstre->type = type;
    monstre->x = x;
    monstre->y = y;

     /* Niveau selon le type */
    if (monstre->type == TROLL) {
        monstre->niveau = 1;
    } else if (monstre->type == SQUELETTE) {
        monstre->niveau = 1 + rand() % 2;
    } else if (monstre->type == YETI) {
        monstre->niveau = 3 + rand() % 2;
    }


    /* on changera les valeurs plus tard, elles sont provisoires */
    monstre->force = 6 + rand() % 11;
    monstre->sante = 9 + rand() % 10;
    monstre->sante_max = monstre->sante;
    monstre->degats = 3 + rand() % 7;
    monstre->intelligence = 6 + rand() % 11;
    monstre->rapidite = 6 + rand() % 11;
    monstre->evasion = 4 + rand() % 8;

    return monstre;
}

monstre_t * creer_boss_final() {
    monstre_t *monstre = malloc(sizeof(monstre_t));

    if (!monstre) {
        printf("Erreur d'allocation mémoire lors de la création d'un monstre.");
        return NULL;
    }

    monstre->type = BOSS_FINAL;

    /* le boss final n'est pas sur la carte */
    monstre->x = -1;
    monstre->y = -1;

     /* Niveau selon le type */
    monstre->niveau = 8;

    /* on changera les valeurs plus tard, elles sont provisoires */
    monstre->force = 54 + rand() % 33;
    monstre->sante = 111 + rand() % 40;
    monstre->sante_max = monstre->sante;
    monstre->degats = 27 + rand() % 21;
    monstre->intelligence = 54 + rand() % 33;
    monstre->rapidite = 54 + rand() % 33;
    monstre->evasion = 36 + rand() % 24;

    return monstre;
}