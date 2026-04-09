#include "../lib/campement_menu.h"
#include "../lib/affichage_commun.h"
#include "../lib/perso.h"
#include <math.h>
#include <string.h>

/* Couleurs du thème  */
#define COULEUR_FOND_R 25
#define COULEUR_FOND_G 25
#define COULEUR_FOND_B 30
#define COULEUR_FOND_A 240

/* Doré de base */
#define OR_FONCE_R 160
#define OR_FONCE_G 120
#define OR_FONCE_B 10

/* Doré plus clair pour les survols et reliefs */
#define OR_CLAIR_R 210
#define OR_CLAIR_G 170
#define OR_CLAIR_B 40

#define PANNEAU_W 220 
#define PANNEAU_H 250
#define BTN_W 95
#define BTN_H 32

#define MARGE 15
#define GAP_BOUTONS 10

/*
 * Dessine un bouton avec un effet de relief 
 */
static void dessiner_bouton_campement(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect rect, const char *label, int survole) {
    /* Fond du bouton */
    if (survole) {
        SDL_SetRenderDrawColor(renderer, 65, 60, 25, 255); /* Plus lumineux au survol */
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
        SDL_Rect pos = { rect.x + (rect.w - r_txt.w) / 2, rect.y + (rect.h - r_txt.h) / 2, r_txt.w, r_txt.h };
        SDL_RenderCopy(renderer, t, NULL, &pos);
        SDL_DestroyTexture(t);
    }
}

