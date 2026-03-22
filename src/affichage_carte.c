#include "../lib/affichage_carte.h"

/* Fonctions d'affichage */

/*
On stocke les cos et sin une fois dans un tableau, au lieu de le faire à chaque boucle
Car les angles sont les mêmes pour chaque hexagone et ne changent pas.
Cela permet de gagner en temps de calcul.
*/
/* Modif par Massoud: On sépare les valeurs pour les hexagones plats (carte) et pointus (badges/portrait) */
float HEX_COS[6];
float HEX_SIN[6];
float POINTY_HEX_COS[6];
float POINTY_HEX_SIN[6];

void remplir_hexagone(SDL_Renderer *renderer, float cx, float cy, float rayon, SDL_Color couleur)
{
    SDL_Vertex sommets[7];
    int indices[18] = {0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,1};

    sommets[0].position = (SDL_FPoint){cx, cy};
    sommets[0].color = couleur;

    for(int i = 0; i < 6; i++)
    {
        sommets[i+1].position.x = cx + rayon * HEX_COS[i];
        sommets[i+1].position.y = cy + rayon * HEX_SIN[i];
        sommets[i+1].color = couleur;
    }

    SDL_RenderGeometry(renderer, NULL, sommets, 7, indices, 18);
}

void dessiner_contour_hex(SDL_Renderer *renderer, float cx, float cy, float rayon, SDL_Color couleur)
{
    SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, couleur.a);
    int i;

    for (i = 0; i < 6; i++) {
        int next = (i + 1) % 6;

        float x1 = cx + rayon * HEX_COS[i];
        float y1 = cy + rayon * HEX_SIN[i];

        float x2 = cx + rayon * HEX_COS[next];
        float y2 = cy + rayon * HEX_SIN[next];

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

/* Massoud : Fonction pour dessiner le contour d'un hexagone pointu */
void dessiner_contour_hex_pointy(SDL_Renderer *renderer, float cx, float cy, float rayon, SDL_Color couleur)
{
    SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, couleur.a);
    for (int i = 0; i < 6; i++) {
        int next = (i + 1) % 6;
        float x1 = cx + rayon * POINTY_HEX_COS[i];
        float y1 = cy + rayon * POINTY_HEX_SIN[i];
        float x2 = cx + rayon * POINTY_HEX_COS[next];
        float y2 = cy + rayon * POINTY_HEX_SIN[next];
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void dessiner_contour_dore(SDL_Renderer * renderer, float cx, float cy, float rayon)
{
    SDL_Color or_clair = {218,165,32,200};
    SDL_Color or_sombre = {139,101,8,200};

    dessiner_contour_hex(renderer, cx, cy, rayon, or_clair);
    dessiner_contour_hex(renderer, cx, cy, rayon - 2, or_sombre);
}

void dessiner_hex_texture(SDL_Renderer * renderer, SDL_Texture * texture, float cx, float cy, float rayon)
{
    SDL_Vertex sommets[7];
    int indices[18] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 1 };
    sommets[0].position = (SDL_FPoint){ cx, cy };
    sommets[0].tex_coord = (SDL_FPoint){ 0.5f, 0.5f };
    sommets[0].color = (SDL_Color){ 255, 255, 255, 255 };

    for (int k = 0; k < 6; k++) {
        sommets[k+1].position = (SDL_FPoint){ cx + rayon * HEX_COS[k], cy + rayon * HEX_SIN[k] };
        sommets[k+1].tex_coord = (SDL_FPoint){ 0.5f + 0.5f * HEX_COS[k], 0.5f + 0.5f * HEX_SIN[k] };
        sommets[k+1].color = (SDL_Color){ 255, 255, 255, 255 };
    }

    SDL_RenderGeometry(renderer, texture, sommets, 7, indices, 18);
}

/* Massoud : Fonction pour mapper une texture dans un hexagone pointu */
void dessiner_hex_texture_pointy(SDL_Renderer * renderer, SDL_Texture * texture, float cx, float cy, float rayon)
{
    SDL_Vertex sommets[7];
    int indices[18] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 1 };
    sommets[0].position = (SDL_FPoint){ cx, cy };
    sommets[0].tex_coord = (SDL_FPoint){ 0.5f, 0.5f };
    sommets[0].color = (SDL_Color){ 255, 255, 255, 255 };

    for (int k = 0; k < 6; k++) {
        sommets[k+1].position = (SDL_FPoint){ cx + rayon * POINTY_HEX_COS[k], cy + rayon * POINTY_HEX_SIN[k] };
        // Mapping UV de -1..1 vers 0..1
        sommets[k+1].tex_coord = (SDL_FPoint){ 0.5f + 0.5f * POINTY_HEX_COS[k], 0.5f + 0.5f * POINTY_HEX_SIN[k] };
        sommets[k+1].color = (SDL_Color){ 255, 255, 255, 255 };
    }

    SDL_RenderGeometry(renderer, texture, sommets, 7, indices, 18);
}

