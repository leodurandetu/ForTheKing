#include <stdio.h>
#include <assert.h>
#include "../lib/carte.h"

#define TAILLE_CARTE_TEST 4

void reinitialiser_carte(case_t ** carte) {
    int i, j;

     if (carte != NULL) {
    
        for (i = 0; i < TAILLE_CARTE_TEST; i++) {

            for (j = 0; j < TAILLE_CARTE_TEST; j++) {
                monstre_t *monstre = carte[i][j].monstre;

                if (monstre != NULL) {
                    free(monstre);
                }

            }

        }

    }

    for (i = 0; i < TAILLE_CARTE_TEST; i++) {

        for (j = 0; j < TAILLE_CARTE_TEST; j++) {
            batiment_t batiment;

            batiment.type = PAS_DE_BATIMENT;

            carte[i][j].biome = TERRE;
            carte[i][j].terrain = PAS_DE_TERRAIN;
            carte[i][j].sanctuaires = PAS_DE_SANCTUAIRE;
            carte[i][j].batiment = batiment;
            carte[i][j].monstre = NULL;
            carte[i][j].estVisible = 1;
        }

    }

}

int main() {
    case_t ** carte;

    carte = malloc(sizeof(case_t) * TAILLE_CARTE_TEST);

    int k;

    for (k = 0; k < TAILLE_CARTE_TEST; k++) {
        carte[k] = malloc(sizeof(case_t) * TAILLE_CARTE_TEST);
    }

    reinitialiser_carte(carte);

    /*
    Test de cette fonction :

    int chemin_valide(case_t **carte, int taille_carte
        int xDepart, int yDepart, int xCible, int yCible, 
        int pts_deplacement_max, perso_t *perso, int *distance);
    */

    carte[0][1].biome = EAU;
    carte[1][0].biome = EAU;

    printf("TEST 1: Peut-on se déplacer sur une case d'eau ? Attendu : NON\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 2, 2, 1, 0, 10, NULL, NULL) == FAUX);
    printf("OK.\n\n");

    carte[1][2].terrain = ARBRES;

    printf("TEST 2: Peut-on se déplacer sur une case avec un obstacle sur le terrain (exemple : montagnes ou arbres) ? Attendu : NON\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 2, 2, 2, 1, 10, NULL, NULL) == FAUX);
    printf("OK.\n\n");

    printf("TEST 3: Peut-on atteindre une case entourée d'eau ? Attendu : NON\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 2, 2, 0, 0, 10, NULL, NULL) == FAUX);
    printf("OK.\n\n");

    reinitialiser_carte(carte);

    printf("TEST 4: Peut-on se déplacer sur une case trop lointaine ? Attendu : NON\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 3, 3, 0, 0, 1, NULL, NULL) == FAUX);
    printf("OK.\n\n");

    printf("TEST 5: Peut-on se déplacer sur une case quelconque ? Attendu: OUI\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 3, 3, 0, 0, 10, NULL, NULL) == VRAI);
    printf("OK.\n\n");

    reinitialiser_carte(carte);

    carte[1][1].monstre = creer_monstre_aleatoire(SQUELETTE, 1, 1);

    /* oui car lorsque le joueur se déplace sur la
     * case d'un monstre, il lance un combat
     */
    printf("TEST 6: Peut-on se déplacer sur une case avec un monstre dessus ? Attendu : OUI\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 3, 3, 1, 1, 10, NULL, NULL) == VRAI);
    printf("OK.\n\n");

    reinitialiser_carte(carte);

    int i, j;

    for (i = 1; i <= 3; i++) {

        for (j = 1; j <= 3; j++) {

            if (i != 2 || j != 2) {
                carte[i][j].biome = EAU;
            }

        }

    }

    printf("TEST 7: Peut-on se déplacer sur une case à l'extérieur si on est entouré d'eau ? Attendu : NON\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 2, 2, 0, 0, 10, NULL, NULL) == FAUX);
    printf("OK.\n\n");

    reinitialiser_carte(carte);

    printf("TEST 8: Peut-on se déplacer sur une case en dehors de la carte ? Attendu : NON\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 2, 2, 500, 500, 10, NULL, NULL) == FAUX);
    printf("OK.\n\n");

    printf("TEST 9: Peut-on se déplacer sur une case en dehors de la carte ? Attendu : NON\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 2, 2, 80, -41, 10, NULL, NULL) == FAUX);
    printf("OK.\n\n");

    printf("TEST 10: Peut-on se déplacer sur une case quelconque si on a aucun point de déplacement ? Attendu : NON\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 2, 2, 1, 1, 0, NULL, NULL) == FAUX);
    printf("OK.\n\n");

    carte[1][1].biome = DESERT;

    printf("TEST 11: Peut-on se déplacer sur une case de desert ? Attendu : OUI\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 2, 2, 1, 1, 10, NULL, NULL) == VRAI);
    printf("OK.\n\n");

    printf("TEST 12: Peut-on se déplacer sur notre case actuelle ? Attendu : OUI\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 2, 2, 2, 2, 10, NULL, NULL) == VRAI);
    printf("OK.\n\n");

    reinitialiser_carte(carte);

    carte[3][0].estVisible = 0;

    printf("TEST 13: Peut-on se déplacer sur une case pas visible ? Attendu : NON\n");
    assert(chemin_valide(carte, TAILLE_CARTE_TEST, 2, 2, 0, 3, 10, NULL, NULL) == FAUX);
    printf("OK.\n\n");

    liberer_memoire_carte(&carte, TAILLE_CARTE_TEST);
}