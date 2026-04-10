#include "../lib/sanctuaire_menu.h"
#include "../lib/affichage_commun.h"
#include <math.h>
#include <string.h>

/* Bonus attribué pour chaque type de sanctuaire */
#define BONUS_SANCTUAIRE 2

/* Couleurs du thème du menu */
#define COULEUR_FOND_R   25
#define COULEUR_FOND_G   25
#define COULEUR_FOND_B   30
#define COULEUR_FOND_A  240

/* Doré foncé */
#define OR_FONCE_R  160
#define OR_FONCE_G  120
#define OR_FONCE_B   10

/* Doré plus clair pour les survols, reliefs et titres */
#define OR_CLAIR_R 210
#define OR_CLAIR_G 170
#define OR_CLAIR_B 40

/* Construit la description du sanctuaire selon son type */
static void construire_description(type_sanc_t type, char *buf) {
    switch (type) {
        case PUISSANCE:
            sprintf(buf, "Ce sanctuaire vous octroie\n+%d en Force et en Dégats.", BONUS_SANCTUAIRE);
            break;
        case RAPIDITE:
            sprintf(buf, "Ce sanctuaire vous octroie\n+%d en Rapidité.", BONUS_SANCTUAIRE);
            break;
        case INTELLIGENCE:
            sprintf(buf, "Ce sanctuaire vous octroie\n+%d en Intelligence.", BONUS_SANCTUAIRE);
            break;
        case EXPERIENCE:
            sprintf(buf, "Ce sanctuaire vous octroie\n+%d points d'experience.", BONUS_SANCTUAIRE);
            break;
        default:
            strcpy(buf, "Sanctuaire inconnu.");
            break;
    }
}

/* Applique le bonus du sanctuaire au personnage */
static void appliquer_bonus_sanctuaire(perso_t *perso, type_sanc_t type) {
    switch (type) {
        case PUISSANCE:
            perso->force  += BONUS_SANCTUAIRE;
            perso->degats += BONUS_SANCTUAIRE;
            break;
        case RAPIDITE:
            perso->rapidite += BONUS_SANCTUAIRE;
            break;
        case INTELLIGENCE:
            perso->intelligence += BONUS_SANCTUAIRE;
            break;
        case EXPERIENCE:
            perso->exp += BONUS_SANCTUAIRE;
            break;
        default:
            break;
    }
}

/* Dessine un bouton avec effet 3D */
static void dessiner_bouton_sanctuaire(SDL_Renderer *renderer, TTF_Font *font,
    SDL_Rect rect, const char *label, int survole)
{
    /* Fond du bouton */
    if (survole) {
        SDL_SetRenderDrawColor(renderer, 65, 60, 25, 255); 
    } else {
        SDL_SetRenderDrawColor(renderer, 35, 30, 15, 255);
    }
    SDL_RenderFillRect(renderer, &rect);

    /* Effet 3D ( Style rétro ) */
    if (survole) {
        SDL_SetRenderDrawColor(renderer, OR_CLAIR_R, OR_CLAIR_G, OR_CLAIR_B, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 100, 80, 20, 255);
    }
    
    /* Bordure Haut et Gauche */
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);

    /* Bordure Bas et Droite */
    SDL_SetRenderDrawColor(renderer, 15, 10, 0, 255);
    SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
    SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);

    /* Texte centré */
    SDL_Color couleur_texte = survole ? (SDL_Color){OR_CLAIR_R, OR_CLAIR_G, OR_CLAIR_B, 255} : (SDL_Color){OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255};
    SDL_Rect r_txt;
    SDL_Texture *t = creer_texte(renderer, font, label, couleur_texte, &r_txt);
    if (t) {
        SDL_Rect pos = {
            rect.x + (rect.w - r_txt.w) / 2,
            rect.y + (rect.h - r_txt.h) / 2,
            r_txt.w, r_txt.h
        };
        SDL_RenderCopy(renderer, t, NULL, &pos);
        SDL_DestroyTexture(t);
    }
}

