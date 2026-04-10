#include <stdio.h>
#include <string.h>
#include "../lib/quetes.h"
#include "../lib/affichage_commun.h"

#define COULEUR_FOND_R 25
#define COULEUR_FOND_G 25
#define COULEUR_FOND_B 30
#define COULEUR_FOND_A 240

#define OR_FONCE_R 160
#define OR_FONCE_G 120
#define OR_FONCE_B 10

#define OR_CLAIR_R 210
#define OR_CLAIR_G 170
#define OR_CLAIR_B 40

#define DUREE_BANDEAU_MS 2000

#define PANNEAU_W 300
#define PANNEAU_H 140
#define MARGE 15

/*
 * Tableau des données statiques des quêtes.
 * L'indice correspond à quete_id_t.
 */
static const info_quete_t QUETES_TAB[3] = {
    { "Eliminer 5 trolls", "Trolls", TROLL, 2, 2 },
    { "Eliminer 5 squelettes", "Squelettes", SQUELETTE, 1, 1 },
    { "Eliminer 5 yetis", "Yetis", YETI, 1, 2 }
};

/*
 * Cette fonction dessine un rectangle de fond sombre avec
 * ombre et double bordure dorée 
 */
static void dessiner_panneau(SDL_Renderer * renderer, SDL_Rect panneau) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    /* Ombre sous le panneau  */
    SDL_Rect ombre = {panneau.x + 4, panneau.y + 4, panneau.w, panneau.h};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, &ombre);

    /* Fond principal */
    SDL_SetRenderDrawColor(renderer, COULEUR_FOND_R, COULEUR_FOND_G, COULEUR_FOND_B, COULEUR_FOND_A);
    SDL_RenderFillRect(renderer, &panneau);

    /* Double Bordure */
    SDL_SetRenderDrawColor(renderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255);
    SDL_RenderDrawRect(renderer, &panneau);

    SDL_Rect panneau_int = {panneau.x + 3, panneau.y + 3, panneau.w - 6, panneau.h - 6};
    SDL_SetRenderDrawColor(renderer, 15, 15, 20, 255);
    SDL_RenderDrawRect(renderer, &panneau_int);
}

/*
 * Cette fonction affiche une ligne de texte dorée à
 * la position (x, y) donnée, et renvoie la hauteur du texte
 */
static int afficher_ligne(SDL_Renderer * renderer, TTF_Font * font, const char * texte, int x, int y, SDL_Color couleur) {
    SDL_Rect r;
    SDL_Texture * t = creer_texte(renderer, font, texte, couleur, &r);
    
    if (!t) {
        return 0;
    }

    SDL_Rect pos = { x, y, r.w, r.h };
    SDL_RenderCopy(renderer, t, NULL, &pos);
    SDL_DestroyTexture(t);
    
    return r.h;
}

/*
 * Cette fonction initialise la structure de quêtes
 * avec la première quête et met le compteur à 0
 */
void init_quetes(quetes_t * quetes) {
    if (quetes != NULL) {
        quetes->actuelle = QUETE_TROLLS;
        quetes->nb_tues = 0;
    }
}

/*
 * Cette fonction vérifie si le monstre tué correspond
 * à la quête active et incrémente la progression.
 * Elle retourne 1 si la quête est finie, 0 sinon
 */
int notifier_mort_monstre(quetes_t * quetes, type_monstre_t type_monstre_tue) {
    if (quetes == NULL || quetes->actuelle == QUETE_TOUTES_FINIES) {
        return 0;
    }

    if (type_monstre_tue != QUETES_TAB[quetes->actuelle].type_cible) {
        return 0;
    }

    quetes->nb_tues++;

    if (quetes->nb_tues >= QUETE_OBJECTIF) {
        return 1; 
    }

    return 0;
}

/*
 * Cette fonction permet d'afficher le panneau latéral
 * pour suivre l'avancement de la quête en cours
 */
