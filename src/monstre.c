#include <stdlib.h>
#include <stdio.h>
#include "../lib/monstre.h"

/*
 * Cette fonction crée un monstre avec des coordonnées
 * x et y données. Ce monstre aura des attributs
 * générés aléatoirement.
 */
monstre_t * creer_monstre_aleatoire(int x, int y) {
    monstre_t *monstre = malloc(sizeof(monstre_t));

    if (!monstre) {
        printf("Erreur d'allocation mémoire lors de la création d'un monstre.");
        return NULL;
    }

    monstre->x = x;
    monstre->y = y;

    /* on changera les valeurs plus tard, elles sont provisoires */
    monstre->force = 6 + rand() % 11;
    monstre->sante = 9 + rand() % 16;
    monstre->intelligence = 6 + rand() % 11;
    monstre->rapidite = 6 + rand() % 11;
    monstre->evasion = 4 + rand() % 8;

    return monstre;
}