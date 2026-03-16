#include "../lib/combat.h"

/* -- Fonctions Utilitaires -- */

/* Leo
 * Cette fonction créer une texture pour
 * un texte et la renvoie en retour de fonction
 */
static SDL_Texture* creer_texte(SDL_Renderer* renderer, TTF_Font* police, const char* texte, SDL_Color couleur, SDL_Rect* rectangle_sortie) {
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
 * Cette fonction dessine une icône,
 * avec un effet de survol si on place
 * la souris par-dessus
 */
extern void dessiner_icone(combat_t *combat, SDL_Rect *r_icone, int survole, SDL_Texture *texture_arme, SDL_Texture *texture_texte, SDL_Rect *rect_texte)
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
 * Cette fonction dessine une barre de statistiques,
 * pour l'instant seulement les points de vie (la santé)
 */
static void dessiner_barre(SDL_Renderer* r, TTF_Font* f, const char* label, int val, int max, SDL_Rect zone, SDL_Color coul_barre) {
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
 * Cette fonction permet d'ouvrir
 * une fenêtre de combat, et donc
 * également de commencer un combat
 * en parallèle.
 */
void ouvrir_fenetre_combat(combat_t ** combat) {
    (*combat)->pFenetre = (SDL_CreateWindow("For The King - Combat", 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));

    if (!((*combat)->pFenetre)) {
        fprintf(stderr, "Erreur fenêtre : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Création du moteur de rendu (accéléré par la carte graphique)
    (*combat)->renderer = (SDL_CreateRenderer((*combat)->pFenetre, -1, SDL_RENDERER_ACCELERATED));
    if (!((*combat)->renderer)) {
        fprintf(stderr, "Erreur renderer : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Chargement du sprite du personnage
    (*combat)->texture_perso = NULL;
    SDL_Surface *img_perso = IMG_Load("img/mage.png");
    if (img_perso) {
        // Rend la couleur blanche (255, 255, 255) transparente
        SDL_SetColorKey(img_perso, SDL_TRUE, SDL_MapRGB(img_perso->format, 255, 255, 255));
        (*combat)->texture_perso = (SDL_CreateTextureFromSurface((*combat)->renderer, img_perso));
        SDL_SetTextureBlendMode((*combat)->texture_perso, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_perso);
    }

    // Chargement du sprite du monstre (unique pour l'instant)
    (*combat)->texture_monstre = NULL;
    SDL_Surface *img_monstre = IMG_Load("img/squelette.png");
    if (img_monstre) {
        (*combat)->texture_monstre = (SDL_CreateTextureFromSurface((*combat)->renderer, img_monstre));
        SDL_SetTextureBlendMode((*combat)->texture_monstre, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_monstre);
    }
    
    // Chargement du sprite du fond d'écran
    (*combat)->texture_fond_ecran = NULL;
    SDL_Surface *img_fond_foret = IMG_Load("img/fond_foret.png");
    if (img_fond_foret) {
        (*combat)->texture_fond_ecran = (SDL_CreateTextureFromSurface((*combat)->renderer, img_fond_foret));
        SDL_SetTextureBlendMode((*combat)->texture_fond_ecran, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_fond_foret);
    }

    // Chargement des symboles de l'attaque legère
    (*combat)->texture_attaque_legere = NULL;
    SDL_Surface *img_legere = IMG_Load("img/attaque_l.png"); 
    if (img_legere) {
        (*combat)->texture_attaque_legere = SDL_CreateTextureFromSurface((*combat)->renderer, img_legere);
        SDL_SetTextureBlendMode((*combat)->texture_attaque_legere, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_legere);
    }

    // Chargement des symboles de l'attaque lourde
    (*combat)->texture_attaque_lourde = NULL;
    SDL_Surface *img_lourde = IMG_Load("img/attaque_L.png"); 
    if (img_lourde) {
        (*combat)->texture_attaque_lourde = SDL_CreateTextureFromSurface((*combat)->renderer, img_lourde);
        SDL_SetTextureBlendMode((*combat)->texture_attaque_lourde, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_lourde);
    }

    TTF_Init();
    (*combat)->font = TTF_OpenFont("Fonts/Enchanted Land.otf", 24);

    SDL_Color blanc = {255, 255, 255, 255}; // blanc

    // Texte bouton léger
    SDL_Surface *surf_leger = TTF_RenderUTF8_Blended((*combat)->font, "Attaque Légère", blanc);

    if (surf_leger) {
        (*combat)->texture_texte_leger = SDL_CreateTextureFromSurface((*combat)->renderer, surf_leger);
        (*combat)->rect_texte_leger.w = surf_leger->w;
        (*combat)->rect_texte_leger.h = surf_leger->h;
        SDL_FreeSurface(surf_leger);
    }

    // Texte bouton lourd
    SDL_Surface *surf_lourd = TTF_RenderUTF8_Blended((*combat)->font, "Attaque Lourde", blanc);
    if (surf_lourd) {
        (*combat)->texture_texte_lourd = SDL_CreateTextureFromSurface((*combat)->renderer, surf_lourd);
        (*combat)->rect_texte_lourd.w = surf_lourd->w;
        (*combat)->rect_texte_lourd.h = surf_lourd->h;
        SDL_FreeSurface(surf_lourd);
    }

    int running = 1;
    int majAffichage = 1;
    (*combat)->tour = TOUR_JOUEUR;
    SDL_Point point;

    while (running) {
        SDL_Event e;

        while (SDL_PollEvent(&e)) {

            switch (e.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_MOUSEMOTION:
                    point.x = e.motion.x;
                    point.y = e.motion.y;

                    (*combat)->survole_bouton_leger = SDL_PointInRect(&point, &((*combat)->bouton_leger));
                    (*combat)->survole_bouton_lourd = SDL_PointInRect(&point, &((*combat)->bouton_lourd));

                    majAffichage = 1;
                    break;

                // COMBAT
                case SDL_MOUSEBUTTONDOWN:
                    point.x = e.button.x;
                    point.y = e.button.y;

                    if((*combat)->tour == TOUR_JOUEUR){
                        if (SDL_PointInRect(&point, &((*combat)->bouton_leger))) {
                            printf("Attaque légère\n");
                            attaque_legere(*combat);
                            (*combat)->tour = changer_tour(*combat);
                            printf("Le joueur a joué\n");
                            printf("santé monstre : %d",(*combat)->monstre->sante);
                            

                        } else if (SDL_PointInRect(&point, &((*combat)->bouton_lourd))) {
                            printf("Attaque lourde\n");
                            attaque_lourde(*combat);
                            (*combat)->tour = changer_tour(*combat);
                            printf("Le joueur a joué\n");
                            printf("santé monstre : %d",(*combat)->monstre->sante);
                        }

                    }

                    if ((*combat)->tour == TOUR_MONSTRE) {
                        printf("\nTour du monstre\n");
                        choix_attaque_monstre(*combat);
                        (*combat)->tour = changer_tour(*combat);
                        printf("Le monstre a joué\n");
                        printf("Santé joueur : %d\n",(*combat)->perso->sante);

                        majAffichage = 1;
                    }
                    if((*combat)->perso->sante == 0){
                        printf("Vous avez perdu.\n");
                        running = 0;
                    }
                    if((*combat)->monstre->sante == 0){
                        printf("Vous avez gagné.\n");
                        running = 0;
                    }
                    
                    break;

                case SDL_WINDOWEVENT:

                    if (e.window.event == SDL_WINDOWEVENT_RESIZED
                        || e.window.event == SDL_WINDOWEVENT_EXPOSED) {
                        majAffichage = 1;
                    }

                    if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                        running = 0;
                    }

                    break;

            }

        }

        if (majAffichage) {
            maj_affichage_fenetre_combat(*combat);
            majAffichage = 0;
        }

    }

    detruire_fenetre_combat(combat);
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
    dessiner_interface_combat(renderer, combat->font, combat->texture_monstre, 0, "Squelette", monstre->sante, monstre->sante_max, monstre->force, monstre->intelligence, monstre->rapidite, monstre->evasion);

    SDL_RenderPresent(renderer);
}

/* Leo
 * Cette fonction libère la mémoire d'un combat,
 * et de sa fenêtre et des éléments graphiques.
 */
void detruire_fenetre_combat(combat_t ** combat) {
    
    if (combat != NULL && *combat != NULL) {

        if ((*combat)->texture_attaque_legere != NULL) {
            SDL_DestroyTexture((*combat)->texture_attaque_legere);
            (*combat)->texture_attaque_legere = NULL;
        }

        if ((*combat)->texture_attaque_lourde != NULL) {
            SDL_DestroyTexture((*combat)->texture_attaque_lourde);
            (*combat)->texture_attaque_lourde = NULL;
        }

        if ((*combat)->texture_monstre != NULL) {
            SDL_DestroyTexture((*combat)->texture_monstre);
            (*combat)->texture_monstre = NULL;
        }

        if ((*combat)->texture_perso != NULL) {
            SDL_DestroyTexture((*combat)->texture_perso);
            (*combat)->texture_perso = NULL;
        }

        if ((*combat)->texture_fond_ecran != NULL) {
            SDL_DestroyTexture((*combat)->texture_fond_ecran);
            (*combat)->texture_fond_ecran = NULL;
        }

        if (((*combat)->renderer) != NULL) {
            SDL_DestroyRenderer((*combat)->renderer);
            ((*combat)->renderer) = NULL;
        }

        if (((*combat)->pFenetre) != NULL) {
            SDL_DestroyWindow((*combat)->pFenetre);
            ((*combat)->pFenetre) = NULL;
        }

        if (((*combat)->texture_texte_leger) != NULL) {
            SDL_DestroyTexture((*combat)->texture_texte_leger);
            (*combat)->texture_texte_leger = NULL;
        }
        if (((*combat)->texture_texte_lourd) != NULL) {
            SDL_DestroyTexture((*combat)->texture_texte_lourd);
            (*combat)->texture_texte_lourd = NULL;
        }

        free(*combat);
        *combat = NULL;
    }

}



/* Massoud 
 * Cette fonction est faite pour alterner le tour.
*/
tour_t changer_tour(combat_t *combat){
    return (combat->tour == TOUR_JOUEUR) ? TOUR_MONSTRE : TOUR_JOUEUR;
}


/* Massoud
 * Fonctions d'attaque du combat.
 * Chaque attaque génère un pourcentage aléatoire, puis applique
 * ce pourcentage aux dégâts de base de l'attaquant pour calculer
 * les dégâts réels infligés à la cible.
 * La santé de la cible est ensuite diminuée, sans jamais descendre sous 0.
 *
 * Attaque légère :
 *   - Joueur  : entre 10 % et 40 % des dégâts.
 *   - Monstre : entre 5 % et 30 % des dégâts.
 *
 * Attaque lourde :
 *   - Joueur  : entre 30 % et 60 % des dégâts.
 *   - Monstre : entre 20 % et 50 % des dégâts.
 */

void attaque_legere(combat_t *combat){
    int pourcentage = 10 + rand() % 31; // 10% à 40%
    int vrai_degats = (pourcentage * combat->perso->degats)/100;

    combat->monstre->sante -= vrai_degats;

    if (combat->monstre->sante < 0){
        combat->monstre->sante = 0;
    }
}

void attaque_lourde(combat_t *combat){
    int pourcentage = 30 + rand() % 31; // 30% à 60%
    int vrai_degats = (pourcentage * combat->perso->degats)/100;

    combat->monstre->sante -= vrai_degats;

    if (combat->monstre->sante < 0){
        combat->monstre->sante = 0;
    }
}

void attaque_legere_monstre(combat_t *combat){
    int pourcentage = 5 + rand() % 26; // 5% à 30%
    int vrai_degats = (pourcentage * combat->monstre->degats)/100;

    combat->perso->sante -= vrai_degats;

    if(combat->perso->sante < 0){
        combat->perso->sante = 0;
    }
}

void attaque_lourde_monstre(combat_t *combat){
    int pourcentage = 20 + rand() % 31; // 20% à 50%
    int vrai_degats = (pourcentage * combat->monstre->degats)/100;

    combat->perso->sante -= vrai_degats;

    if(combat->perso->sante < 0){
        combat->perso->sante = 0;
    }
}

/* Massoud
 * Cette fonction choisit l'attaque du monstre en fonction de la situation
 * du combat, en gardant une part de hasard pour rendre le comportement
 * moins prévisible.
 *
 * - Si le monstre a peu de vie (≤ 30 % de sa santé maximale), il devient
 *   plus agressif et privilégie l'attaque lourde.
 * - Si le joueur a beaucoup de vie (> 60 % de sa santé maximale), le
 *   monstre attaque fort (préférence pour l'attaque lourde).
 * - Si le joueur a peu de vie (≤ 30 % de sa santé maximale), le monstre
 *   agit plus prudemment (préférence pour l'attaque légère).
 * - Dans tous les autres cas, le monstre choisit de façon équilibrée
 *   entre attaque légère et lourde, avec 50 % de chance pour chacune.
 */
void choix_attaque_monstre(combat_t *combat){
    int hasard = rand() % 100;

    int sante_joueur = combat->perso->sante;
    int sante_max_joueur = combat->perso->sante_max;

    int sante_monstre = combat->monstre->sante;
    int sante_max_monstre = combat->monstre->sante_max;

    /* Si le monstre a peu de vie -> il devient agressif */
    if(sante_monstre <= (sante_max_monstre * 30) / 100){
        if(hasard < 80){
            attaque_lourde_monstre(combat);
        } else {
            attaque_legere_monstre(combat);
        }
    }

    /* Si le joueur a beaucoup de vie -> le monstre attaque fort */
    else if(sante_joueur > (sante_max_joueur * 60) / 100){
        if(hasard < 70){
            attaque_lourde_monstre(combat);
        } else {
            attaque_legere_monstre(combat);
        }
    }

    /* Si le joueur a peu de vie -> le monstre joue plus prudemment */
    else if(sante_joueur <= (sante_max_joueur * 30) / 100){
        if(hasard < 70){
            attaque_legere_monstre(combat);
        } else {
            attaque_lourde_monstre(combat);
        }
    }

    /* Comportement équilibré */
    else {
        if(hasard < 50){
            attaque_legere_monstre(combat);
        } else {
            attaque_lourde_monstre(combat);
        }
    }
}
