#include "../lib/affichage_commun.h"

/* Leo
 * Cette fonction créer une texture pour
 * un texte et la renvoie en retour de fonction
 */
SDL_Texture* creer_texte(SDL_Renderer* renderer, TTF_Font* police, const char* texte, SDL_Color couleur, SDL_Rect* rectangle_sortie) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(police, texte, couleur);
    if (!surface) return NULL;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (rectangle_sortie) {
        rectangle_sortie->w = surface->w;
        rectangle_sortie->h = surface->h;
    }
    SDL_FreeSurface(surface);
    return texture;
}

/* Calcule la position pixel (cx, cy) du centre d'une case hexagonale
 * à partir de ses coordonnées carte, de la taille de case et du joueur */
void case_vers_pixels(int cx_carte, int cy_carte,
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

/* Leo
 * Cette fonction dessine une barre de statistiques,
 * pour l'instant seulement les points de vie (la santé)
 */
void dessiner_barre(SDL_Renderer* r, TTF_Font* f, const char* label, int val, int max, SDL_Rect zone, SDL_Color coul_barre) {
    SDL_Color blanc = {255, 255, 255, 255};
    char buffer[32];
    SDL_Rect r_texte;

    /* Fond de la barre (gris foncé) */
    SDL_SetRenderDrawColor(r, 40, 40, 40, 200);
    SDL_RenderFillRect(r, &zone);

    /* Remplissage proportionnel */
    float ratio = (max > 0) ? (float)val / max : 0;
    SDL_Rect remplissage = { zone.x + 2, zone.y + 2, (int)((zone.w - 4) * ratio), zone.h - 4 };
    SDL_SetRenderDrawColor(r, coul_barre.r, coul_barre.g, coul_barre.b, 255);
    SDL_RenderFillRect(r, &remplissage);

    /* Texte "Valeur / Max" */
    sprintf(buffer, "%d / %d", val, max);
    SDL_Texture* t_points = creer_texte(r, f, buffer, blanc, &r_texte);
    if (t_points) {
        SDL_Rect pos = { zone.x + zone.w - r_texte.w - 5, zone.y + (zone.h - r_texte.h)/2, r_texte.w, r_texte.h };
        SDL_RenderCopy(r, t_points, NULL, &pos);
        SDL_DestroyTexture(t_points);
    }
}

/* Leo
 * Cette fonction affiche un message
 * centré sur une fenêtre
 */
void afficherMessageCentre(SDL_Renderer *renderer, TTF_Font *font, const char *message, 
    int fenetre_w, int fenetre_h, SDL_Texture **texture, char *ancienMessage)
{
    SDL_Color color = {255, 255, 255, 255};

    // On recrée la texture seulement si le message change
    if (*texture == NULL || strcmp(ancienMessage, message) != 0)
    {
        // Supprimer ancienne texture
        if (*texture != NULL) {
            SDL_DestroyTexture(*texture);
            *texture = NULL;
        }

        SDL_Surface *surface = TTF_RenderText_Solid(font, message, color);
        if (!surface) {
            printf("Erreur surface: %s\n", TTF_GetError());
            return;
        }

        *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!(*texture)) {
            printf("Erreur texture: %s\n", SDL_GetError());
            SDL_FreeSurface(surface);
            return;
        }

        strcpy(ancienMessage, message);

        SDL_FreeSurface(surface);
    }

    int textW, textH;
    SDL_QueryTexture(*texture, NULL, NULL, &textW, &textH);

    SDL_Rect dest;
    dest.x = (fenetre_w - textW) / 2;
    dest.y = (fenetre_h - textH) / 2;
    dest.w = textW;
    dest.h = textH;

    SDL_RenderCopy(renderer, *texture, NULL, &dest);
}

