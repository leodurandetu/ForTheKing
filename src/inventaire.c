#include "../lib/inventaire.h"

/* Leo
 * Cette fonction permet d'ajouter un objet
 * dans un inventaire
 */
booleen_t ajouter_objet_inventaire(inventaire_t * inventaire, objet_t objet) {
    booleen_t ajout_reussi = FAUX;
    int i = 0;

    while( i < TAILLE_INVENTAIRE && ajout_reussi == FAUX) {

        if (inventaire->contenu[i].quantite <= 0) {
            inventaire->contenu[i] = objet;
            ajout_reussi = VRAI;
        } else if (inventaire->contenu[i].quantite > 0 && inventaire->contenu[i].type == objet.type) {
            inventaire->contenu[i].quantite += objet.quantite;
            ajout_reussi = VRAI;
        }

        i++;
    }

    return ajout_reussi;
}

/* Leo
 * Cette fonction permet de compter
 * les objets d'un certain type dans
 * l'inventaire
 */
int compter_objet_inventaire(inventaire_t * inventaire, objet_type_t objet_type) {
    int total = 0;
    int i = 0;

    while( i < TAILLE_INVENTAIRE) {
        objet_t contenu = inventaire->contenu[i];

        if (contenu.quantite != -1 && contenu.type == objet_type) {
            total += contenu.quantite;
        }

        i++;
    }

    return total;
}

/* Leo */
/*
 * Cette fonction permet d'ajouter
 * une certaine quantité à un type
 * d'objet dans l'inventaire
 */
void ajouter_quantite_inventaire(SDL_Renderer * renderer, inventaire_t * inventaire, objet_type_t objet_type, int quantite) {
    booleen_t objet_trouve = FAUX;
    int i = 0;

    while (i < TAILLE_INVENTAIRE && objet_trouve == FAUX) {
        objet_t contenu = inventaire->contenu[i];

        if (contenu.quantite != -1 && contenu.type == objet_type) {
            inventaire->contenu[i].quantite += quantite;
            objet_trouve = VRAI;
        }

        i++;
    }

    if (objet_trouve == FAUX) {
        ajouter_objet_inventaire(inventaire, creer_objet(renderer, objet_type, quantite));
    }

}

 /* Leo */
/*
 * Cette fonction permet d'enlever
 * une certaine quantité à un type
 * d'objet dans l'inventaire
 */
void enlever_quantite_inventaire(inventaire_t * inventaire, objet_type_t objet_type, int quantite) {
    int i = 0;
    int reste_a_enlever = quantite;

    while (i < TAILLE_INVENTAIRE && reste_a_enlever > 0) {
        objet_t contenu = inventaire->contenu[i];

        if (contenu.quantite != -1 && contenu.type == objet_type) {

            while (inventaire->contenu[i].quantite > 0 && reste_a_enlever > 0) {
                inventaire->contenu[i].quantite--;
                reste_a_enlever--;
            }

        }

        i++;
    }

}

/* Leo
 * Cette fonction permet d'enlever un
 * objet d'un certain type dans
 * l'inventaire
 */
booleen_t enlever_objet_inventaire(inventaire_t * inventaire, objet_type_t objet_type) {
    booleen_t objet_trouve = FAUX;
    int i = 0;

    while( i < TAILLE_INVENTAIRE && objet_trouve == FAUX) {
        objet_t contenu = inventaire->contenu[i];

        if (contenu.quantite != -1 && contenu.type == objet_type) {
            inventaire->contenu[i].quantite = -1;
            objet_trouve = VRAI;
        }

        i++;
    }

    return objet_trouve;
}

/* Leo
 * Cette fonction permet d'utiliser
 * un objet présent dans l'inventaire
 */
booleen_t utiliser_objet_inventaire(inventaire_t * inventaire, int indice_objet, perso_t * perso, int * maj_affichage) {
    booleen_t objet_trouve = FAUX;

    if (indice_objet >= 0 && indice_objet < TAILLE_INVENTAIRE) {

        if (inventaire->contenu[indice_objet].quantite > 0) {
            utiliser_objet(&(inventaire->contenu[indice_objet]), perso);

            if (inventaire->contenu[indice_objet].type != PIECE_DOR) {
                inventaire->contenu[indice_objet].quantite--;
            }

            objet_trouve = VRAI;
            *maj_affichage = 1;
        }

    }

    return objet_trouve = VRAI;
}

/* Leo
 * Cette fonction permet d'initialiser
 * l'inventaire, elle doit être appelée
 * avant la boucle principale dans le main.
 */
void initialiser_inventaire(inventaire_t * inventaire) {
    int i;

    for (i = 0; i < TAILLE_INVENTAIRE; i++) {
        inventaire->contenu[i].quantite = -1;
        inventaire->contenu[i].texture = NULL;
    }
    
}

/* Leo
 * Cette fonction libère la mémoire
 * de l'inventaire. Elle doit être
 * appelée à la fin du main uniquement.
 */
void liberer_memoire_inventaire(inventaire_t * inventaire) {
    int i;

    for (i = 0; i < TAILLE_INVENTAIRE; i++) {
        detruire_objet(&(inventaire->contenu[i]));
        inventaire->contenu[i].quantite = -1;
    }

}