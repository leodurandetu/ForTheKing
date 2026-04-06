#include "../lib/sanctuaire_menu.h"
#include "../lib/affichage_commun.h"
#include <math.h>
#include <string.h>

/* Bonus attribué pour chaque type de sanctuaire */
#define BONUS_SANCTUAIRE 2

/* Couleurs du thème du menu (cohérentes avec l'interface carte) */
#define COULEUR_FOND_R   25
#define COULEUR_FOND_G   25
#define COULEUR_FOND_B   30
#define COULEUR_FOND_A  240

/* Doré foncé pour le texte principal */
#define OR_FONCE_R  160
#define OR_FONCE_G  120
#define OR_FONCE_B   10

/* Construit la description du sanctuaire selon son type */
static void construire_description(type_sanc_t type, char *buf) {
    switch (type) {
        case PUISSANCE:
            sprintf(buf, "Ce sanctuaire vous octroie\n+%d en Force et en Degats.", BONUS_SANCTUAIRE);
            break;
        case RAPIDITE:
            sprintf(buf, "Ce sanctuaire vous octroie\n+%d en Rapidite.", BONUS_SANCTUAIRE);
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

/* Calcule la position pixel (cx, cy) du centre d'une case hexagonale
 * à partir de ses coordonnées carte, de la taille de case et du joueur */
static void case_vers_pixels(int cx_carte, int cy_carte,
    int perso_x, int perso_y,
    int tailleCase,
    SDL_Renderer *renderer,
    float *px, float *py)
{
    int fen_w, fen_h;
    SDL_GetRendererOutputSize(renderer, &fen_w, &fen_h);

    float rayon         = tailleCase / 2.0f;
    float hex_w         = 2.0f * rayon;
    float hex_h         = sqrtf(3.0f) * rayon;
    float espacement_x  = hex_w * 0.75f;

    /* Position du joueur en pixels */
    float cam_x = perso_x * espacement_x;
    float cam_y = perso_y * hex_h + (perso_x % 2 ? hex_h / 2.0f : 0.0f);

    int decalage_x = (int)(fen_w / 2 - cam_x - hex_w / 2);
    int decalage_y = (int)(fen_h / 2 - cam_y - hex_h / 2);

    *px = cx_carte * espacement_x + hex_w / 2.0f + decalage_x;
    *py = cy_carte * hex_h + (cx_carte % 2 ? hex_h / 2.0f : 0.0f) + hex_h / 2.0f + decalage_y;
}

/* Dessine un bouton avec fond sombre et texte doré foncé,
 * avec un surlignage si la souris est dessus */
static void dessiner_bouton_sanctuaire(SDL_Renderer *renderer, TTF_Font *font,
    SDL_Rect rect, const char *label, int survole)
{
    /* Fond du bouton : légèrement plus clair si survolé */
    if (survole) {
        SDL_SetRenderDrawColor(renderer, 55, 50, 20, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 35, 30, 10, 255);
    }
    SDL_RenderFillRect(renderer, &rect);

    /* Bordure dorée */
    SDL_SetRenderDrawColor(renderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255);
    SDL_RenderDrawRect(renderer, &rect);

    /* Texte centré dans le bouton */
    SDL_Color couleur_texte = {OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255};
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

/* Affiche le menu du sanctuaire à droite de la case cliquée.
 * Retourne SANCTUAIRE_SAISI ou SANCTUAIRE_QUITTE. */
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

    /* Si le sanctuaire a déjà été saisi, on affiche un message d'information */
    if (carte[sanc_y][sanc_x].sanctuaire_pris == 1) {
        const int PANNEAU_W = 220;
        const int PANNEAU_H = 120;
        const int BTN_W     = 100;
        const int BTN_H     = 28;
        const int MARGE     = 14;

        int fen_w, fen_h;
        SDL_GetRendererOutputSize(renderer, &fen_w, &fen_h);

        int rayon_hex = tailleCase / 2;
        int panneau_x = (int)(cx_sanc + rayon_hex + 8);
        if (panneau_x + PANNEAU_W > fen_w)
            panneau_x = (int)(cx_sanc - rayon_hex - 8 - PANNEAU_W);
        int panneau_y = (int)(cy_sanc - PANNEAU_H / 2);
        if (panneau_y < 0) panneau_y = 0;
        if (panneau_y + PANNEAU_H > fen_h) panneau_y = fen_h - PANNEAU_H;

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

            /* Fond du panneau */
            SDL_SetRenderDrawColor(renderer, COULEUR_FOND_R, COULEUR_FOND_G, COULEUR_FOND_B, COULEUR_FOND_A);
            SDL_RenderFillRect(renderer, &panneau);

            /* Bordures */
            SDL_SetRenderDrawColor(renderer, 100, 90, 40, 255);
            SDL_RenderDrawRect(renderer, &panneau);
            SDL_Rect pan_int = {panneau.x+2, panneau.y+2, panneau.w-4, panneau.h-4};
            SDL_SetRenderDrawColor(renderer, 45, 40, 15, 255);
            SDL_RenderDrawRect(renderer, &pan_int);

            /* Message sur deux lignes */
            SDL_Color or_fonce = {OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255};
            const char *lignes_ep[2] = {
                "Malheureusement, ce",
                "sanctuaire a déjà été visité..."
            };
            int ty = panneau_y + MARGE;
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

    /* Construction de la description (peut contenir \n) */
    char description[128];
    construire_description(type, description);

    /* Dimensions du panneau */
    const int PANNEAU_W   = 220;
    const int PANNEAU_H   = 130;
    const int BTN_W       = 85;
    const int BTN_H       = 28;
    const int MARGE       = 10;
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
    if (panneau_y < 0)            panneau_y = 0;
    if (panneau_y + PANNEAU_H > fen_h) panneau_y = fen_h - PANNEAU_H;

    SDL_Rect panneau = {panneau_x, panneau_y, PANNEAU_W, PANNEAU_H};

    /* Boutons en bas du panneau, côte à côte */
    int btns_total_w = 2 * BTN_W + GAP_BOUTONS;
    int btn_start_x  = panneau_x + (PANNEAU_W - btns_total_w) / 2;
    int btn_y        = panneau_y + PANNEAU_H - BTN_H - MARGE;

    SDL_Rect btn_saisir = {btn_start_x,                   btn_y, BTN_W, BTN_H};
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
                    /* Le sanctuaire est consommé */
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

        /* --- Rendu --- */
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        /* Fond du panneau */
        SDL_SetRenderDrawColor(renderer,
            COULEUR_FOND_R, COULEUR_FOND_G, COULEUR_FOND_B, COULEUR_FOND_A);
        SDL_RenderFillRect(renderer, &panneau);

        /* Bordure extérieure claire */
        SDL_SetRenderDrawColor(renderer, 100, 90, 40, 255);
        SDL_RenderDrawRect(renderer, &panneau);

        /* Bordure intérieure sombre pour l'effet double */
        SDL_Rect panneau_int = {panneau.x + 2, panneau.y + 2,
                                panneau.w - 4, panneau.h - 4};
        SDL_SetRenderDrawColor(renderer, 45, 40, 15, 255);
        SDL_RenderDrawRect(renderer, &panneau_int);

        /* Petite flèche pointant vers le sanctuaire */
        SDL_SetRenderDrawColor(renderer, 100, 90, 40, 255);
        int arrow_y = panneau_y + PANNEAU_H / 2;
        if (panneau_x > (int)cx_sanc) {
            /* Panneau à droite → flèche à gauche */
            SDL_RenderDrawLine(renderer,
                panneau.x, arrow_y - 6, panneau.x - 7, arrow_y);
            SDL_RenderDrawLine(renderer,
                panneau.x - 7, arrow_y, panneau.x, arrow_y + 6);
        } else {
            /* Panneau à gauche → flèche à droite */
            SDL_RenderDrawLine(renderer,
                panneau.x + PANNEAU_W, arrow_y - 6,
                panneau.x + PANNEAU_W + 7, arrow_y);
            SDL_RenderDrawLine(renderer,
                panneau.x + PANNEAU_W + 7, arrow_y,
                panneau.x + PANNEAU_W, arrow_y + 6);
        }

        /* Texte de description : on découpe manuellement sur le \n */
        SDL_Color or_fonce = {OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255};
        int texte_y = panneau_y + MARGE;

        char ligne[128];
        const char *ptr = description;
        while (*ptr) {
            /* Récupère jusqu'au prochain \n ou fin de chaîne */
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
                    SDL_Rect pos = {panneau_x + MARGE, texte_y, r_txt.w, r_txt.h};
                    SDL_RenderCopy(renderer, t, NULL, &pos);
                    SDL_DestroyTexture(t);
                    texte_y += r_txt.h + 4;
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