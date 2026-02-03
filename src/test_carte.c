#include <stdio.h>
#include "carte.h"
#include <stdlib.h>
#include <time.h>

case_t carte[TAILLE_CARTE][TAILLE_CARTE];

int main() {

    srand(time(NULL));
    printf("Test de la carte de For The King :\n");

    init_carte(carte);

    generer_eau(carte);

    afficher_carte(carte);
}