/* Léo */
/*
 * Fonction utile pour afficher une texture centrée sur une case
 */
 static void dessiner_texture(SDL_Renderer* renderer, SDL_Texture* texture, float cx, float cy, float taille, float echelle) {
    
    if (texture != NULL) {
        SDL_Rect dst = {
            (int)(cx - taille * echelle / 2),
            (int)(cy - taille * echelle / 2),
            (int)(taille * echelle),
            (int)(taille * echelle)
        };

        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }
}

/* Massoud principalement, Léo secondairement */
/*
 * Cette fonction permet d'afficher la carte avec SDL2.
 * Elle affiche les cases, le brouillard, les monstres
 * qui sont sur les cases, ainsi que les indicateurs
 * qui permettent de voir la case ou le personnage
 * sélectionné
 */
void afficher_carte_sdl(SDL_Renderer * renderer,
    case_t carte[TAILLE_CARTE][TAILLE_CARTE],
    SDL_Texture * textures_cases[NB_BIOMES], 
    SDL_Texture * textures_obstacles[4],
    SDL_Texture * texture_brouillard,
    SDL_Texture * textures_monstres[2],
    SDL_Texture * textures_batiments[3],
    SDL_Texture * textures_sanctuaires[4],
    int tailleCase,
    int persX, int persY,
    int case_selection_x, int case_selection_y,
    perso_t *perso)
{
    int lFenetre, hFenetre;
    SDL_GetRendererOutputSize(renderer, &lFenetre, &hFenetre);
    
    // Activation du mode BLEND pour que l'alpha fonctionne
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    float rayonHex = tailleCase / 2.0f; 

    float largeurHex = 2.0f * rayonHex;
    float hauteurHex = sqrtf(3) * rayonHex;
                   
    float espacementX = largeurHex * 0.75f;       
    float espacementY = hauteurHex;

    /* Position du personnage au centre de la carte */
    float positionPersX = persX * espacementX;
    float decalagePersColonne = (persX % 2) ? hauteurHex / 2.0f : 0.0f;
    float positionPersY = persY * hauteurHex + decalagePersColonne;

    /* Calcul du décalage pour centrer l'affichage sur le personnage */
    int decalageX = (int) (lFenetre / 2 - positionPersX - largeurHex / 2);
    int decalageY = (int) (hFenetre / 2 - positionPersY - hauteurHex / 2);

    /*
     * Au lieu de traiter toute la carte, on calcule les bornes de
     * la carte à traiter, afin d'éviter de faire des tours de
     * boucles sur toute la carte, mais seulement la petite partie
     * visible à l'écran.
     */
    int xMin = max(0, (int) ((-largeurHex / 2 - decalageX) / espacementX));
    int xMax = min(TAILLE_CARTE - 1, (int) ((lFenetre + largeurHex / 2 - decalageX) / espacementX));

    int yMin = max(0, (int)((-hauteurHex / 2 - decalageY) / espacementY));
    int yMax = min(TAILLE_CARTE - 1, (int)((hFenetre + hauteurHex / 2 - decalageY) / espacementY));

    int x;
    int y;

    for (x = xMin; x <= xMax; x++) {
        for (y = yMin; y <= yMax; y++) {
            case_t maCase = carte[y][x];

            float cx = x * espacementX + largeurHex / 2.0f + decalageX;
            float decalageColonne = (x % 2) ? (hauteurHex / 2.0f) : 0.0f;
            float cy = y * espacementY + decalageColonne + hauteurHex / 2.0f + decalageY;

            /* Texture du Biome */
            SDL_Texture *tex_actuelle;

            int estVisible = 1;
                
            if (maCase.estVisible) {
                tex_actuelle = textures_cases[maCase.biome];
                estVisible = 1;
            } else {
                tex_actuelle = texture_brouillard;
                estVisible = 0;
            }

            if (tex_actuelle) {
                dessiner_hex_texture(renderer, tex_actuelle, cx, cy, rayonHex);
            }

            if (estVisible) {
                SDL_Texture *tex_obstacle = NULL;

                /* Affichage des obstacles */
                switch(maCase.terrain) {
                    case ARBRES:    tex_obstacle = textures_obstacles[0]; break;
                    case MONTAGNES: tex_obstacle = textures_obstacles[1]; break;
                    case CACTUS:    tex_obstacle = textures_obstacles[2]; break;
                    case BASSIN_EAU:tex_obstacle = textures_obstacles[3]; break;
                    default: break;
                }

                dessiner_texture(renderer, tex_obstacle, cx, cy, largeurHex, 0.65f);

                /* Affichage du sanctuaire si il existe */
                if (maCase.sanctuaires != PAS_DE_SANCTUAIRE) {
                    // Les enums commencent à 1 pour les sanctuaires (PUISSANCE=1),
                    // donc l'index dans le tableau est maCase.sanctuaires - 1
                    SDL_Texture * texture_sanc = textures_sanctuaires[maCase.sanctuaires - 1];
                    dessiner_texture(renderer, texture_sanc, cx, cy, largeurHex, 0.75f); 
                }

                /* Affichage du monstre si il existe */
                if (maCase.monstre != NULL) {
                    dessiner_texture(renderer, textures_monstres[maCase.monstre->type], cx, cy, largeurHex, 0.65f);
                }

                dessiner_contour_dore(renderer, cx, cy, rayonHex);

                if (maCase.batiment.type != PAS_DE_BATIMENT) {
                    SDL_Texture * texture_batiment = NULL;
                            
                    if (maCase.batiment.type == TOUR_DU_BOSS) {
                        texture_batiment = textures_batiments[1]; 
                    } else if (estVisible) {

                        switch (maCase.batiment.type) {

                            case CAMPEMENT:
                                texture_batiment = textures_batiments[0];
                                break;

                            case TOMBE:
                                texture_batiment = textures_batiments[2];
                                break;

                            default:
                                texture_batiment = textures_batiments[0];
                                break;

                        }

                    }

                    dessiner_texture(renderer, texture_batiment, cx, cy, largeurHex, 0.85f);
                }

            }

        }

    }

    int portee = get_pers_movements_points(perso);

    /* Overlays */
    if (case_selection_x >= 0 && case_selection_y >= 0) {
        float cx = case_selection_x * espacementX + largeurHex / 2.0f + decalageX;
        float decalageColonne = (case_selection_x % 2) ? (hauteurHex / 2.0f) : 0.0f;
        float cy = case_selection_y * espacementY + decalageColonne + hauteurHex / 2.0f + decalageY;
        int distanceChemin = -1;

        if (chemin_valide(carte, persX, persY, case_selection_x, case_selection_y, portee, perso, &distanceChemin)) {
            
            if (carte[case_selection_y][case_selection_x].monstre != NULL) {
                SDL_Color couleur = { 0, 255, 0, 120 }; // Vert : monstre atteignable
                remplir_hexagone(renderer, cx, cy, rayonHex, couleur);
            } else if (carte[case_selection_y][case_selection_x].batiment.type != PAS_DE_BATIMENT) {
                SDL_Color couleur = { 255, 0, 0, 120 }; // Rouge : impossible (pour le moment)
                remplir_hexagone(renderer, cx, cy, rayonHex, couleur);
            } else {
                SDL_Color couleur = { 0, 0, 0, 120 };   // Noir : case vide atteignable
                remplir_hexagone(renderer, cx, cy, rayonHex, couleur);
            }

        } else {
            // Trop loin ou chemin bloqué
            SDL_Color couleur = { 255, 0, 0, 120 };     // Rouge : impossible
            remplir_hexagone(renderer, cx, cy, rayonHex, couleur);
        }

    }

    /*
     * On affiche des indicateurs sur les cases
     * où le personnage peut se déplacer
     */

    if (portee >= 0) {
        int xDepart = max(0, persX - portee);
        int xArrivee = min(TAILLE_CARTE - 1, persX + portee);
        int yDepart = max(0, persY - portee);
        int yArrivee = min(TAILLE_CARTE - 1, persY + portee);

        for (int x = xDepart; x <= xArrivee; x++) {

            for (int y = yDepart; y <= yArrivee; y++) {
                int distanceChemin = -1;
                
                if (chemin_valide(carte, persX, persY, x, y, portee, perso, &distanceChemin)) {
                    float cx = x * espacementX + largeurHex / 2.0f + decalageX;
                    float decalageColonne = (x % 2) ? (hauteurHex / 2.0f) : 0.0f;
                    float cy = y * espacementY + decalageColonne + hauteurHex / 2.0f + decalageY;
                    int tailleCarre = rayonHex * 0.25f;

                    SDL_Rect rect = { (int)cx - tailleCarre / 2, (int)cy - tailleCarre / 2, tailleCarre, tailleCarre };
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                    if (carte[y][x].monstre != NULL) {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200); // Point rouge sur monstre
                    } else {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);   // Point noir sur case libre
                    }

                    if (carte[y][x].batiment.type == PAS_DE_BATIMENT) {
                        SDL_RenderFillRect(renderer, &rect);
                    }

                }

            }

        }

    }

}

