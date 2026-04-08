#include <stdio.h>
#include <string.h>
#include "../lib/quetes.h"
#include "../lib/affichage_commun.h"

#define COULEUR_FOND_R 25
#define COULEUR_FOND_G 25
#define COULEUR_FOND_B 30
#define COULEUR_FOND_A 220

#define OR_FONCE_R 160
#define OR_FONCE_G 120
#define OR_FONCE_B 10

#define DUREE_BANDEAU_MS 2000

#define PANNEAU_W 300
#define PANNEAU_H 110
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
 * une double bordure dorée, pour l'interface des quêtes
 */
static void dessiner_panneau(SDL_Renderer * renderer, SDL_Rect panneau) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, COULEUR_FOND_R, COULEUR_FOND_G, COULEUR_FOND_B, COULEUR_FOND_A);
    SDL_RenderFillRect(renderer, &panneau);

    SDL_SetRenderDrawColor(renderer, 100, 90, 40, 255);
    SDL_RenderDrawRect(renderer, &panneau);

    SDL_Rect interieur = { panneau.x + 2, panneau.y + 2, panneau.w - 4, panneau.h - 4 };
    SDL_SetRenderDrawColor(renderer, 45, 40, 15, 255);
    SDL_RenderDrawRect(renderer, &interieur);
}


/*
 * Cette fonction affiche une ligne de texte dorée à
 * la position (x, y) donnée, et renvoie la hauteur du texte
 */
static int afficher_ligne(SDL_Renderer * renderer, TTF_Font * font, const char * texte, int x, int y) {
    SDL_Color or_fonce = { OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255 };
    SDL_Rect r;
    SDL_Texture * t = creer_texte(renderer, font, texte, or_fonce, &r);
    
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
void afficher_quetes(SDL_Renderer * renderer, TTF_Font * font, const quetes_t * quetes) {
    
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

    int hauteur_titre = 30;
    SDL_Rect boite_titre = { px + 5, py + 5, PANNEAU_W - 10, hauteur_titre };
    
    // Fond de l'encadré 
    SDL_SetRenderDrawColor(renderer, 35, 30, 20, 255); 
    SDL_RenderFillRect(renderer, &boite_titre);
    
    // Bordure de l'encadré
    SDL_SetRenderDrawColor(renderer, 120, 100, 30, 255); 
    SDL_RenderDrawRect(renderer, &boite_titre);

    // Affichage du texte centré dans cet encadré
    SDL_Color or_fonce = { OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255 };
    SDL_Rect r_titre;
    SDL_Texture * t_titre = creer_texte(renderer, font, "Quetes annexes", or_fonce, &r_titre);
    
    if (t_titre) {
        SDL_Rect pos_titre = { 
            px + (PANNEAU_W - r_titre.w) / 2,         // Centré horizontalement
            py + 5 + (hauteur_titre - r_titre.h) / 2, // Centré verticalement
            r_titre.w, 
            r_titre.h 
        };
        SDL_RenderCopy(renderer, t_titre, NULL, &pos_titre);
        SDL_DestroyTexture(t_titre);
    }

    // Positionne le reste du texte sous l'encadré
    int ty = py + 5 + hauteur_titre + 12; 
    
    int h = afficher_ligne(renderer, font, q->intitule, px + MARGE, ty);
    ty += h + 4;

    char progression[64];
    int nb = (quetes->nb_tues > QUETE_OBJECTIF) ? QUETE_OBJECTIF : quetes->nb_tues;
    sprintf(progression, "%s elimines : %d/%d", q->ennemi_nom, nb, QUETE_OBJECTIF);
    afficher_ligne(renderer, font, progression, px + MARGE, ty);
}


void afficher_quete_accomplie(SDL_Renderer * renderer, TTF_Font * font, perso_t * perso, quetes_t * quetes) {
    
    if (renderer == NULL || font == NULL || perso == NULL || quetes == NULL) {
        return;
    }
    
    if (quetes->actuelle == QUETE_TOUTES_FINIES) {
        return;
    }

    const info_quete_t * q = &QUETES_TAB[quetes->actuelle];

    perso->degats += q->bonus_degats;
    perso->sante += q->bonus_sante;
    perso->sante_max += q->bonus_sante;

    quetes->actuelle = (quete_id_t)(quetes->actuelle + 1);
    quetes->nb_tues = 0;

    char ligne_recompense[96];
    sprintf(ligne_recompense, "Vous gagnez +%d degats et +%d sante !", q->bonus_degats, q->bonus_sante);

    const char * lignes[2] = { "Quete accomplie !", ligne_recompense };

    int fenetre_w, fenetre_h;
    SDL_GetRendererOutputSize(renderer, &fenetre_w, &fenetre_h);

    const int largeur_bandeau = 380;
    const int hauteur_bandeau = 80;
    int bx = (fenetre_w - largeur_bandeau) / 2;
    int by = (fenetre_h - hauteur_bandeau) / 2;

    SDL_Rect bandeau = { bx, by, largeur_bandeau, hauteur_bandeau };
    Uint32 debut = SDL_GetTicks();

    while (SDL_GetTicks() - debut < DUREE_BANDEAU_MS) {
        SDL_Event e;
        
        while (SDL_PollEvent(&e)) {
            
            if (e.type == SDL_QUIT) {
                return;
            }

        }

        dessiner_panneau(renderer, bandeau);
        int ty = by + MARGE + 4;
        
        for (int i = 0; i < 2; i++) {
            SDL_Rect r;
            SDL_Color or_fonce = { OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255 };
            SDL_Texture * t = creer_texte(renderer, font, lignes[i], or_fonce, &r);
            
            if (t) {
                SDL_Rect pos = { bx + (largeur_bandeau - r.w) / 2, ty, r.w, r.h };
                SDL_RenderCopy(renderer, t, NULL, &pos);
                SDL_DestroyTexture(t);
                ty += r.h + 6;
            }

        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

}

