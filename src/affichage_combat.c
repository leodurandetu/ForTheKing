#include "../lib/affichage_combat.h"

/* Leo
 * Cette fonction dessine une icône,
 * avec un effet de survol si on place
 * la souris par-dessus
 */
static void dessiner_icone(combat_t *combat, SDL_Rect *r_icone, int survole, SDL_Texture *texture_arme, SDL_Texture *texture_texte, SDL_Rect *rect_texte)
{
    // transparence
    SDL_SetRenderDrawBlendMode(combat->renderer, SDL_BLENDMODE_BLEND);

    if (survole) {
        // Survolé -> fond un peu plus clair et bordure Jaune/Or
        SDL_SetRenderDrawColor(combat->renderer, 255, 255, 255, 60); 
        SDL_RenderFillRect(combat->renderer, r_icone);
        SDL_SetRenderDrawColor(combat->renderer, 255, 215, 0, 255); 
    } else {
        // fond très sombre transparent et bordure grise
        SDL_SetRenderDrawColor(combat->renderer, 0, 0, 0, 100); 
        SDL_RenderFillRect(combat->renderer, r_icone);
        SDL_SetRenderDrawColor(combat->renderer, 150, 150, 150, 255); 
    }

    SDL_RenderDrawRect(combat->renderer, r_icone);

    if (texture_arme) {
        SDL_RenderCopy(combat->renderer, texture_arme, NULL, r_icone);
    }

    if (texture_texte && rect_texte) {
        SDL_Rect rect = *rect_texte;

        // Centrer le texte horizontalement par rapport à l'icône
        rect.x = r_icone->x + (r_icone->w - rect.w) / 2;
        rect.y = r_icone->y + r_icone->h + 5; // un peu en dessous de l'icône

        SDL_RenderCopy(combat->renderer, texture_texte, NULL, &rect);
    }

}

/* Leo
 * Cette fonction dessine le fond d'écran
 * qui prend toute la taille de l'écran, mais
 * en faisant en sorte que l'image ne soit
 * pas étendue ou étirée bizarrement.
 */
static void dessiner_fond_ecran(combat_t *combat, int largeur, int hauteur)
{
    if (!combat->texture_fond_ecran) return;

    int largeur_texture, hauteur_texture;
    SDL_QueryTexture(combat->texture_fond_ecran, NULL, NULL, &largeur_texture, &hauteur_texture);

    float echelle = fmax( (float) largeur / largeur_texture, (float) hauteur / hauteur_texture);

    SDL_Rect rect;

    rect.w = largeur_texture * echelle;
    rect.h = hauteur_texture * echelle;
    rect.x = (largeur - rect.w) / 2;
    rect.y = (hauteur - rect.h) / 2;

    SDL_RenderCopy(combat->renderer, combat->texture_fond_ecran, NULL, &rect);
}

/* Leo
 * Cette fonction dessine un menu de statistiques sur le
 * côté à propos d'un personnage ou un monstre
 */
static void dessiner_interface_combat(SDL_Renderer* r, TTF_Font* f, SDL_Texture* portrait, int a_gauche,
    const char* nom, int pv, int pv_max, int force, int intel, int rapidite, int evasion)
{
    int fen_w, fen_h;
    SDL_GetRendererOutputSize(r, &fen_w, &fen_h);

    /* Calcul de la taille du menu */
    int l_menu = fen_w * 0.22;
    int h_min_requise = 360; // Le menu ne fera JAMAIS moins de 360px de haut
    int h_menu = fen_h * 0.6;
    if (h_menu < h_min_requise) h_menu = h_min_requise;

    /* Empêcher le menu d'être plus grand que la fenêtre elle-même */
    if (h_menu > fen_h - 40) h_menu = fen_h - 40;

    int x_menu = a_gauche ? 20 : (fen_w - l_menu - 20);
    int y_menu = (fen_h - h_menu) / 2;

    SDL_Rect fond = { x_menu, y_menu, l_menu, h_menu };

    /* Dessin du cadre */
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 20, 20, 25, 230);
    SDL_RenderFillRect(r, &fond);
    SDL_SetRenderDrawColor(r, 100, 100, 110, 255);
    SDL_RenderDrawRect(r, &fond);

    /* Éléments positionnés proportionnellement à h_menu */

    /* Nom (environ 5% du haut) */
    SDL_Rect r_nom;
    SDL_Texture* t_nom = creer_texte(r, f, nom, (SDL_Color){255, 215, 0, 255}, &r_nom);
    if (t_nom) {
        SDL_Rect pos = { fond.x + (fond.w - r_nom.w) / 2, fond.y + (h_menu * 0.04), r_nom.w, r_nom.h };
        SDL_RenderCopy(r, t_nom, NULL, &pos);
        SDL_DestroyTexture(t_nom);
    }

    /* Portrait (Taille adaptée à la hauteur du menu) */
    int taille_p = h_menu * 0.25; // Le portrait prend 25% de la hauteur du menu
    if (taille_p > 100) taille_p = 100; // On le plafonne pour ne pas qu'il soit géant

    SDL_Rect r_portrait = { fond.x + (fond.w - taille_p) / 2, fond.y + (h_menu * 0.15), taille_p, taille_p };
    SDL_SetRenderDrawColor(r, 60, 60, 65, 255);
    SDL_RenderFillRect(r, &r_portrait);
    if (portrait) SDL_RenderCopy(r, portrait, NULL, &r_portrait);

    /* Barre de Vie */
    int h_barre = 22;
    SDL_Rect b_vie = { fond.x + 20, r_portrait.y + r_portrait.h + (h_menu * 0.08), fond.w - 40, h_barre };
    dessiner_barre(r, f, "PV", pv, pv_max, b_vie, (SDL_Color){200, 40, 40, 255});

    /* On affiche un texte pour chaque statistique avec sa valeur */
    const char* labels[] = {"Force", "Intelligence", "Rapidite", "Evasion"};
    int valeurs[] = {force, intel, rapidite, evasion};
    SDL_Color coul_texte = {220, 220, 220, 255};

    int y_stats = b_vie.y + b_vie.h + (h_menu * 0.08);
    int espacement = (h_menu * 0.09); // Espacement relatif

    for (int i = 0; i < 4; i++) {
        char buf[64];
        sprintf(buf, "%s : %d", labels[i], valeurs[i]);
        SDL_Rect r_s;
        SDL_Texture* t_s = creer_texte(r, f, buf, coul_texte, &r_s);
        if (t_s) {
            SDL_Rect pos = { fond.x + 25, y_stats + (i * espacement), r_s.w, r_s.h };
            SDL_RenderCopy(r, t_s, NULL, &pos);
            SDL_DestroyTexture(t_s);
        }
    }
}

