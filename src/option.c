#include "../lib/option.h"
#include "../lib/affichage_commun.h"
#include <SDL2/SDL_image.h>

/* Couleurs du thème */
#define COULEUR_FOND_R 25
#define COULEUR_FOND_G 25
#define COULEUR_FOND_B 30
#define COULEUR_FOND_A 240

/* Doré foncé */
#define OR_FONCE_R 160
#define OR_FONCE_G 120
#define OR_FONCE_B 10

/* Doré plus clair pour les survols et reliefs */
#define OR_CLAIR_R 210
#define OR_CLAIR_G 170
#define OR_CLAIR_B 40

/*
 * Dessine un bouton avec un effet de relief 3D.
 * Si "bloque" est à 1, le bouton sera grisé.
 */
static void dessiner_bouton_option(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect rect, const char *label, int survole, int bloque) {
    
    /* Fond du bouton */
    if (bloque) {
        SDL_SetRenderDrawColor(renderer, 25, 20, 15, 255); 
    } else if (survole) {
        SDL_SetRenderDrawColor(renderer, 65, 60, 25, 255); 
    } else {
        SDL_SetRenderDrawColor(renderer, 35, 30, 15, 255);
    }
    SDL_RenderFillRect(renderer, &rect);

    /* Effet 3D de bordure */
    if (bloque) {
        SDL_SetRenderDrawColor(renderer, 60, 50, 40, 255);
    } else if (survole) {
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

    /* Couleur du texte */
    SDL_Color couleur_texte;
    if (bloque) {
        couleur_texte = (SDL_Color){100, 100, 100, 255}; 
    } else if (survole) {
        couleur_texte = (SDL_Color){OR_CLAIR_R, OR_CLAIR_G, OR_CLAIR_B, 255};
    } else {
        couleur_texte = (SDL_Color){OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255};
    }

    /* Rendu du texte centré */
    SDL_Rect r_txt;
    SDL_Texture *t = creer_texte(renderer, font, label, couleur_texte, &r_txt);
    if (t) {
        SDL_Rect pos = { rect.x + (rect.w - r_txt.w) / 2, rect.y + (rect.h - r_txt.h) / 2, r_txt.w, r_txt.h };
        SDL_RenderCopy(renderer, t, NULL, &pos);
        SDL_DestroyTexture(t);
    }
}

/*
 * Charge l'image du monstre depuis le disque 
 */
SDL_Texture* charger_texture_monstre(SDL_Renderer *renderer, const char *chemin) {
    SDL_Surface *img = IMG_Load(chemin);
    if (!img) return NULL;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
    return texture;
}


int afficher_option(SDL_Renderer* gRenderer, TTF_Font *gFont,
                    int perso_x, int perso_y, int perso_w, int perso_h,
                    int fuite_bloquee,const char *chemin_monstre,
                    int monstre_x, int monstre_y, int tailleCase) {
    int lancerCombat = 0;
    int running = 1;

    const int IMG_W = 60;
    const int IMG_H = 60;
    const int PANNEAU_W = 200;
    const int PANNEAU_H = 200;
    const int BTN_W = 160;
    const int BTN_H = 35;
    const int MARGE = 15;
    const int GAP = 10;

    float cx_monstre, cy_monstre;
    case_vers_pixels(monstre_x, monstre_y, perso_x, perso_y,
                    tailleCase, gRenderer, &cx_monstre, &cy_monstre);

    /* Récupération dynamique de la taille du renderer */
    int fen_w, fen_h;
    SDL_GetRendererOutputSize(gRenderer, &fen_w, &fen_h);

    /* Placement du menu à droite du sprite, avec repli à gauche */
    int rayon_hex = tailleCase / 2;
    int panneau_x = (int)(cx_monstre + rayon_hex + 8);

    if (panneau_x + PANNEAU_W > fen_w) {
        panneau_x = (int)(cx_monstre - rayon_hex - 8 - PANNEAU_W);
    }

    int panneau_y = (int)(cy_monstre - PANNEAU_H / 2);
    
    if (panneau_y < 0) panneau_y = 10;
    if (panneau_y + PANNEAU_H > fen_h) panneau_y = fen_h - PANNEAU_H - 10;

    SDL_Rect panneau = {panneau_x, panneau_y, PANNEAU_W, PANNEAU_H};

    /* Positionnement des éléments internes */
    int btn_x = panneau.x + (PANNEAU_W - BTN_W) / 2;
    
    int img_x = panneau.x + (PANNEAU_W - IMG_W) / 2;
    int img_y = panneau.y + MARGE;

    int btn_y_atk = img_y + IMG_H + MARGE;
    int btn_y_run = btn_y_atk + BTN_H + GAP;

    SDL_Rect bouton_attaque = {btn_x, btn_y_atk, BTN_W, BTN_H};
    SDL_Rect bouton_fuite = {btn_x, btn_y_run, BTN_W, BTN_H};
    SDL_Rect rect_monstre = {img_x, img_y, IMG_W, IMG_H};

    /* Chargement du monstre une seule fois avant la boucle */
    SDL_Texture *tex_monstre = charger_texture_monstre(gRenderer, chemin_monstre);

    SDL_Event e;

    while (running)
    {
        /* Récupération position souris pour les effets de survol */
        int souris_x, souris_y;
        SDL_GetMouseState(&souris_x, &souris_y);
        SDL_Point souris = {souris_x, souris_y};

        int survole_atk = SDL_PointInRect(&souris, &bouton_attaque);
        int survole_run = SDL_PointInRect(&souris, &bouton_fuite);

        /* === Événements === */
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) {
                running = 0;
                break;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                SDL_Point clic = {e.button.x, e.button.y};

                if (SDL_PointInRect(&clic, &bouton_attaque)) {
                    lancerCombat = 1;
                    running = 0;
                } else if (SDL_PointInRect(&clic, &bouton_fuite) && !fuite_bloquee) {
                    lancerCombat = 0;
                    running = 0;
                } 
            }
        }

        /* Affichage */

        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

        /* Ombre sous le panneau */
        SDL_Rect ombre = {panneau.x + 4, panneau.y + 4, panneau.w, panneau.h};
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 150);
        SDL_RenderFillRect(gRenderer, &ombre);

        /* Fond principal */
        SDL_SetRenderDrawColor(gRenderer, COULEUR_FOND_R, COULEUR_FOND_G, COULEUR_FOND_B, COULEUR_FOND_A);
        SDL_RenderFillRect(gRenderer, &panneau);

        /* Double Bordure */
        SDL_SetRenderDrawColor(gRenderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255);
        SDL_RenderDrawRect(gRenderer, &panneau);

        SDL_Rect panneau_int = {panneau.x + 3, panneau.y + 3, panneau.w - 6, panneau.h - 6};
        SDL_SetRenderDrawColor(gRenderer, 15, 15, 20, 255);
        SDL_RenderDrawRect(gRenderer, &panneau_int);

        /* Flèche pointant vers le monstre sur la carte */
        SDL_SetRenderDrawColor(gRenderer, OR_FONCE_R, OR_FONCE_G, OR_FONCE_B, 255);
        int arrow_y = panneau.y + PANNEAU_H / 2;

        if (panneau_x > (int)cx_monstre) {
            /* menu à droite : flèche à gauche */
            SDL_RenderDrawLine(gRenderer, panneau.x, arrow_y - 6, panneau.x - 7, arrow_y);
            SDL_RenderDrawLine(gRenderer, panneau.x - 7, arrow_y, panneau.x, arrow_y + 6);
        } else {
            /* menu à gauche : flèche à droite */
            SDL_RenderDrawLine(gRenderer, panneau.x + PANNEAU_W, arrow_y - 6, panneau.x + PANNEAU_W + 7, arrow_y);
            SDL_RenderDrawLine(gRenderer, panneau.x + PANNEAU_W + 7, arrow_y, panneau.x + PANNEAU_W, arrow_y + 6);
        }

        /* Sprite du monstre */
        if (tex_monstre) {
            SDL_RenderCopy(gRenderer, tex_monstre, NULL, &rect_monstre);
        }

        /* Dessin des boutons */
        dessiner_bouton_option(gRenderer, gFont, bouton_attaque, "Attaquer", survole_atk, 0);
        dessiner_bouton_option(gRenderer, gFont, bouton_fuite, "Fuir", survole_run, fuite_bloquee);

        SDL_RenderPresent(gRenderer);
        SDL_Delay(16);
    }

    /* Libération des ressources */
    if (tex_monstre) {
        SDL_DestroyTexture(tex_monstre);
    }

    return lancerCombat;
}