/* Leo
 * Cette fonction doit être appelée
 * avant le premier affichage.
 * Elle sert notamment à initialiser les
 * tableaux HEX_SIN, HEX_COS, POINTY_HEX_COS, POINTY_HEX_SIN */
void preparer_avant_affichage() {
    
    /* Calcul des cos/sin pour les hexagones plats (carte) */
    for (int i = 0; i < 6; i++) {
        float angle = (60.0f * i) * (M_PI / 180.0f);
        HEX_COS[i] = cosf(angle);
        HEX_SIN[i] = sinf(angle);
    }
    
    /* Calcul des cos/sin pour les hexagones pointus */
    for (int i = 0; i < 6; i++) {
        // Décalage de 30 degrés pour obtenir la pointe vers le haut
        float angle = (60.0f * i + 30.0f) * (M_PI / 180.0f);
        POINTY_HEX_COS[i] = cosf(angle);
        POINTY_HEX_SIN[i] = sinf(angle);
    }

}

/* Massoud
 * Dessine un badge hexagonal Pointy Top sombre
 * avec un nombre coloré au centre
 */
static void dessiner_insigne_stat(SDL_Renderer* renderer, TTF_Font* font, float cx, float cy, float rayon, const char* valeur, SDL_Color couleur_fond, SDL_Color couleur_texte) {
    SDL_Vertex sommets[7];
    int indices[18] = {0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,1};

    // Centre de l'hexagone
    sommets[0].position = (SDL_FPoint){cx, cy};
    sommets[0].color = couleur_fond;

    // Calcul des 6 sommets pointus
    for(int i = 0; i < 6; i++) {
        sommets[i+1].position.x = cx + rayon * POINTY_HEX_COS[i];
        sommets[i+1].position.y = cy + rayon * POINTY_HEX_SIN[i];
        sommets[i+1].color = couleur_fond;
    }

    // Remplissage de la géométrie
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderGeometry(renderer, NULL, sommets, 7, indices, 18);

    // Dessin du contour plus clair pour délimiter l'hexagone
    SDL_SetRenderDrawColor(renderer, 80, 80, 90, 255);
    for (int i = 0; i < 6; i++) {
        int next = (i + 1) % 6;
        float x1 = cx + rayon * POINTY_HEX_COS[i];
        float y1 = cy + rayon * POINTY_HEX_SIN[i];
        float x2 = cx + rayon * POINTY_HEX_COS[next];
        float y2 = cy + rayon * POINTY_HEX_SIN[next];
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    // Affichage du texte coloré au centre
    SDL_Rect r_txt;
    SDL_Texture* t_txt = creer_texte(renderer, font, valeur, couleur_texte, &r_txt);
    if(t_txt) {
        // On centre le chiffre
        SDL_Rect pos_txt = { (int)(cx - r_txt.w/2.0f), (int)(cy - r_txt.h/2.0f), r_txt.w, r_txt.h };
        SDL_RenderCopy(renderer, t_txt, NULL, &pos_txt);
        SDL_DestroyTexture(t_txt);
    }
}

/* Leo principalement, et Massoud
 * Dessine l'interface du personnage
 * lorsqu'on est sur la carte du jeu
 * Cette fonction ne doit pas être
 * appelée depuis le main.
 */
static void dessiner_interface_carte_bis(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* portrait, const char* nom, int x, int y, int largeur_totale, int hauteur_totale,
    perso_t * perso, int clic_gauche, int * maj_affichage)
{
    int pv = perso->sante;
    int pv_max = perso->sante_max;
    int xp = perso->exp;
    int xp_max = xp_necessaire(perso->niveau);
    int niveau = perso->niveau;
    int force = perso->force;
    int intel = perso->intelligence;
    int rapidite = perso->rapidite;
    int evasion = perso->evasion;

    /* On obtient les coordonnées de la souris sur l'écran */
    int souris_x, souris_y;
    SDL_GetMouseState(&souris_x, &souris_y);

    /* Dessin du fond principal */
    SDL_Rect fond = { x, y, largeur_totale, hauteur_totale };
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    /* Fond sombre bleuté/noir */
    SDL_SetRenderDrawColor(renderer, 25, 25, 30, 240);
    SDL_RenderFillRect(renderer, &fond);
    
    /* Bordure double effet */
    SDL_SetRenderDrawColor(renderer, 100, 100, 110, 255); // Bordure claire extérieure
    SDL_RenderDrawRect(renderer, &fond);
    SDL_Rect fond_int = { x + 2, y + 2, largeur_totale - 4, hauteur_totale - 4 };
    SDL_SetRenderDrawColor(renderer, 45, 45, 55, 255); // Bordure intérieure sombre
    SDL_RenderDrawRect(renderer, &fond_int);

    /* Dessin des insgines (hexagones) */
    float rayon_insigne = 14.0f;
    float espacement_y = 28.0f; // Espace vertical entre les centres
    
    // Le centre X est exactement sur le bord gauche du fond (collé à l'extérieur)
    float insigne_cx = fond.x;
    
    // On centre verticalement les 4 hexagones par rapport au rectangle
    float insigne_cy_depart = fond.y + 24.0f; 

    char val_f[16], val_i[16], val_r[16], val_e[16];
    sprintf(val_f, "%d", force);
    sprintf(val_i, "%d", intel);
    sprintf(val_r, "%d", rapidite);
    sprintf(val_e, "%d", evasion);

    SDL_Color fond_hex_sombre = {30, 30, 35, 255}; // Hexagone très sombre

    // Force (Texte Rouge)
    dessiner_insigne_stat(renderer, font, insigne_cx, insigne_cy_depart, rayon_insigne, val_f, fond_hex_sombre, (SDL_Color){240, 60, 60, 255});
    // Intelligence (Texte Bleu)
    dessiner_insigne_stat(renderer, font, insigne_cx, insigne_cy_depart + espacement_y, rayon_insigne, val_i, fond_hex_sombre, (SDL_Color){60, 150, 255, 255});
    // Rapidité (Texte Vert)
    dessiner_insigne_stat(renderer, font, insigne_cx, insigne_cy_depart + espacement_y * 2, rayon_insigne, val_r, fond_hex_sombre, (SDL_Color){60, 220, 80, 255});
    // Évasion (Texte Jaune/Or)
    dessiner_insigne_stat(renderer, font, insigne_cx, insigne_cy_depart + espacement_y * 3, rayon_insigne, val_e, fond_hex_sombre, (SDL_Color){240, 200, 50, 255});

    /* Portrait en hexagone pointy top */
    float portrait_rayon = 50.0f;
    // On décale le centre X pour ne pas toucher les insignes de gauche
    float portrait_cx = fond.x + 70.0f; 
    float portrait_cy = fond.y + (hauteur_totale / 3.0f); // Centré verticalement
    
    // Fond de l'hexagone de portrait
    SDL_Vertex sommets[7];
    int indices[18] = {0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,1};
    sommets[0].position = (SDL_FPoint){portrait_cx, portrait_cy};
    sommets[0].color = (SDL_Color){40, 40, 45, 255};
    for(int i = 0; i < 6; i++) {
        sommets[i+1].position.x = portrait_cx + portrait_rayon * POINTY_HEX_COS[i];
        sommets[i+1].position.y = portrait_cy + portrait_rayon * POINTY_HEX_SIN[i];
        sommets[i+1].color = (SDL_Color){40, 40, 45, 255};
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderGeometry(renderer, NULL, sommets, 7, indices, 18);

    // Dessin de l'image (texture mappée dans l'hexagone)
    if(portrait) {
        dessiner_hex_texture_pointy(renderer, portrait, portrait_cx, portrait_cy, portrait_rayon);
    }
    
    // Dessin du double contour
    dessiner_contour_hex_pointy(renderer, portrait_cx, portrait_cy, portrait_rayon, (SDL_Color){100, 100, 110, 255});
    dessiner_contour_hex_pointy(renderer, portrait_cx, portrait_cy, portrait_rayon - 1.0f, (SDL_Color){60, 60, 70, 255});

    /* Nom du personnage */
    SDL_Rect r_nom;
    char nomComplet[64];
    sprintf(nomComplet, "%s (Niv. %d)", nom, niveau);
    SDL_Texture* t_nom = creer_texte(renderer, font, nomComplet, (SDL_Color){255, 215, 0, 255}, &r_nom);

    // Le contenu à droite commence après le rayon de l'hexagone portrait + marge
    int start_droite_x = (int)(portrait_cx + portrait_rayon + 15);

    if(t_nom) {
        SDL_Rect pos_nom = { start_droite_x, fond.y + 15, r_nom.w, r_nom.h };
        SDL_RenderCopy(renderer, t_nom, NULL, &pos_nom);
        SDL_DestroyTexture(t_nom);
    }

    dessiner_inventaire(renderer, font, perso, fond, r_nom, clic_gauche, maj_affichage, INV_HORIZONTAL, start_droite_x);

    /* Barres d'xp/pv fixées */
    int h_barre = 14;
    int espace_barres = 6;
    int marge_bords = 12; // Marge entre la barre et le bord du rectangle
    
    // Calcul du Y verrouillé sur le bas
    int y_barres = fond.y + hauteur_totale - marge_bords - (2 * h_barre) - espace_barres;
    
    // Position X et largeur pour que ça traverse tout le bas
    int barre_x = fond.x + marge_bords;
    int largeur_barre_pleine = largeur_totale - (2 * marge_bords); 
    
    // Barre PV (Rouge)
    SDL_Rect barre_pv = { barre_x, y_barres, largeur_barre_pleine, h_barre };
    dessiner_barre(renderer, font, "PV", pv, pv_max, barre_pv, (SDL_Color){220, 60, 60, 255});

    // Barre XP (Bleu)
    SDL_Rect barre_xp = { barre_x, y_barres + h_barre + espace_barres, largeur_barre_pleine, h_barre };
    dessiner_barre(renderer, font, "XP", xp, xp_max, barre_xp, (SDL_Color){60, 150, 255, 255});
}

/* Leo 
 * Dessine l'interface du personnage
 * lorsqu'on est sur la carte du jeu
 */
void dessiner_interface_carte(SDL_Renderer *renderer, TTF_Font* font, SDL_Texture* portrait, perso_t * perso, int clic_gauche, int * maj_affichage) {
    int fenetre_largeur, fenetre_hauteur;

    SDL_GetRendererOutputSize(renderer, &fenetre_largeur, &fenetre_hauteur);

    /* On réduit un peu la taille  */
    int largeur_menu = 300;
    int hauteur_menu = 160;

    /* On le place à gauche au lieu de le centrer */
    int x_menu = 20;

    /* On le garde en bas */
    int y_menu = (fenetre_hauteur - hauteur_menu - 15);

    dessiner_interface_carte_bis(renderer, font, portrait, "Mage Joueur", x_menu, y_menu, largeur_menu, hauteur_menu, perso, clic_gauche, maj_affichage);
}
