#include "../lib/objet.h"
#include "../lib/perso.h"

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
objet_t creer_kit_de_soins(SDL_Renderer * renderer) {
    objet_t objet;

    objet.type = KIT_DE_SOINS;
    objet.nom = "Kit de Soins";
    objet.quantite = 1;
    objet.utiliser = utiliser_kit_de_soins;

    // Chargement de l'image
    SDL_Texture *texture_objet = NULL;
    SDL_Surface *img_objet = IMG_Load("img/kit_de_soins.png");
    if (img_objet) {
        texture_objet = SDL_CreateTextureFromSurface(renderer, img_objet);
        SDL_SetTextureBlendMode(texture_objet, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_objet);
    }

    objet.texture = texture_objet;

    return objet;
}

/* Leo
 * Cette fonction libère la mémoire
 * des attributs d'un objet
 */
void detruire_objet(objet_t * objet) {

    if (objet != NULL) {

        if (objet->texture != NULL) {
            SDL_DestroyTexture(objet->texture);
            objet->texture = NULL;
        }

    }

}