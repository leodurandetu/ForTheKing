#include "../lib/campement_menu.h"
#include "../lib/affichage_commun.h"
#include <math.h>
#include <string.h>

/* Couleurs du thème (cohérentes avec l'interface carte) */
#define COULEUR_FOND_R   25
#define COULEUR_FOND_G   25
#define COULEUR_FOND_B   30
#define COULEUR_FOND_A  240

/* Doré foncé pour le texte et les bordures */
#define OR_FONCE_R  160
#define OR_FONCE_G  120
#define OR_FONCE_B   10

/* Dimensions du panneau */
#define PANNEAU_W  210
#define PANNEAU_H  230
#define BTN_W       95
#define BTN_H       30
#define MARGE       12
#define GAP_BOUTONS 10


/*
 * Dessine un bouton avec fond sombre et texte doré foncé,
 * avec un surlignage si la souris est dessus.
 * Même style que dessiner_bouton_sanctuaire.
 */
static void dessiner_bouton_campement(SDL_Renderer *renderer, TTF_Font *font,
    SDL_Rect rect, const char *label, int survole)
{
    /* Fond du bouton : plus clair si survolé */
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


/*
 * Affiche le menu du campement à droite de la case cliquée.
 * Retourne CAMPEMENT_REPOS, CAMPEMENT_SOINS ou CAMPEMENT_QUITTE.
 */
resultat_campement_t afficher_menu_campement(
    SDL_Renderer *renderer,
    TTF_Font     *font,
    perso_t      *perso,
    int           camp_x,
    int           camp_y,
    int           tailleCase)
{
    /* Position pixel du centre du campement */
    float cx_camp, cy_camp;
    case_vers_pixels(camp_x, camp_y, perso->x, perso->y,
                     tailleCase, renderer, &cx_camp, &cy_camp);

    int fen_w, fen_h;
    SDL_GetRendererOutputSize(renderer, &fen_w, &fen_h);

    /* Placement du panneau à droite du campement, repli à gauche si dépassement */
    int rayon_hex = tailleCase / 2;
    int panneau_x = (int)(cx_camp + rayon_hex + 8);
    if (panneau_x + PANNEAU_W > fen_w)
        panneau_x = (int)(cx_camp - rayon_hex - 8 - PANNEAU_W);

    int panneau_y = (int)(cy_camp - PANNEAU_H / 2);
    if (panneau_y < 0)                panneau_y = 0;
    if (panneau_y + PANNEAU_H > fen_h) panneau_y = fen_h - PANNEAU_H;

    SDL_Rect panneau = {panneau_x, panneau_y, PANNEAU_W, PANNEAU_H};

    /* Lignes de texte de description */
    const char *lignes_desc[] = {
        "Bienvenue !",
        "Dans ce campement, vous",
        "pouvez vous reposer pour",
        "gagner des PV, ou acheter",
        "des soins."
    };
    const int nb_lignes = 5;

    /* Boutons en bas du panneau -> côte à côte */
    int btns_total_w = 2 * BTN_W + GAP_BOUTONS;
    int btn_start_x  = panneau_x + (PANNEAU_W - btns_total_w) / 2;
    int btn_y        = panneau_y + PANNEAU_H - BTN_H - MARGE;

    SDL_Rect btn_repos  = {btn_start_x,                        btn_y, BTN_W, BTN_H};
    SDL_Rect btn_soins  = {btn_start_x + BTN_W + GAP_BOUTONS,  btn_y, BTN_W, BTN_H};

    int running = 1;
    resultat_campement_t resultat = CAMPEMENT_QUITTE;

    while (running) {
        int souris_x, souris_y;
        SDL_GetMouseState(&souris_x, &souris_y);
        SDL_Point souris = {souris_x, souris_y};

        int survole_repos  = SDL_PointInRect(&souris, &btn_repos);
        int survole_soins  = SDL_PointInRect(&souris, &btn_soins);

        /* Événements */
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
                break;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                SDL_Point clic = {e.button.x, e.button.y};

                if (SDL_PointInRect(&clic, &btn_repos)) {
                    /* Todo : implémenter la logique de repos */
                    resultat = CAMPEMENT_REPOS;
                    running  = 0;
                } else if (SDL_PointInRect(&clic, &btn_soins)) {
                    /* Todo : implémenter la logique d'achat de soins */
                    resultat = CAMPEMENT_SOINS;
                    running  = 0;
                } else if (!SDL_PointInRect(&clic, &panneau)) {
                    /* Clic en dehors du panneau : fermeture sans action */
                    running = 0;
                }
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                running = 0;
            }
        }

        /* Rendu */
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        /* Fond du panneau */
        SDL_SetRenderDrawColor(renderer,
            COULEUR_FOND_R, COULEUR_FOND_G, COULEUR_FOND_B, COULEUR_FOND_A);
        SDL_RenderFillRect(renderer, &panneau);

        /* Bordure extérieure dorée claire */
        SDL_SetRenderDrawColor(renderer, 100, 90, 40, 255);
        SDL_RenderDrawRect(renderer, &panneau);

        /* Bordure intérieure sombre */
        SDL_Rect panneau_int = {panneau.x + 2, panneau.y + 2,
                                panneau.w - 4, panneau.h - 4};
        SDL_SetRenderDrawColor(renderer, 45, 40, 15, 255);
        SDL_RenderDrawRect(renderer, &panneau_int);

        /* Petite flèche pointant vers le campement */
        SDL_SetRenderDrawColor(renderer, 100, 90, 40, 255);
        int arrow_y = panneau_y + PANNEAU_H / 2;
        if (panneau_x > (int)cx_camp) {
            /* Panneau à droite -> flèche à gauche */
            SDL_RenderDrawLine(renderer,
                panneau.x,     arrow_y - 6,
                panneau.x - 7, arrow_y);
            SDL_RenderDrawLine(renderer,
                panneau.x - 7, arrow_y,
                panneau.x,     arrow_y + 6);
        } else {
            /* Panneau à gauche → flèche à droite */
            SDL_RenderDrawLine(renderer,
                panneau.x + PANNEAU_W,     arrow_y - 6,
                panneau.x + PANNEAU_W + 7, arrow_y);
            SDL_RenderDrawLine(renderer,
                panneau.x + PANNEAU_W + 7, arrow_y,
                panneau.x + PANNEAU_W,     arrow_y + 6);
        }

        /* Texte de description ligne par ligne */
        SDL_Color or_fonce = {OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255};
        int texte_y = panneau_y + MARGE;

        for (int i = 0; i < nb_lignes; i++) {
            SDL_Rect r_txt;
            SDL_Texture *t = creer_texte(renderer, font, lignes_desc[i], or_fonce, &r_txt);
            if (t) {
                SDL_Rect pos = {panneau_x + MARGE, texte_y, r_txt.w, r_txt.h};
                SDL_RenderCopy(renderer, t, NULL, &pos);
                SDL_DestroyTexture(t);
                texte_y += r_txt.h + 3;
            }
        }

        /* Séparateur fin entre le texte et les boutons */
        SDL_SetRenderDrawColor(renderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 120);
        SDL_RenderDrawLine(renderer,
            panneau_x + MARGE,               btn_y - 8,
            panneau_x + PANNEAU_W - MARGE,   btn_y - 8);

        /* Boutons */
        dessiner_bouton_campement(renderer, font, btn_repos, "Se reposer", survole_repos);
        dessiner_bouton_campement(renderer, font, btn_soins, "Acheter soins", survole_soins);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return resultat;
}