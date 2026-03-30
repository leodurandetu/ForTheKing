#include <stdio.h>
#include <stdlib.h>
#include "../lib/adapter_niveau.h"

/* Massoud
 * Applique les bonus de stats au personnage
 * en fonction de son niveau, appelée lors
 * d'une montée de niveau dans niveau.c
 */
void appliquer_bonus_perso(perso_t *perso) {
    switch(perso->niveau) {
        case 2: perso->degats += 5;
                perso->force += 5;
                perso->intelligence += 5;
                perso->sante += 7;
                perso->sante_max += 7 ;
                perso->rapidite += 5;
                perso->evasion += 5;
                break;
        case 3: perso->degats += 4;
                perso->force += 4;
                perso->intelligence += 4;
                perso->sante += 5;
                perso->sante_max += 5;
                perso->rapidite += 4;
                perso->evasion += 4;
                break;
        case 4: perso->degats += 3;
                perso->force += 3;
                perso->intelligence += 3;
                perso->sante += 3;
                perso->sante_max += 3;
                perso->rapidite += 3;
                perso->evasion += 3;
                break;
        case 5: perso->degats += 2;
                perso->force += 2;
                perso->intelligence += 2;
                perso->sante += 2;
                perso->sante_max += 2;
                perso->rapidite += 2;
                perso->evasion += 2;
                break;
        default: printf("Erreur niveau personnage.\n"); break;
    }
}

/* Massoud
 * Applique les bonus de stats au monstre
 * en fonction de son niveau, appelée lors
 * de la création du monstre dans monstre.c
 */
void appliquer_bonus_monstre(monstre_t *monstre) {
    switch(monstre->niveau) {
        case 2: monstre->degats += 2;
                monstre->force += 2;
                monstre->intelligence += 2;
                monstre->sante += 3;
                monstre->sante_max += 3;
                monstre->rapidite += 2;
                monstre->evasion += 2;
                break;
        case 3: monstre->degats += 4;
                monstre->force += 4;
                monstre->intelligence += 4;
                monstre->sante += 4;
                monstre->sante_max += 4;
                monstre->rapidite += 4;
                monstre->evasion += 4;
                break;
        case 4: monstre->degats += 5;
                monstre->force += 5;
                monstre->intelligence += 5;
                monstre->sante += 5;
                monstre->sante_max += 5;
                monstre->rapidite += 5;
                monstre->evasion += 5;
                break;
        case 5: monstre->degats += 6;
                monstre->force += 6;
                monstre->intelligence += 6;
                monstre->sante += 6;
                monstre->sante_max += 6;
                monstre->rapidite += 6;
                monstre->evasion += 6;
                break;
        default: break; // niveau 1 = pas de bonus
    }
}

/* Massoud
 * Attribue un niveau aléatoire au monstre
 * selon son type, appelée dans monstre.c
 * lors de la création du monstre.
 * - TROLL     : niveau 1 ou 2
 * - SQUELETTE : niveau 2 ou 3
 */
void niveau_monstre(monstre_t *monstre, type_monstre_t type) {
        int niveau_final = 1;

        switch(type) {
                case TROLL:
                        niveau_final = 1 + rand() % 2;
                        break;
                case SQUELETTE:
                        niveau_final = 2 + rand() % 2;
                        break;
                case YETI:
                        niveau_final = 4 + rand() % 2;
                        break;
                default:
                        printf("Erreur niveau : func niveau_troll.\n");
                        break;
        }

        monstre->niveau = 1;

        for (int i = 1; i <= niveau_final; i++) {
                monstre->niveau = monstre->niveau + 1;
                appliquer_bonus_monstre(monstre);
        }

}