/* Affiche le menu du sanctuaire à droite de la case cliquée. */
resultat_sanctuaire_t afficher_menu_sanctuaire(
    SDL_Renderer *renderer,
    TTF_Font *font,
    perso_t *perso,
    case_t **carte,
    int sanc_x, int sanc_y,
    int tailleCase)
{
    /* Position pixel du centre du sanctuaire */
    float cx_sanc, cy_sanc;
    case_vers_pixels(sanc_x, sanc_y, perso->x, perso->y,
                     tailleCase, renderer, &cx_sanc, &cy_sanc);

    type_sanc_t type = carte[sanc_y][sanc_x].sanctuaires;

    /* Si le sanctuaire a déjà été saisi */
    if (carte[sanc_y][sanc_x].sanctuaire_pris == 1) {
        const int PANNEAU_W = 220;
        const int PANNEAU_H = 190; /* Agrandit pour le titre */
        const int BTN_W = 100;
        const int BTN_H = 32;
        const int MARGE = 15;

        int fen_w, fen_h;
        SDL_GetRendererOutputSize(renderer, &fen_w, &fen_h);

        int rayon_hex = tailleCase / 2;
        int panneau_x = (int)(cx_sanc + rayon_hex + 8);
        if (panneau_x + PANNEAU_W > fen_w)
            panneau_x = (int)(cx_sanc - rayon_hex - 8 - PANNEAU_W);
        int panneau_y = (int)(cy_sanc - PANNEAU_H / 2);
        if (panneau_y < 0) panneau_y = 10;
        if (panneau_y + PANNEAU_H > fen_h) panneau_y = fen_h - PANNEAU_H - 10;

        SDL_Rect panneau  = {panneau_x, panneau_y, PANNEAU_W, PANNEAU_H};
        SDL_Rect btn_part = {panneau_x + (PANNEAU_W - BTN_W) / 2,
                             panneau_y + PANNEAU_H - BTN_H - MARGE,
                             BTN_W, BTN_H};

        int running_ep = 1;
        while (running_ep) {
            int sx, sy;
            SDL_GetMouseState(&sx, &sy);
            SDL_Point souris_ep = {sx, sy};
            int survole = SDL_PointInRect(&souris_ep, &btn_part);

            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) { running_ep = 0; break; }
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    SDL_Point clic = {e.button.x, e.button.y};
                    if (SDL_PointInRect(&clic, &btn_part) || !SDL_PointInRect(&clic, &panneau))
                        running_ep = 0;
                }
                if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    running_ep = 0;
            }

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

            int ty = panneau_y + MARGE + 5;
            SDL_Color or_clair = {OR_CLAIR_R, OR_CLAIR_G, OR_CLAIR_B, 255};
            SDL_Color or_fonce = {OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255};

            /* Titre centré */
            SDL_Rect r_titre;
            SDL_Texture *t_titre = creer_texte(renderer, font, "- Sanctuaire -", or_clair, &r_titre);
            if(t_titre) { 
                SDL_RenderCopy(renderer, t_titre, NULL, &(SDL_Rect){panneau_x + (PANNEAU_W - r_titre.w) / 2, ty, r_titre.w, r_titre.h}); 
                SDL_DestroyTexture(t_titre); 
                ty += r_titre.h + 8; 
            }

            /* Ligne séparatrice */
            SDL_SetRenderDrawColor(renderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 100);
            SDL_RenderDrawLine(renderer, panneau_x + MARGE, ty, panneau_x + PANNEAU_W - MARGE, ty);
            ty += 12;

            /* Message */
            const char *lignes_ep[2] = {
                "Ce sanctuaire a déjà",
                "été visité..."
            };
            
            for (int l = 0; l < 2; l++) {
                SDL_Rect r_txt;
                SDL_Texture *t = creer_texte(renderer, font, lignes_ep[l], or_fonce, &r_txt);
                if (t) {
                    SDL_Rect pos = {panneau_x + MARGE, ty, r_txt.w, r_txt.h};
                    SDL_RenderCopy(renderer, t, NULL, &pos);
                    SDL_DestroyTexture(t);
                    ty += r_txt.h + 4;
                }
            }

            /* Bouton Partir */
            dessiner_bouton_sanctuaire(renderer, font, btn_part, "Partir", survole);

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
        return SANCTUAIRE_QUITTE;
    }

    /* Construction de la description ( -> \n) */
    char description[128];
    construire_description(type, description);

    /* Dimensions du panneau */
    const int PANNEAU_W = 240;
    const int PANNEAU_H = 190; 
    const int BTN_W = 95;
    const int BTN_H = 32;
    const int MARGE = 15;
    const int GAP_BOUTONS = 10;

    int fen_w, fen_h;
    SDL_GetRendererOutputSize(renderer, &fen_w, &fen_h);

    /* On place le panneau à droite du sanctuaire par défaut,
     * et on replie à gauche si ça dépasse l'écran */
    int rayon_hex = tailleCase / 2;
    int panneau_x = (int)(cx_sanc + rayon_hex + 8);
    if (panneau_x + PANNEAU_W > fen_w)
        panneau_x = (int)(cx_sanc - rayon_hex - 8 - PANNEAU_W);

    int panneau_y = (int)(cy_sanc - PANNEAU_H / 2);
    if (panneau_y < 0) panneau_y = 10;
    if (panneau_y + PANNEAU_H > fen_h) panneau_y = fen_h - PANNEAU_H - 10;

    SDL_Rect panneau = {panneau_x, panneau_y, PANNEAU_W, PANNEAU_H};

    /* Boutons en bas du panneau, côte à côte */
    int btns_total_w = 2 * BTN_W + GAP_BOUTONS;
    int btn_start_x = panneau_x + (PANNEAU_W - btns_total_w) / 2;
    int btn_y = panneau_y + PANNEAU_H - BTN_H - MARGE;

    SDL_Rect btn_saisir = {btn_start_x, btn_y, BTN_W, BTN_H};
    SDL_Rect btn_partir = {btn_start_x + BTN_W + GAP_BOUTONS, btn_y, BTN_W, BTN_H};

    int running = 1;
    resultat_sanctuaire_t resultat = SANCTUAIRE_QUITTE;

    while (running) {
        int souris_x, souris_y;
        SDL_GetMouseState(&souris_x, &souris_y);
        SDL_Point souris = {souris_x, souris_y};

        int survole_saisir = SDL_PointInRect(&souris, &btn_saisir);
        int survole_partir = SDL_PointInRect(&souris, &btn_partir);

        /* Événements */
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
                break;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                SDL_Point clic = {e.button.x, e.button.y};
                if (SDL_PointInRect(&clic, &btn_saisir)) {
                    appliquer_bonus_sanctuaire(perso, type);
                    /* Le sanctuaire est visité */
                    carte[sanc_y][sanc_x].sanctuaire_pris = 1;
                    resultat = SANCTUAIRE_SAISI;
                    running = 0;
                } else if (SDL_PointInRect(&clic, &btn_partir)) {
                    resultat = SANCTUAIRE_QUITTE;
                    running = 0;
                } else if (!SDL_PointInRect(&clic, &panneau)) {
                    /* Clic en dehors du panneau : on ferme sans rien faire */
                    running = 0;
                }
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                running = 0;
            }
        }

        /* Rendu */
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

        /* Petite flèche pointant vers le sanctuaire */
        SDL_SetRenderDrawColor(renderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255);
        int arrow_y = panneau_y + PANNEAU_H / 2;
        if (panneau_x > (int)cx_sanc) {
            /* Panneau à droite -> flèche à gauche */
            SDL_RenderDrawLine(renderer,
                panneau.x, arrow_y - 6, panneau.x - 7, arrow_y);
            SDL_RenderDrawLine(renderer,
                panneau.x - 7, arrow_y, panneau.x, arrow_y + 6);
        } else {
            /* Panneau à gauche -> flèche à droite */
            SDL_RenderDrawLine(renderer,
                panneau.x + PANNEAU_W, arrow_y - 6,
                panneau.x + PANNEAU_W + 7, arrow_y);
            SDL_RenderDrawLine(renderer,
                panneau.x + PANNEAU_W + 7, arrow_y,
                panneau.x + PANNEAU_W, arrow_y + 6);
        }

        int ty = panneau_y + MARGE + 5;
        SDL_Color or_clair = {OR_CLAIR_R, OR_CLAIR_G, OR_CLAIR_B, 255};
        SDL_Color or_fonce = {OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255};

        /* Titre centré */
        SDL_Rect r_titre;
        SDL_Texture *t_titre = creer_texte(renderer, font, "- Sanctuaire -", or_clair, &r_titre);
        if(t_titre) { 
            SDL_RenderCopy(renderer, t_titre, NULL, &(SDL_Rect){panneau_x + (PANNEAU_W - r_titre.w) / 2, ty, r_titre.w, r_titre.h}); 
            SDL_DestroyTexture(t_titre); 
            ty += r_titre.h + 8; 
        }

        /* Ligne séparatrice */
        SDL_SetRenderDrawColor(renderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 100);
        SDL_RenderDrawLine(renderer, panneau_x + MARGE, ty, panneau_x + PANNEAU_W - MARGE, ty);
        ty += 12;

        /* Texte de description : on découpe sur '\n' */
        char ligne[128];
        const char *ptr = description;
        while (*ptr) {
            int i = 0;
            while (*ptr && *ptr != '\n') {
                ligne[i++] = *ptr++;
            }
            ligne[i] = '\0';
            if (*ptr == '\n') ptr++;

            if (i > 0) {
                SDL_Rect r_txt;
                SDL_Texture *t = creer_texte(renderer, font, ligne, or_fonce, &r_txt);
                if (t) {
                    SDL_Rect pos = {panneau_x + MARGE, ty, r_txt.w, r_txt.h};
                    SDL_RenderCopy(renderer, t, NULL, &pos);
                    SDL_DestroyTexture(t);
                    ty += r_txt.h + 4;
                }
            }
        }

        /* Boutons */
        dessiner_bouton_sanctuaire(renderer, font, btn_saisir, "Saisir",  survole_saisir);
        dessiner_bouton_sanctuaire(renderer, font, btn_partir, "Partir",  survole_partir);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return resultat;
}
