#include "../lib/objet.h"
#include "../lib/perso.h"

/* Leo
 * Cette fonction est appelée lorsque le
 * personnage utilise un kit de soins
 */
void utiliser_kit_de_soins(perso_t * perso) {
    printf("Vous avez utilisé un kit de soins.\n");

    int pv_supplementaires = (int) (0.35f * perso->sante_max);

    int pv = perso->sante;

    int nouveauPv = pv + pv_supplementaires;

    if (nouveauPv > perso->sante_max) {
        nouveauPv = perso->sante_max;
    }

    perso->sante = nouveauPv;
}

void utiliser_objet(objet_t * objet, perso_t * perso) {

    if (objet != NULL) {

        switch(objet->type) {

            case KIT_DE_SOINS:
                utiliser_kit_de_soins(perso);
                break;

            case PIECE_DOR:
                break;

        }
        
    }

}

/* Leo
 * Cette fonction crée un nouvel objet
 * d'un certain type avec une certaine
 * quantité et le renvoie
 */
objet_t creer_objet(SDL_Renderer * renderer, objet_type_t type, int nombre) {
    objet_t objet;

    objet.type = type;
    objet.quantite = nombre;
    objet.texture = texture_objet(renderer, type);

    return objet;
}

void remplir_texture_objet(objet_t * objet, SDL_Renderer * renderer) {

    if (objet != NULL) {
        objet->texture = texture_objet(renderer, objet->type);
    }

}

static SDL_Texture * texture(SDL_Renderer * renderer, char * chemin) {
    SDL_Texture *texture_objet = NULL;
    SDL_Surface *img_objet = IMG_Load(chemin);
    if (img_objet) {
        texture_objet = SDL_CreateTextureFromSurface(renderer, img_objet);
        SDL_SetTextureBlendMode(texture_objet, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_objet);
    }

    return texture_objet;
}

SDL_Texture * texture_objet(SDL_Renderer * renderer, objet_type_t type) {

    switch(type) {

        case KIT_DE_SOINS:
            return texture(renderer, "img/kit_de_soins.png");
            break;

        case PIECE_DOR:
            return texture(renderer, "img/piece_dor.png");
            break;

        default:
            return NULL;
            break;

    }

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