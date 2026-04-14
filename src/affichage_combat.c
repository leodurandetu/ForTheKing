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
        SDL_SetRenderDrawColor(combat->renderer, 150, 150, 155, 180); 
        SDL_RenderFillRect(combat->renderer, r_icone);
        SDL_SetRenderDrawColor(combat->renderer, 255, 215, 0, 255); 
    } else {
        // fond très sombre transparent et bordure grise
        SDL_SetRenderDrawColor(combat->renderer, 20, 20, 25, 180); 
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

    SDL_SetRenderDrawColor(combat->renderer, 0, 0, 0, 80);
    SDL_Rect overlay = {0, 0, largeur, hauteur};
    SDL_RenderFillRect(combat->renderer, &overlay);
}

/* Leo
 * Cette fonction dessine un menu de statistiques sur le
 * côté à propos d'un personnage ou un monstre
 */
static void dessiner_interface_combat(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* portrait, int a_gauche,
    const char* nom, monstre_t * monstre, perso_t * perso, int clic_gauche, int * maj_affichage)
{
    int fen_w, fen_h;
    SDL_GetRendererOutputSize(renderer, &fen_w, &fen_h);

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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 20, 20, 25, 230);
    SDL_RenderFillRect(renderer, &fond);
    SDL_SetRenderDrawColor(renderer, 100, 100, 110, 255);
    SDL_RenderDrawRect(renderer, &fond);

    /* Éléments positionnés proportionnellement à h_menu */

    /* Nom (environ 5% du haut) */
    SDL_Rect r_nom;
    SDL_Texture* t_nom = creer_texte(renderer, font, nom, (SDL_Color){255, 215, 0, 255}, &r_nom);
    if (t_nom) {
        SDL_Rect pos = { fond.x + (fond.w - r_nom.w) / 2, fond.y + (h_menu * 0.04), r_nom.w, r_nom.h };
        SDL_RenderCopy(renderer, t_nom, NULL, &pos);
        SDL_DestroyTexture(t_nom);
    }

    if (perso != NULL) {
        dessiner_inventaire(renderer, font, perso, fond, r_nom, clic_gauche, maj_affichage, INV_VERTICAL, -1);
    }

    /* on récupère les statistiques après avoir affiché l'inventaire, 
     * car le personnage notamment peut utiliser des objets dans
     * l'inventaire, et on vérifie ça dans dessiner_inventaire
     * (les objets peuvent augmenter les statistiques).
    */
    int pv = 0;
    int pv_max = 0;
    int force = 0;
    int intel = 0;
    int rapidite = 0;
    int evasion = 0;

    if (monstre != NULL) {
        pv = monstre->sante;
        pv_max = monstre->sante_max;
        force = monstre->force;
        intel = monstre->intelligence;
        rapidite = monstre->rapidite;
        evasion = monstre->evasion;
    } else if (perso != NULL) {
        pv = perso->sante;
        pv_max = perso->sante_max;
        force = perso->force;
        intel = perso->intelligence;
        rapidite = perso->rapidite;
        evasion = perso->evasion;
    }

    /* Portrait (Taille adaptée à la hauteur du menu) */
    int taille_p = h_menu * 0.25; // Le portrait prend 25% de la hauteur du menu
    if (taille_p > 100) taille_p = 100; // On le plafonne pour ne pas qu'il soit géant

    SDL_Rect r_portrait = { fond.x + (fond.w - taille_p) / 2, fond.y + (h_menu * 0.15), taille_p, taille_p };
    SDL_SetRenderDrawColor(renderer, 60, 60, 65, 255);
    SDL_RenderFillRect(renderer, &r_portrait);
    if (portrait) SDL_RenderCopy(renderer, portrait, NULL, &r_portrait);

    /* Barre de Vie */
    int h_barre = 22;
    SDL_Rect b_vie = { fond.x + 20, r_portrait.y + r_portrait.h + (h_menu * 0.08), fond.w - 40, h_barre };
    dessiner_barre(renderer, font, "PV", pv, pv_max, b_vie, (SDL_Color){200, 40, 40, 255});

    /* On affiche un texte pour chaque statistique avec sa valeur */
    const char* labels[] = {"Force", "Intelligence", "Rapidite", "Evasion"};
    int valeurs[] = {force, intel, rapidite, evasion};
    SDL_Color coul_texte = {220, 220, 220, 255};

    int y_stats = b_vie.y + b_vie.h + (h_menu * 0.08);
    int espacement = (h_menu * 0.09);

    for (int i = 0; i < 4; i++) {
        char texte[64];
        sprintf(texte, "%s : %d", labels[i], valeurs[i]);
        SDL_Rect rect_texte;
        SDL_Texture* texture_texte = creer_texte(renderer, font, texte, coul_texte, &rect_texte);
        if (texture_texte) {
            SDL_Rect pos = { fond.x + 25, y_stats + (i * espacement), rect_texte.w, rect_texte.h };
            SDL_RenderCopy(renderer, texture_texte, NULL, &pos);
            SDL_DestroyTexture(texture_texte);
        }
    }

}

/* Leo
 * Cette fonction vérifie si il y a
 * un message pendant le combat à
 * afficher et si oui, on l'affiche.
 */
