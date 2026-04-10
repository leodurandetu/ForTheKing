#include <stdio.h>
#include "../lib/niveau.h"
#include "../lib/adapter_niveau.h"

/* Leo
 * Cette fonction donne le nombre
 * de points d'XP nécessaire pour
 * passer au niveau suivant.
 */
int xp_necessaire(int niveau) {
    if (niveau <= 1) {
        return 20;
    } else {
        return (int)(1.5f * xp_necessaire(niveau - 1));
    }
}

/* Leo
 * Cette fonction vérifie si
 * le joueur a assez d'XP
 * pour monter d'un niveau
 */
void verif_xp(perso_t *perso) {
    int xpMax = xp_necessaire(perso->niveau);
    if (perso->exp >= xpMax) {
        perso->niveau++;
        perso->exp -= xpMax;
        appliquer_bonus_perso(perso);
        printf("Niveau supérieur ! Niveau %d atteint.\n", perso->niveau);
    }
}

/* Leo
 * Cette fonction rajoute des
 * points d'XP à un personnage
 * lorsqu'il a tué un monstre
 */
void monstre_tue(perso_t *perso, monstre_t *monstre) {
    attribuer_experience_victoire(perso, monstre);
    verif_xp(perso);
}