/* Leo
 * Cette fonction dessine et met à jour
 * l'affichage de la fenêtre de combat
 */
void maj_affichage_fenetre_combat(combat_t *combat)
{
    SDL_Renderer *renderer = combat->renderer;

    int largeur, hauteur;
    SDL_GetRendererOutputSize(renderer, &largeur, &hauteur);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    dessiner_fond_ecran(combat, largeur, hauteur);

    int taille = largeur * 0.18f;
    int marge  = largeur * 0.03f;

   int w_fond, h_fond;
    // On récupère les dimensions originales de l'image de fond
    if (combat->texture_fond_ecran) {
        SDL_QueryTexture(combat->texture_fond_ecran, NULL, NULL, &w_fond, &h_fond);
    } else {
        w_fond = largeur; h_fond = hauteur;
    }

    float echelle = fmax( (float) largeur / w_fond, (float) hauteur / h_fond);
    float hauteur_fond_affichee = h_fond * echelle;
    float y_image_fond = (hauteur - hauteur_fond_affichee) / 2.0f;
    float y_sol = y_image_fond + (hauteur_fond_affichee * 0.78f);
    // On place les personnages pour qu'ils touchent ce sol
    int y_personnage = (int)y_sol - taille;

    SDL_Rect rect_perso = {
        marge,
        y_personnage,
        taille,
        taille
    };

    SDL_Rect rect_monstre = {
        largeur - marge - taille,
        y_personnage,
        taille,
        taille
    };

    /* fond blanc sous les personnages */
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //SDL_RenderFillRect(renderer, &perso);
    //SDL_RenderFillRect(renderer, &monstre);

    if (combat->texture_perso)
        SDL_RenderCopy(renderer, combat->texture_perso, NULL, &rect_perso);

    if (combat->texture_monstre)
        SDL_RenderCopy(renderer, combat->texture_monstre, NULL, &rect_monstre);

    /* boutons */

    // On fait des icônes carrées, plus petites
    int taille_icone = largeur * 0.08f; 

    // Placement en hauteur : un peu au-dessus du sol, entre les personnages
    int y_icones = y_personnage + (taille * 0.2f);

    /* 35% de la largeur à gauche / à droite */
    int marge_bord = largeur * 0.35f;

    combat->bouton_leger = (SDL_Rect){
        marge_bord,
        y_icones,
        taille_icone, 
        taille_icone
    };

    combat->bouton_lourd = (SDL_Rect){
        largeur - marge_bord - taille_icone, 
        y_icones,
        taille_icone, 
        taille_icone
    };

    dessiner_icone(combat, &combat->bouton_leger, combat->survole_bouton_leger, combat->texture_attaque_legere, combat->texture_texte_leger, &combat->rect_texte_leger);
    dessiner_icone(combat, &combat->bouton_lourd, combat->survole_bouton_lourd, combat->texture_attaque_lourde, combat->texture_texte_lourd, &combat->rect_texte_lourd);

    /* Dessin des menus de statistiques à gauche (Joueur) et à droite (Monstre) */
    perso_t * perso = combat->perso;
    monstre_t * monstre = combat->monstre;

    dessiner_interface_combat(renderer, combat->font, combat->texture_perso, 1, "Mage Joueur", perso->sante, perso->sante_max, perso->force, perso->intelligence, perso->rapidite, perso->evasion);
    if(combat->monstre->type == SQUELETTE){
        dessiner_interface_combat(renderer, combat->font, combat->texture_monstre, 0, "Squelette", monstre->sante, monstre->sante_max, monstre->force, monstre->intelligence, monstre->rapidite, monstre->evasion);
    } else if (combat->monstre->type == TROLL){
        dessiner_interface_combat(renderer, combat->font, combat->texture_monstre, 0, "Troll", monstre->sante, monstre->sante_max, monstre->force, monstre->intelligence, monstre->rapidite, monstre->evasion);
    }

    SDL_RenderPresent(renderer);
}