resultat_campement_t afficher_menu_campement(SDL_Renderer *renderer, TTF_Font *font, 
                                perso_t *perso, int camp_x, int camp_y, int tailleCase) {
    /* Calcul de positionnement */
    float cx_camp, cy_camp;
    case_vers_pixels(camp_x, camp_y, perso->x, perso->y, tailleCase, renderer, &cx_camp, &cy_camp);
    int fen_w, fen_h;
    SDL_GetRendererOutputSize(renderer, &fen_w, &fen_h);
    
    int rayon_hex = tailleCase / 2;
    int panneau_x = (int)(cx_camp + rayon_hex + 12); 

    if (panneau_x + PANNEAU_W > fen_w) panneau_x = (int)(cx_camp - rayon_hex - 12 - PANNEAU_W);
    int panneau_y = (int)(cy_camp - PANNEAU_H / 2);

    if (panneau_y < 0) panneau_y = 10;
    if (panneau_y + PANNEAU_H > fen_h) panneau_y = fen_h - PANNEAU_H - 10;

    SDL_Rect panneau = {panneau_x, panneau_y, PANNEAU_W, PANNEAU_H};
    int btn_start_x = panneau_x + (PANNEAU_W - (2 * BTN_W + GAP_BOUTONS)) / 2;
    int btn_y = panneau_y + PANNEAU_H - BTN_H - MARGE;
    
    SDL_Rect btn_L = {btn_start_x, btn_y, BTN_W, BTN_H};
    SDL_Rect btn_R = {btn_start_x + BTN_W + GAP_BOUTONS, btn_y, BTN_W, BTN_H};
    
    /* Bouton retour en haut à gauche */
    SDL_Rect btn_retour = {panneau_x + 8, panneau_y + 8, 25, 25};

    int running = 1;
    resultat_campement_t resultat = CAMPEMENT_QUITTE;
    etat_menu_t etat_actuel = MENU_ACCUEIL;

    /* Boucle principale*/
    while (running) {
        int souris_x, souris_y;
        SDL_GetMouseState(&souris_x, &souris_y);
        SDL_Point souris = {souris_x, souris_y};

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { running = 0; break; }
            if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = 0;
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                SDL_Point clic = {e.button.x, e.button.y};
                
                if (!SDL_PointInRect(&clic, &panneau)) {
                    /* Clic à l'extérieur */
                    if (etat_actuel == MENU_CHOIX_REPOS) etat_actuel = MENU_ACCUEIL;
                    else running = 0;
                } else {
                    /* Clic à l'intérieur du panneau */
                    if (etat_actuel == MENU_ACCUEIL) {
                        if (SDL_PointInRect(&clic, &btn_L)) etat_actuel = MENU_CHOIX_REPOS;
                        else if (SDL_PointInRect(&clic, &btn_R)) { resultat = CAMPEMENT_SOINS; running = 0; }
                    } else {
                        if (SDL_PointInRect(&clic, &btn_retour)) {
                            etat_actuel = MENU_ACCUEIL; /* Flèche retour cliquée */
                        }
                        else if (SDL_PointInRect(&clic, &btn_L)) { resultat = CAMPEMENT_REPOS_COMPLET; running = 0; }
                        else if (SDL_PointInRect(&clic, &btn_R)) { resultat = CAMPEMENT_REPOS_PARTIEL; running = 0; }
                    }
                }
            }
        }

        /* Rendu du Panneau */
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

        /* Textes et affichage */
        SDL_Color or_fonce = {OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255};
        SDL_Color or_clair = {OR_CLAIR_R, OR_CLAIR_G, OR_CLAIR_B, 255};
        int ty = panneau_y + MARGE + 5;

        if (etat_actuel == MENU_ACCUEIL) {
            /* Titre centré*/
            SDL_Rect r_titre;
            SDL_Texture *t_titre = creer_texte(renderer, font, "- Campement -", or_clair, &r_titre);
            if(t_titre) { 
                SDL_RenderCopy(renderer, t_titre, NULL, &(SDL_Rect){panneau_x + (PANNEAU_W - r_titre.w) / 2, ty, r_titre.w, r_titre.h}); 
                SDL_DestroyTexture(t_titre); 
                ty += r_titre.h + 8; 
            }

            /* Ligne séparatrice */
            SDL_SetRenderDrawColor(renderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 100);
            SDL_RenderDrawLine(renderer, panneau_x + MARGE, ty, panneau_x + PANNEAU_W - MARGE, ty);
            ty += 12;

            /* Texte : Description*/
            const char *txt[] = {"Vous pouvez vous", "reposer ici,", "ou bien acheter", "des fournitures."};
            for(int i=0; i<4; i++) {
                SDL_Rect r;
                SDL_Texture *t = creer_texte(renderer, font, txt[i], or_fonce, &r);
                if(t) { 
                    SDL_RenderCopy(renderer, t, NULL, &(SDL_Rect){panneau_x + MARGE + 5, ty, r.w, r.h}); 
                    SDL_DestroyTexture(t); 
                    ty += r.h + 6; 
                }
            }

            dessiner_bouton_campement(renderer, font, btn_L, "Repos", SDL_PointInRect(&souris, &btn_L));
            dessiner_bouton_campement(renderer, font, btn_R, "Boutique", SDL_PointInRect(&souris, &btn_R));
        } 
        else {
            /* Bouton de retour en haut à gauche */
            dessiner_bouton_campement(renderer, font, btn_retour, "<", SDL_PointInRect(&souris, &btn_retour));

            /* Titre centré  */
            SDL_Rect r_titre;
            SDL_Texture *t_titre = creer_texte(renderer, font, "- Repos -", or_clair, &r_titre);
            if(t_titre) { 
                SDL_RenderCopy(renderer, t_titre, NULL, &(SDL_Rect){panneau_x + (PANNEAU_W - r_titre.w) / 2, ty, r_titre.w, r_titre.h}); 
                SDL_DestroyTexture(t_titre); 
                ty += r_titre.h + 8; 
            }

            /* Ligne séparatrice */
            SDL_SetRenderDrawColor(renderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 100);
            SDL_RenderDrawLine(renderer, panneau_x + MARGE, ty, panneau_x + PANNEAU_W - MARGE, ty);
            ty += 15;

            /* Texte : Description */
            const char *txt[] = {"Complet : 15 po", "(Restaure 100% PV)", "", "Partiel : 7 po", "(Restaure 50% PV)"};
            for(int i=0; i<5; i++) {
                SDL_Rect r;
                SDL_Texture *t = creer_texte(renderer, font, txt[i], or_fonce, &r);
                if(t) { 
                    SDL_RenderCopy(renderer, t, NULL, &(SDL_Rect){panneau_x + MARGE + 5, ty, r.w, r.h}); 
                    SDL_DestroyTexture(t); 
                    ty += r.h + 4; 
                }
            }

            dessiner_bouton_campement(renderer, font, btn_L, "Complet", SDL_PointInRect(&souris, &btn_L));
            dessiner_bouton_campement(renderer, font, btn_R, "Partiel", SDL_PointInRect(&souris, &btn_R));
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    return resultat;
}