void dessiner_inventaire(SDL_Renderer * renderer, TTF_Font * font, perso_t * perso, SDL_Rect fond_interface,
    SDL_Rect rect_nom, int clic_gauche, int * maj_affichage, orientation_inv_t orientation, int start_droite_x) {
    
    if (!perso) return;

    inventaire_t* inventaire = &(perso->inventaire);

    int souris_x, souris_y;
    SDL_GetMouseState(&souris_x, &souris_y);

    int espacement = 3;
    int nb_cases_par_ligne = (orientation == INV_HORIZONTAL) ? 5 : 1;
    int taille_case;
    if (orientation == INV_VERTICAL) {
        // Taille proportionnelle à la hauteur du menu
        taille_case = fond_interface.h / 12;
        if (taille_case > 48) taille_case = 48; // Maximum
        if (taille_case < 24) taille_case = 24; // Minimum
    } else {
        taille_case = 28;
    }

    int start_x, start_y;

    if (orientation == INV_HORIZONTAL) {
        // Horizontal (comme dans la carte) : sous le nom
        start_x = start_droite_x;
        start_y = fond_interface.y + 15 + rect_nom.h + 5;
    } else {
        // Vertical (comme en combat) : à gauche ou sous le portrait
        start_x = fond_interface.x + fond_interface.w + 10;

        // Nombre de lignes dans l'inventaire vertical
        int nb_lignes = (TAILLE_INVENTAIRE + nb_cases_par_ligne - 1) / nb_cases_par_ligne;

        // Hauteur totale de l'inventaire
        int hauteur_totale = nb_lignes * (taille_case + espacement) - espacement;

        // Centrage vertical par rapport au menu
        start_y = fond_interface.y + (fond_interface.h - hauteur_totale) / 2;
    }
    
    for (int i = 0; i < TAILLE_INVENTAIRE; i++) {
        int ligne = i / nb_cases_par_ligne;
        int colonne = i % nb_cases_par_ligne;

        SDL_Rect case_inv = {
            start_x + colonne * (taille_case + espacement),
            start_y + ligne * (taille_case + espacement),
            taille_case,
            taille_case
        };

        int souris_par_dessus = (souris_x >= case_inv.x && souris_x <= case_inv.x + case_inv.w)
            && (souris_y >= case_inv.y && souris_y <= case_inv.y + case_inv.h);

        if (souris_par_dessus) {
            SDL_SetRenderDrawColor(renderer, 55, 55, 65, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 35, 35, 40, 255);
        }

        SDL_RenderFillRect(renderer, &case_inv);

        if (souris_par_dessus) {
            SDL_SetRenderDrawColor(renderer, 120, 120, 140, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 80, 80, 90, 255);
        }

        SDL_RenderDrawRect(renderer, &case_inv);

        if (souris_par_dessus && clic_gauche) {
            utiliser_objet_inventaire(inventaire, i, perso, maj_affichage);
        }

        objet_t * obj = &(inventaire->contenu[i]);

        if (obj->quantite > 0 && obj->texture != NULL) {
            SDL_Rect rect_icone = {
                case_inv.x + 2,
                case_inv.y + 2,
                case_inv.w - 4,
                case_inv.h - 4
            };

            SDL_RenderCopy(renderer, obj->texture, NULL, &rect_icone);

            /* Afficher la quantité */
            char texte_quantite_str[8];
            sprintf(texte_quantite_str, "%d", obj->quantite);

            SDL_Rect rect_texte;
            SDL_Texture * texte_quantite = creer_texte(renderer, font, texte_quantite_str, (SDL_Color) {255, 255, 255, 255}, &rect_texte);

            if (texte_quantite) {
                SDL_Rect rect_quantite = {
                    case_inv.x + case_inv.w - rect_texte.w - 2,
                    case_inv.y + case_inv.h - rect_texte.h - 2,
                    rect_texte.w,
                    rect_texte.h
                };

                SDL_RenderCopy(renderer, texte_quantite, NULL, &rect_quantite);
                SDL_DestroyTexture(texte_quantite);
            }

        }

    }

}