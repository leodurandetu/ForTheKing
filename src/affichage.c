#include "../lib/affichage.h"

/* Fonctions d'affichage */

/*
On stocke les cos et sin une fois dans un tableau, au lieu de le faire à chaque boucle 
Car les angles sont les mêmes pour chaque hexagone et ne changent pas.
Cela permet de gagner en temps de calcul.
*/
float HEX_COS[6];
float HEX_SIN[6];

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
    SDL_Texture * textures_batiments[2],
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
 * tableaux HEX_SIN et HEX_COS */
void preparer_avant_affichage() {
    
    /*
     * On calcule les cos et sin une fois, au lieu de le faire à chaque boucle 
     * Car les angles sont les mêmes pour chaque hexagone.
     * Cela est utilisé pour la fonction d'affichage en hexagones.
     */
    for (int i = 0; i < 6; i++) {
        float angle = (60.0f * i) * (M_PI / 180.0f);
        HEX_COS[i] = cosf(angle);
        HEX_SIN[i] = sinf(angle);
    }

}