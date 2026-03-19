#include "../lib/objet.h"

/* Leo
 * Cette fonction est appelée lorsque le
 * personnage utilise un kit de soins
 */
void utiliser_kit_de_soins(perso_t * perso) {
    printf("Vous avez utilisé un kit de soins.\n");

    int pv_supplementaires = (int) (0.25f * perso->sante_max);

    int pv = perso->sante;

    int nouveauPv = pv + pv_supplementaires;

    if (nouveauPv > perso->sante_max) {
        nouveauPv = perso->sante_max;
    }

    perso->sante = nouveauPv;
}

/* Leo
 * Cette fonction crée un nouvel objet
 * de type kit de soins et le renvoie
 */
objet_t creer_kit_de_soins() {
    objet_t objet;

    objet.type = KIT_DE_SOINS;
    objet.nom = "Kit de Soins";
    objet.texture = NULL;
    objet.quantite = 1;

    objet.utiliser = utiliser_kit_de_soins;
    
    return objet;
}