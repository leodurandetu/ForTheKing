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