static void gerer_affichage_message(combat_t *combat) {
    // Si le temps actuel est inférieur à l'expiration, on affiche
    if (SDL_GetTicks() < combat->message_expiration) {
        SDL_Renderer *r = combat->renderer;
        int w, h;
        SDL_GetRendererOutputSize(r, &w, &h);

        // Dimensions de la boîte de message
        int box_w = 400;
        int box_h = 120;
        SDL_Rect fond = { (w - box_w)/2, (h - box_h)/2, box_w, box_h };

        // Fond sombre transparent 
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 25, 30, 25, 245); 
        SDL_RenderFillRect(r, &fond);
        
        // Bordure fine 
        SDL_SetRenderDrawColor(r, 140, 150, 120, 255);
        SDL_RenderDrawRect(r, &fond);
        
        // On double la bordure vers l'intérieur pour qu'elle ressorte un peu mieux
        SDL_Rect fond_interieur = { fond.x + 1, fond.y + 1, fond.w - 2, fond.h - 2 };
        SDL_RenderDrawRect(r, &fond_interieur);

        // Rendu du texte
        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Surface *surf = TTF_RenderUTF8_Blended(combat->font, combat->message_combat, blanc);

        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(r, surf);
            // On centre le texte au milieu de notre nouvelle boîte
            SDL_Rect pos = { (w - surf->w)/2, (h - surf->h)/2, surf->w, surf->h };
            SDL_RenderCopy(r, tex, NULL, &pos);
            SDL_DestroyTexture(tex);
            SDL_FreeSurface(surf);
        }
        
    }

}

/* Leo
 * Cette fonction dessine et met à jour
 * l'affichage de la fenêtre de combat
 */
void maj_affichage_fenetre_combat(combat_t *combat, int clicGauche, int * maj_affichage)
{
    SDL_Renderer *renderer = combat->renderer;

    int largeur, hauteur;
    SDL_GetRendererOutputSize(renderer, &largeur, &hauteur);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    dessiner_fond_ecran(combat, largeur, hauteur);

    int taille = largeur * 0.18f;

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
    int y_personnage = (int)y_sol - taille;

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

    const char *noms_classes[] = {"Brute", "Chasseur", "Assassin", "Mage"};
    const char *nom_perso = noms_classes[perso->type];

    dessiner_interface_combat(renderer, combat->font, combat->texture_perso, 1, nom_perso, NULL, perso, clicGauche, maj_affichage);
    
    int nul = 0;

    char nom_monstre_formate[64];

    if(combat->monstre->type == SQUELETTE) {
        sprintf(nom_monstre_formate, "Squelette (Niv. %d)",monstre->niveau);
    } else if (combat->monstre->type == TROLL) {
        sprintf(nom_monstre_formate, "Troll (Niv. %d)",monstre->niveau);
    } else if (combat->monstre->type == YETI) {
        sprintf(nom_monstre_formate, "Yeti (Niv. %d)",monstre->niveau);
    } else if (combat->monstre->type == VER_GEANT) {
        sprintf(nom_monstre_formate, "Ver Geant (Niv. %d)",monstre->niveau);
    } else if (combat->monstre->type == BOSS_FINAL) {
        sprintf(nom_monstre_formate, "Boss Final (Niv. %d)",monstre->niveau);
    }

    /* Appel unique à la fonction de dessin avec le nom complet */
    dessiner_interface_combat(renderer, combat->font, combat->texture_monstre, 0, nom_monstre_formate, monstre, NULL, 0, &nul);

    gerer_affichage_message(combat);

    SDL_RenderPresent(renderer);
}

void afficher_message_combat(combat_t *combat, const char *texte) {

    if (texte != NULL) {
        // On copie le texte dans l'attribut de la structure combat
        strncpy(combat->message_combat, texte, sizeof(combat->message_combat) - 1);
        
        /* On définit quand le message doit expirer :
        * Temps actuel + 1000 millisecondes
        * (c'est-à-dire le message dure 1 seconde)
        */
        combat->message_expiration = SDL_GetTicks() + 1000;
    }

}

void demander_et_lancer_combat(SDL_Renderer * renderer, TTF_Font * police2, perso_t * perso,
    monstre_t * monstre, int tailleCase, int * nbFuites, int * vies_globales, etat_jeu_t * etat,
    case_t ** carte, int carte_x, int carte_y, combat_t ** combat_actuel, SDL_Window * pFenetre,
    SDL_Texture * portraitPerso) {
    *combat_actuel = creer_combat(perso, monstre);

    // Taille du sprite à l'écran (même logique que dans afficher_personnage)
    int sprite_w = tailleCase;
    int sprite_h = tailleCase;
    int choix;

    char * cheminTexture;

    switch (monstre->type){
        case SQUELETTE :
            cheminTexture = "img/squelette.png";
            break;

        case TROLL:
            cheminTexture = "img/troll.png";
            break;

        case YETI:
            cheminTexture = "img/yeti.png";
            break;

        case VER_GEANT:
            cheminTexture = "img/ver_geant.png";
            break;

        case BOSS_FINAL:
            cheminTexture = "img/boss_final.png";
            break;
        
        default:
            return;
        
    }

    choix = afficher_option(renderer, police2, perso->x, perso->y, sprite_w, sprite_h,
        (*nbFuites >= MAX_FUITE), cheminTexture, carte_x, carte_y, tailleCase);
    if(choix == 1)
    {
        ouvrir_fenetre_combat(pFenetre, renderer, carte[carte_y][carte_x].biome, combat_actuel, carte, vies_globales, portraitPerso);
        *etat = COMBAT;
        (*nbFuites) = 0;
    } else if (choix == 0) {
        detruire_combat(combat_actuel);
        *etat = CARTE;
        (*nbFuites)++;
    }

}
