#include <stdio.h>
#include "carte.h"

case_t carte[TAILLE_CARTE][TAILLE_CARTE];

int main() {
    printf("Test de la carte de For The King :\n");

    init_carte(carte);

    afficher_carte(carte);
}