void afficher_panneau_lateral_quetes(SDL_Renderer * renderer, TTF_Font * font, const quetes_t * quetes) {
    
    if (renderer == NULL || font == NULL || quetes == NULL) {
        return;
    }

    if (quetes->actuelle == QUETE_TOUTES_FINIES) {
        return;
    }

    int fenetre_w, fenetre_h;
    SDL_GetRendererOutputSize(renderer, &fenetre_w, &fenetre_h);

    int px = fenetre_w - PANNEAU_W - 12;
    int py = fenetre_h / 2 - PANNEAU_H - 20;

    SDL_Rect panneau = { px, py, PANNEAU_W, PANNEAU_H };
    dessiner_panneau(renderer, panneau);

    const info_quete_t * q = &QUETES_TAB[quetes->actuelle];

    int ty = py + MARGE + 5;
    SDL_Color or_clair = {OR_CLAIR_R, OR_CLAIR_G, OR_CLAIR_B, 255};
    SDL_Color or_fonce = {OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255};

    /* Titre centré */
    SDL_Rect r_titre;
    SDL_Texture * t_titre = creer_texte(renderer, font, "- Quetes annexes -", or_clair, &r_titre);
    if (t_titre) {
        SDL_Rect pos_titre = { 
            px + (PANNEAU_W - r_titre.w) / 2, 
            ty, 
            r_titre.w, 
            r_titre.h 
        };
        SDL_RenderCopy(renderer, t_titre, NULL, &pos_titre);
        SDL_DestroyTexture(t_titre);
        ty += r_titre.h + 8;
    }

    /* Ligne séparatrice */
    SDL_SetRenderDrawColor(renderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 100);
    SDL_RenderDrawLine(renderer, px + MARGE, ty, px + PANNEAU_W - MARGE, ty);
    ty += 12;

    /* Affichage de la quête */
    int h = afficher_ligne(renderer, font, q->intitule, px + MARGE, ty, or_fonce);
    ty += h + 8;

    char progression[64];
    int nb = (quetes->nb_tues > QUETE_OBJECTIF) ? QUETE_OBJECTIF : quetes->nb_tues;
    sprintf(progression, "%s elimines : %d/%d", q->ennemi_nom, nb, QUETE_OBJECTIF);
    afficher_ligne(renderer, font, progression, px + MARGE, ty, or_fonce);
}

void lancer_affichage_quete(SDL_Renderer * renderer, perso_t * perso, quetes_t * quetes, affichage_quete_t * affichage) {

    if (renderer == NULL || perso == NULL || quetes == NULL || affichage == NULL) return;
    if (quetes->actuelle == QUETE_TOUTES_FINIES) return;

    const info_quete_t * q = &QUETES_TAB[quetes->actuelle];

    /* Appliquer les bonus */
    perso->degats += q->bonus_degats;
    perso->sante += q->bonus_sante;
    perso->sante_max += q->bonus_sante;

    quetes->actuelle = (quete_id_t)(quetes->actuelle + 1);
    quetes->nb_tues = 0;

    char ligne_recompense[96];
    sprintf(ligne_recompense, "Vous gagnez +%d degats et +%d sante !", q->bonus_degats, q->bonus_sante);

    /* Initialiser l'affichage */
    affichage->lignes[0] = "Quete accomplie !";
    affichage->lignes[1] = ligne_recompense;
    affichage->active = 1;
    affichage->debut = SDL_GetTicks();

    /* Calcul position bandeau */
    int fenetre_w, fenetre_h;
    SDL_GetRendererOutputSize(renderer, &fenetre_w, &fenetre_h);
    const int largeur_bandeau = 400;
    const int hauteur_bandeau = 90;
    int bx = (fenetre_w - largeur_bandeau) / 2;
    int by = (fenetre_h - hauteur_bandeau) / 2;
    affichage->bandeau = (SDL_Rect){ bx, by, largeur_bandeau, hauteur_bandeau };
}

void update_affichage_quete(SDL_Renderer * renderer, TTF_Font * font, affichage_quete_t * affichage) {
    if (!affichage->active) return;

    Uint32 now = SDL_GetTicks();
    if (now - affichage->debut >= DUREE_BANDEAU_MS) {
        affichage->active = 0; // fin de l'affichage
        return;
    }

    /* Dessin du panneau */
    dessiner_panneau(renderer, affichage->bandeau);
    int ty = affichage->bandeau.y + MARGE + 4;

    SDL_Color or_clair = { OR_CLAIR_R, OR_CLAIR_G, OR_CLAIR_B, 255 };
    SDL_Color or_fonce = { OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255 };

    for (int i = 0; i < 2; i++) {
        SDL_Rect r;
        /* Le premier titre plus clair */
        SDL_Color couleur = (i == 0) ? or_clair : or_fonce;
        SDL_Texture * t = creer_texte(renderer, font, affichage->lignes[i], couleur, &r);

        if (t) {
            SDL_Rect pos = { affichage->bandeau.x + (affichage->bandeau.w - r.w) / 2, ty, r.w, r.h };
            SDL_RenderCopy(renderer, t, NULL, &pos);
            SDL_DestroyTexture(t);
            ty += r.h + 8;
        }

        if (i == 0) {
            /* Ligne de séparation après le titre */
            SDL_SetRenderDrawColor(renderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 100);
            SDL_RenderDrawLine(renderer, affichage->bandeau.x + MARGE, ty, affichage->bandeau.x + affichage->bandeau.w - MARGE, ty);
            ty += 8;
        }
    }
}
