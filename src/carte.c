#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "../lib/carte.h"
#include "../lib/couleur.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../lib/monstre.h"
#include "../lib/perso.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Maths */
int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

/* Fonctions d'affichage */

/*
On stocke les cos et sin une fois dans un tableau, au lieu de le faire à chaque boucle 
Car les angles sont les mêmes pour chaque hexagone et ne changent pas.
Cela permet de gagner en temps de calcul.
*/
float HEX_COS[6];
float HEX_SIN[6];

void dessiner_hexagone(SDL_Renderer * renderer, float cx, float cy, float rayon, SDL_Color couleur)
{
    SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, couleur.a);

    int i;

    for (i = 0; i < 6; i++)
    {
        int prochain = (i + 1) % 6;

        float x1 = cx + rayon * HEX_COS[i];
        float y1 = cy + rayon * HEX_SIN[i];

        float x2 = cx + rayon * HEX_COS[prochain];
        float y2 = cy + rayon * HEX_SIN[prochain];

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

}

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

void dessiner_hex(SDL_Renderer * renderer, float cx, float cy, float rayon, SDL_Color couleur, int rempli, int contour)
{

    if (rempli)
    {
        remplir_hexagone(renderer, cx, cy, rayon, couleur);
    }

    if (contour)
    {
        dessiner_hexagone(renderer, cx, cy, rayon, couleur);
    }

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
    SDL_Texture * texture_monstre,
    SDL_Texture * texture_campement,
    int tailleCase,
    int persX, int persY,
    int case_selection_x, int case_selection_y,
    perso_t *perso)
{
    int lFenetre, hFenetre;
    SDL_GetRendererOutputSize(renderer, &lFenetre, &hFenetre);
    
    // Activation du mode BLEND pour que l'alpha fonctionne
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    float rayon = tailleCase / 2.0f;                    
    float hex_w = 2.0f * rayon;                        
    float hex_h = sqrtf(3) * rayon;                    
    float espacement_colonnes = hex_w * 0.75f;       

    float camX = persX * espacement_colonnes;
    float camY = persY * hex_h + (persX % 2 ? hex_h / 2.0f : 0);

    int decalageX = (int)(lFenetre / 2 - camX - hex_w / 2);
    int decalageY = (int)(hFenetre / 2 - camY - hex_h / 2);

    int x;
    int y;

    for (x = 0; x < TAILLE_CARTE; x++) {
        for (y = 0; y < TAILLE_CARTE; y++) {
            case_t maCase = carte[y][x];

            float cx = x * espacement_colonnes + hex_w / 2.0f + decalageX;
            float cy = y * hex_h + (x % 2 ? hex_h / 2.0f : 0) + hex_h / 2.0f + decalageY;

            /* Texture du Biome */
            SDL_Texture *tex_actuelle;
            
            if (maCase.estVisible) {
                tex_actuelle = textures_cases[maCase.biome];
            } else {
                tex_actuelle = texture_brouillard;
            }

            if (tex_actuelle) {
                dessiner_hex_texture(renderer, tex_actuelle, cx, cy, rayon);
            }

            if (maCase.estVisible) {

                SDL_Texture *tex_obstacle = NULL;

                /* Affichage des obstacles */
                switch(maCase.terrain) {
                    case ARBRES:    tex_obstacle = textures_obstacles[0]; break;
                    case MONTAGNES: tex_obstacle = textures_obstacles[1]; break;
                    case CACTUS:    tex_obstacle = textures_obstacles[2]; break;
                    case BASSIN_EAU:tex_obstacle = textures_obstacles[3]; break;
                    default: break;
                }

                if (tex_obstacle != NULL) {
                    float echelle_obs = 0.65f; // ajuster
                    SDL_Rect dstRectObs;
                    dstRectObs.w = (int) (hex_w * echelle_obs);
                    dstRectObs.h = (int) (hex_w * echelle_obs);
                    dstRectObs.x = (int)(cx - dstRectObs.w / 2);
                    dstRectObs.y = (int)(cy - dstRectObs.h / 2);

                    SDL_RenderCopy(renderer, tex_obstacle, NULL, &dstRectObs);
                }

                /* Affichage du monstre si il existe */
                if (maCase.monstre != NULL) {

                    if (texture_monstre) {
                        float echelle = 0.65f;

                        SDL_Rect dstRect;
                        dstRect.w = (int) (hex_w * echelle);
                        dstRect.h = (int) (hex_w * echelle); // Carré
                        /* Centrage automatique */
                        dstRect.x = (int)(cx - dstRect.w / 2);
                        dstRect.y = (int)(cy - dstRect.h / 2);

                        SDL_RenderCopy(renderer, texture_monstre, NULL, &dstRect);
                    }

                }

                if (maCase.batiment.type != PAS_DE_BATIMENT) {
                    
                    if (texture_campement) {
                        float echelle = 0.85f;

                        SDL_Rect dstRect;
                        dstRect.w = (int) (hex_w * echelle);
                        dstRect.h = (int) (hex_w * echelle); // Carré
                        /* Centrage automatique */
                        dstRect.x = (int)(cx - dstRect.w / 2);
                        dstRect.y = (int)(cy - dstRect.h / 2);

                        SDL_RenderCopy(renderer, texture_campement, NULL, &dstRect);
                    }

                }

                dessiner_contour_dore(renderer, cx, cy, rayon);    
            }

        }
    }

    int portee = get_pers_movements_points(perso);

    /* Overlays */
    if (case_selection_x >= 0 && case_selection_y >= 0) {
        float cx = case_selection_x * espacement_colonnes + hex_w / 2.0f + decalageX;
        float cy = case_selection_y * hex_h + (case_selection_x % 2 ? hex_h / 2.0f : 0) + hex_h / 2.0f + decalageY;
        int distanceChemin = -1;

        if (chemin_valide(carte, persX, persY, case_selection_x, case_selection_y, portee, perso, &distanceChemin)) {
            
            if (carte[case_selection_y][case_selection_x].monstre != NULL) {
                SDL_Color couleur = { 0, 255, 0, 120 }; // Vert : monstre atteignable
                remplir_hexagone(renderer, cx, cy, rayon, couleur);
            } else if (carte[case_selection_y][case_selection_x].batiment.type != PAS_DE_BATIMENT) {
                SDL_Color couleur = { 255, 0, 0, 120 }; // Rouge : impossible (pour le moment)
                remplir_hexagone(renderer, cx, cy, rayon, couleur);
            } else {
                SDL_Color couleur = { 0, 0, 0, 120 };   // Noir : case vide atteignable
                remplir_hexagone(renderer, cx, cy, rayon, couleur);
            }

        } else {
            // Trop loin ou chemin bloqué
            SDL_Color couleur = { 255, 0, 0, 120 };     // Rouge : impossible
            remplir_hexagone(renderer, cx, cy, rayon, couleur);
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
                    float cx = x * espacement_colonnes + hex_w / 2.0f + decalageX;
                    float cy = y * hex_h + (x % 2 ? hex_h / 2.0f : 0) + hex_h / 2.0f + decalageY;
                    int tailleCarre = rayon * 0.25f;

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
/* ========================================================================*/

/* Leo */
/*
 * Cette fonction renvoie les coordonnées
 * d'une case libre sur la carte choisie aléatoirement
 * (sans monstres, batiments et pas dans l'eau) 
*/
void coords_case_libre(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int *x, int *y) {
    *x = rand() % TAILLE_CARTE;
    *y = rand() % TAILLE_CARTE;

    while (!deplacement_possible(carte, NULL, *x, *y)) {
        *x = rand() % TAILLE_CARTE;
        *y = rand() % TAILLE_CARTE;
    }

}

/* Leo */
/*
 * Cette fonction retourne VRAI si le personnage
 * peut se déplacer sur cette case, FAUX sinon.
 */
int deplacement_possible(case_t carte[TAILLE_CARTE][TAILLE_CARTE], perso_t *perso, int x, int y) {
    case_t maCase = carte[y][x];

    if (maCase.biome == EAU) {
        return FAUX;
    } else if (maCase.terrain != PAS_DE_TERRAIN) {
        return FAUX;
    } else {
        return VRAI;
    }

}

/* Leo */
/*
 * Cette fonction retourne VRAI si la case
 * est occupée soit par un batiment,
 * soit par un monstre.
 */
int case_occupee(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y) {
    case_t maCase = carte[y][x];

    if (maCase.monstre != NULL || maCase.batiment.type != PAS_DE_BATIMENT) {
        return VRAI;
    } else {
        return FAUX;
    }

}


/* Leo et Saandi */
/*
 * Cette fonction permet de dévoiler le brouillard sur la carte,
 * à partir d'un point donné d'abscisse x et d'ordonnée y,
 * et avec un rayon donné.
 */
void devoiler_brouillard_rayon(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y, int rayon) {
     int i, j;

    for (i = 0; i < TAILLE_CARTE; i++) {
        for (j = 0; j < TAILLE_CARTE; j++) {

            // Calcul de la distance entre la case (i, j) et le centre (x, y)
            int dx = i - x;
            int dy = j - y;
            int distanceCarree = dx * dx + dy * dy;

            // Si la case est dans le cercle, on la dévoile
            if (distanceCarree <= rayon * rayon) {
                carte[j][i].estVisible = 1;
            }
        }
    }

}

/* Leo */
/*
 * Cette fonction permet d'initialiser la carte,
 * c'est à dire d'initialiser toutes les cases
 * à une case de terre, vide, cachée par le brouillard,
 * sans monstre ni batiment.
 */
void init_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
    int x, y;

    for (x = 0; x < TAILLE_CARTE; x++) {

        for (y = 0; y < TAILLE_CARTE; y++) {
            case_t maCase;
            batiment_t monBatiment;

            monBatiment.type = PAS_DE_BATIMENT;

            maCase.biome = TERRE;
            maCase.batiment = monBatiment;

            /* Par défaut, toutes les cases sont cachées par le brouillard */
            maCase.estVisible = 0;

            maCase.monstre = NULL;

            carte[x][y] = maCase;
        }

    }

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


/* Massoud */
/*
 * Cette fonction permet de placer des cases d'eau
 * sur la carte, de manière aléatoire, et de façon
 * à ce que ça paraisse naturel.
 */
void generer_eau(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
    // 70 à 111 zones
    int nb_zones = 15 + rand() % 25;

    for (int z = 0; z < nb_zones; z++) {
        coordonnee_t dep = { rand() % TAILLE_CARTE, rand() % TAILLE_CARTE };
        
        // Entre 7 et 15 cases de large/haut
        int largeur = 7 + rand() % 9;
        int hauteur = 7 + rand() % 9;

        coordonnee_t arr = { dep.x + largeur, dep.y + hauteur };
        remplir_zone(carte, EAU, dep, arr);
    }
}

/*Saandi*/
void remplir_zone(case_t carte[TAILLE_CARTE][TAILLE_CARTE], biome_t biome, coordonnee_t dep, coordonnee_t arr) {
    int x_min = fmax(0, fmin(dep.x, arr.x));
    int x_max = fmin(TAILLE_CARTE - 1, fmax(dep.x, arr.x));
    int y_min = fmax(0, fmin(dep.y, arr.y));
    int y_max = fmin(TAILLE_CARTE - 1, fmax(dep.y, arr.y));
    
    for (int i = y_min; i <= y_max; i++) {
        for (int j = x_min; j <= x_max; j++) {
            carte[i][j].biome = biome;
        }
    }
}
/* Saandi */
void generer_biomes(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
    // La TERRE est déjà initialisée partout par init_carte
    biome_t liste[] = {DESERT, NEIGE, FORET};
    
    for (int i = 0; i < 3; i++) {
        // 28 à 42 zones
        int nb_zones = 28 + rand() % 15; 
        
        for (int z = 0; z < nb_zones; z++) {
            coordonnee_t dep = { rand() % TAILLE_CARTE, rand() % TAILLE_CARTE };
            
            // 5 à 11 cases
            int largeur = 5 + rand() % 7;
            int hauteur = 5 + rand() % 7;
            
            coordonnee_t arr = { dep.x + largeur, dep.y + hauteur };
            remplir_zone(carte, liste[i], dep, arr);
        }
    }
}

/* Leo */
/*
 * Cette fonction permet de placer un certain nombre
 * de monstres sur la carte de manière aléatoire
 */
void placer_monstres(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
    int i;
    int nb = 70 + rand() % 46;

    for (i = 0; i < nb; i++) {
        int x = rand() % TAILLE_CARTE;
        int y = rand() % TAILLE_CARTE;

        /* On recommence jusqu'à trouver un endroit convenable,
         * C'est à dire sans monstre, et pas dans l'eau.
         */
        while (carte[x][y].monstre != NULL || carte[x][y].biome == EAU || carte[x][y].terrain != PAS_DE_TERRAIN) {
            x = rand() % TAILLE_CARTE;
            y = rand() % TAILLE_CARTE;
        }

        monstre_t *monstre = creer_monstre_aleatoire(x, y);

        carte[x][y].monstre = monstre;
    }

}

/* Leo */
/*
 * Cette fonction permet de libérer la mémoire à la fin
 * du programme, tout ce qui est relié à la carte.
 * Notamment les pointeurs vers les monstres qui sont 
 * stockés (ou pas) sur chaque case de la carte.
 */
void liberer_memoire_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
    int i, j;
    
    for (i = 0; i < TAILLE_CARTE; i++) {

        for (j = 0; j < TAILLE_CARTE; j++) {
            monstre_t *monstre = carte[i][j].monstre;

            if (monstre != NULL) {
                free(monstre);
            }

        }

    }

}

/* Leo */
/*
 * Cette fonction place des batiments aléatoirement
 * sur la carte : campements et magasins.
 */
void placer_batiments(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
    int i;
    int nb = 70 + rand() % 46;

    for (i = 0; i < nb; i++) {
        int x = rand() % TAILLE_CARTE;
        int y = rand() % TAILLE_CARTE;

        /* On recommence jusqu'à trouver un endroit convenable,
         * C'est à dire sans monstre, et pas dans l'eau.
         */
        while (carte[x][y].monstre != NULL || carte[x][y].biome == EAU || carte[x][y].terrain != PAS_DE_TERRAIN) {
            x = rand() % TAILLE_CARTE;
            y = rand() % TAILLE_CARTE;
        }

        carte[x][y].batiment.type = CAMPEMENT;
    }

}

/* Crée par Massoud, transformée en fonction sans modifier le contenu par Léo */
/*
 * Cette fonction permet de détecter la case
 * qui est à la position de la souris à l'écran
 */
void souris_vers_case(int mouseX, int mouseY,
    int *carte_x, int *carte_y, int tailleCase,
    perso_t *perso, SDL_Renderer *renderer) {
   
    int fenetre_taille_x, fenetre_taille_y;
    SDL_GetRendererOutputSize(renderer, &fenetre_taille_x, &fenetre_taille_y);

    // Mathématiques de la grille "flat-top"
    float rayon = tailleCase / 2.0f;                    
    float hex_w = 2.0f * rayon;                        
    float hex_h = sqrtf(3) * rayon;                    
    float espacement_colonnes = hex_w * 0.75f; 

    // Décalage de la caméra centrée sur le joueur
    float camX = perso->x * espacement_colonnes;
    float camY = perso->y * hex_h + (perso->x % 2 ? hex_h / 2 : 0);
    int decalageX = (int)(fenetre_taille_x / 2 - camX - hex_w / 2);
    int decalageY = (int)(fenetre_taille_y / 2 - camY - hex_h / 2);

    int result_x = -1;
    int result_y = -1;

    // Détection du clic hexagonal par calcul de distance
    float dist_min_carree = rayon * rayon; 

    // Approximation de la zone pour ne pas vérifier les milliers de cases
    int approx_x = (int)((mouseX - decalageX) / espacement_colonnes);
    int approx_y = (int)((mouseY - decalageY) / hex_h);

    // On teste un carré de 3x3 autour de la souris
    for (int i = approx_y - 1; i <= approx_y + 1; i++) {
        for (int j = approx_x - 1; j <= approx_x + 1; j++) {
            if (i >= 0 && i < TAILLE_CARTE && j >= 0 && j < TAILLE_CARTE) {
                float cx = j * espacement_colonnes + hex_w / 2 + decalageX;
                float cy = i * hex_h + (j % 2 ? hex_h / 2 : 0) + hex_h / 2 + decalageY;

                // Théorème de Pythagore pour trouver le centre le plus proche
                float dx = mouseX - cx;
                float dy = mouseY - cy;
                float dist_carree = dx * dx + dy * dy;

                if (dist_carree < dist_min_carree) {
                    dist_min_carree = dist_carree;
                    result_x = j;
                    result_y = i;
                }
            }
        }
    }

    *carte_x = result_x;
    *carte_y = result_y;
}

/* Leo */
/*
 * Attention : ne pas appeler cette fonction directement,
 * mais plutôt chemin_valide
 *
 * Cette fonction retourne la distance d'un chemin, si il y a un
 * chemin constitué de cases valides (pas d'eau, pas de monstre, 
 * pas de batiment par-dessus) pour arriver à une case 
 * destination, ou INFINI sinon.
 * 
 * Elle est récursive pour l'instant.
 */
static int peut_atteindre_rec(case_t carte[TAILLE_CARTE][TAILLE_CARTE], 
    int xDepart, int yDepart, int xCible, int yCible, 
    int pts_deplacement_restants, int dejaVisite[TAILLE_CARTE][TAILLE_CARTE], perso_t *perso)
{

    /* Cas d'arrêt : on est arrivé à destination */
    if (xDepart == xCible && yDepart == yCible) {
        return 0;
    }

    /* Si l'un des chemins va en dehors de la carte */
    if (xCible < 0 || xDepart < 0 || xCible >= TAILLE_CARTE || xDepart >= TAILLE_CARTE
        || yCible < 0 || yDepart < 0 || yCible >= TAILLE_CARTE || yDepart >= TAILLE_CARTE) {
        return INFINI;
    }

    if (pts_deplacement_restants <= 0) {
        /* on renvoie INFINI quand on ne peut pas atteindre la case finale */
        return INFINI;
    }

    /* On évite les boucles en regardant
       si on a déjà visité le chemin */
    if (dejaVisite[yDepart][xDepart]) {
        return INFINI;
    }

    dejaVisite[yDepart][xDepart] = VRAI;
    int meilleure_dist = INFINI;

    int dx[] = {  1,  1,  0, -1, -1,  0};
    int dy[] = {  0, -1, -1,  0,  1,  1};

    int direction;

    for (direction = 0; direction < 6; direction++) {
        int nx = xDepart + dx[direction];
        int ny = yDepart + dy[direction];

        if (nx >= 0 && nx < TAILLE_CARTE && ny >= 0 && ny < TAILLE_CARTE) {

            if (deplacement_possible(carte, perso, nx, ny)) {
                /* Appel récursif */
                int distance = peut_atteindre_rec(carte, nx, ny, xCible, yCible, pts_deplacement_restants - 1, dejaVisite, perso);

                if (distance != INFINI) {

                    if (distance + 1 < meilleure_dist) {
                        meilleure_dist = distance + 1;
                    }

                }

            }

        }

    }

    dejaVisite[yDepart][xDepart] = FAUX;

    return meilleure_dist;
}

/* Leo */
/*
 * Cette fonction permet de savoir si il y a un chemin
 * constitué de cases valides (pas d'eau, pas de monstre, 
 * pas de batiment par-dessus) pour arriver à une case 
 * destination.
 * Elle retourne également la distance du chemin, à travers
 * le paramètre pointeur sur entier *distance, ou -1 si il
 * n'y a pas de chemin valide.
 */
int chemin_valide(case_t carte[TAILLE_CARTE][TAILLE_CARTE],
    int xDepart, int yDepart, int xCible, int yCible, 
    int pts_deplacement_max, perso_t *perso, int *distance)
{

    if (pts_deplacement_max < 0) {
        return FAUX;
    }

    /* on prépare un tableau de int qui regarde
       si une case a déjà été visitée pour éviter
       des appels récursifs inutiles */
    int dejaVisite[TAILLE_CARTE][TAILLE_CARTE];

    int i, j;

    for (i = 0; i < TAILLE_CARTE; i++) {

        for (j = 0; j < TAILLE_CARTE; j++) {
            dejaVisite[i][j] = FAUX;
        }
    }

    int resultat = peut_atteindre_rec(
        carte, xDepart, yDepart, xCible, 
        yCible, pts_deplacement_max, dejaVisite, perso
    );

    if (resultat != INFINI) {
        *distance = resultat;
        return VRAI;
    } else {
        *distance = -1;
        return FAUX;
    }

}


/* Massoud */
/*
 * Cette fonction ajoute des obstacles
 * pour chaque type de biome, tels que les arbres,
 * montagnes, cactus et la boue, aléatoirement.
 */
void ajout_obstacles(case_t carte[TAILLE_CARTE][TAILLE_CARTE]){
    int i, j;

    for(i = 0; i < TAILLE_CARTE; i++){
        for(j = 0; j < TAILLE_CARTE; j++){

            carte[i][j].terrain = PAS_DE_TERRAIN;

            int r = rand() % 100;

            switch(carte[i][j].biome){

                case FORET:
                    if(r < 5){
                        carte[i][j].terrain = ARBRES;
                    }
                    break;

                case NEIGE:
                    if(r < 5){
                        carte[i][j].terrain = MONTAGNES;
                    }
                    break;

                case DESERT:
                    if(r < 5){
                        carte[i][j].terrain = CACTUS;
                    }
                    break;

                case TERRE:
                    if(r < 5){
                        carte[i][j].terrain = BASSIN_EAU;
                    }
                    break;

                default:
                    break;
            }
        }
    }
}
