#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "../lib/carte.h"
#include "../lib/couleur.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../lib/monstre.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* =====================================================
// ==================== AFFICHAGE HEX ==================
// =====================================================*/
/* Massoud */

/*
On stocke les cos et sin une fois dans un tableau, au lieu de le faire à chaque boucle 
Car les angles sont les mêmes pour chaque hexagone et ne changent pas.
Cela permet de gagner en temps de calcul.
*/
float HEX_COS[6];
float HEX_SIN[6];

/*
 Dessine un anneau hexagonal (hexagone creux) vers l'intérieur.
 Utilisé ici pour créer une ombre portée interne sous le doré.
*/
void dessiner_contour_ftk(SDL_Renderer* r, float cx, float cy, float rayon, int epaisseur, SDL_Color couleur) {
    SDL_Vertex vertices[12]; 
    int indices[36]; 
    
    float r_ext = rayon;
    float r_int = rayon - epaisseur;

    for (int i = 0; i < 6; i++) {
        float cos_a = HEX_COS[i];
        float sin_a = HEX_SIN[i];

        vertices[i].position = (SDL_FPoint){ cx + r_ext * cos_a, cy + r_ext * sin_a };
        vertices[i].color = couleur;

        vertices[i + 6].position = (SDL_FPoint){ cx + r_int * cos_a, cy + r_int * sin_a };
        // On réduit l'alpha vers l'intérieur pour un effet de fondu doux
        vertices[i + 6].color = (SDL_Color){ couleur.r, couleur.g, couleur.b, (Uint8)(couleur.a / 2) };
    }

    for (int i = 0; i < 6; i++) {
        int next = (i + 1) % 6;
        int base = i * 6;
        indices[base + 0] = i;
        indices[base + 1] = next;
        indices[base + 2] = i + 6;
        indices[base + 3] = next;
        indices[base + 4] = next + 6;
        indices[base + 5] = i + 6;
    }
    SDL_RenderGeometry(r, NULL, vertices, 12, indices, 36);
}


/* Dessine le double filet doré avec transparence */
void dessiner_contour_double_dore(SDL_Renderer* r, float cx, float cy, float rayon, int alpha) {
    SDL_Color or_clair = {218, 165, 32, (Uint8)alpha}; 
    SDL_Color or_sombre = {139, 101, 8, (Uint8)alpha};

    for (int k = 0; k < 6; k++) {
        float cos_a1 = HEX_COS[k];
        float cos_a2 = HEX_COS[(k + 1) % 6]; // en gros il fallait remmetre à 0 l'indice(sinon il allait dessiner là ou il ne voit pas)

        float sin_a1 = HEX_SIN[k];
        float sin_a2 = HEX_SIN[(k + 1) % 6];

        // Filet extérieur (Or clair)
        thickLineRGBA(r, 
            cx + rayon * cos_a1, cy + rayon * sin_a1,
            cx + rayon * cos_a2, cy + rayon * sin_a2,
            1, or_clair.r, or_clair.g, or_clair.b, or_clair.a);

        // Filet intérieur (Or sombre, décalage de 2.2 pixels pour la finesse)
        float r_int = rayon - 2.2f; 
        thickLineRGBA(r, 
            cx + r_int * cos_a1, cy + r_int * sin_a1,
            cx + r_int * cos_a2, cy + r_int * sin_a2,
            1, or_sombre.r, or_sombre.g, or_sombre.b, or_sombre.a);
    }
}

/* Affichage des couleurs des hex */
void afficher_hex_couleur(SDL_Renderer* renderer, float cx, float cy, float rayon, SDL_Color couleur)
{
    /* On utilise la géométrie SDL pour dessiner un hexagone plein à partir de triangles */
    SDL_Vertex sommets[7];
    /* Les indices définissent 6 triangles partant du centre (sommet 0) vers les bords */
    int indices[18] = {
        0, 1, 2,  0, 2, 3,  0, 3, 4,
        0, 4, 5,  0, 5, 6,  0, 6, 1
    };

    /* Centre de l'hexagone */
    sommets[0].position.x = cx;
    sommets[0].position.y = cy;
    sommets[0].color = couleur;

    /* Les 6 coins de l'hexagone (identique au dessin des contours) */
    for (int i = 0; i < 6; i++) {
        sommets[i+1].position.x = cx + rayon * HEX_COS[i];
        sommets[i+1].position.y = cy + rayon * HEX_SIN[i];
        sommets[i+1].color = couleur;
    }

    /* Activation du BLEND pour la transparence */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    /* Rendu des triangles */
    SDL_RenderGeometry(renderer, NULL, sommets, 7, indices, 18);
}

/* Leo et Massoud */
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
    SDL_Texture * texture_brouillard,
    SDL_Texture * texture_monstre,
    int tailleCase,
    int persX, int persY,
    int case_selection_x, int case_selection_y,
    int perso_selectionne)
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

    /*
    Au lieu d'afficher toute la carte tout le temps,
    on affiche que la partie de la carte qui est visible
    sur l'écran, pour gagner du temps de calcul.
    */
    // CORRECTION : J'ai réglé le zoom et dezoom, j'ai prit les coordonnées de l'écran en le soustraynt au décalage
    int j_min = fmax(0, (-decalageX) / espacement_colonnes - 1);
    int j_max = fmin(TAILLE_CARTE, (lFenetre - decalageX) / espacement_colonnes + 2);
    int i_min = fmax(0, (-decalageY) / hex_h - 1);
    int i_max = fmin(TAILLE_CARTE, (hFenetre - decalageY) / hex_h + 2);

    for (int j = j_min; j < j_max; j++) {
        for (int i = i_min; i < i_max; i++) {
            case_t maCase = carte[i][j];

            float cx = j * espacement_colonnes + hex_w / 2.0f + decalageX;
            float cy = i * hex_h + (j % 2 ? hex_h / 2.0f : 0) + hex_h / 2.0f + decalageY;

            /* Texture du Biome */
            SDL_Texture *tex_actuelle;
            
            if (maCase.estVisible) {
                tex_actuelle = textures_cases[maCase.biome];
            } else {
                tex_actuelle = texture_brouillard;
            }

            if (tex_actuelle) {
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
                SDL_RenderGeometry(renderer, tex_actuelle, sommets, 7, indices, 18);
            }

            if (maCase.estVisible) {

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

                /* Double Contour Doré style FTK avec transparence */
                SDL_Color ombre_interne = {0, 0, 0, 90};
                dessiner_contour_ftk(renderer, cx, cy, rayon, 5, ombre_interne);
                
                // Double filet doré avec un alpha de 170/255
                dessiner_contour_double_dore(renderer, cx, cy, rayon, 170);
            }

        }
    }

    /* Overlays */
    if (case_selection_x >= 0 && case_selection_y >= 0) {
        float cx = case_selection_x * espacement_colonnes + hex_w / 2.0f + decalageX;
        float cy = case_selection_y * hex_h + (case_selection_x % 2 ? hex_h / 2.0f : 0) + hex_h / 2.0f + decalageY;
        SDL_Color sel = {255, 255, 255, 60};
        dessiner_contour_ftk(renderer, cx, cy, rayon, (int)rayon, sel); 
    }

    /*
     * Si le personnage est sélectionné,
     * on affiche un halo bleu sur la case
     * du personnage et les cases voisines
     */
    if (perso_selectionne) {
        int xDepart = persX - 1;

        if (xDepart < 0) {
            xDepart = 0;
        }

        int xArrivee = persX + 1;

        if (xArrivee > TAILLE_CARTE - 1) {
            xArrivee = TAILLE_CARTE - 1;
        }

        int yDepart = persY - 1;

        if (yDepart < 0) {
            yDepart = 0;
        }

        int yArrivee = persY + 1;

        if (yArrivee > TAILLE_CARTE - 1) {
            yArrivee = TAILLE_CARTE - 1;
        }


        int x, y;

        for (x = xDepart; x <= xArrivee; x++) {

            for (y = yDepart; y <= yArrivee; y++) {
                float cx = x * espacement_colonnes + hex_w / 2.0f + decalageX;
                float cy = y * hex_h + (x % 2 ? hex_h / 2.0f : 0) + hex_h / 2.0f + decalageY;

                SDL_Color couleurHalo = { 100, 255, 255, 128 };
                dessiner_contour_ftk(renderer, cx, cy, rayon, (int)rayon, couleurHalo);
            }

        }

    }
    
}
/* ========================================================================*/

/* Leo */
/*
 * Cette fonction permet d'afficher la carte en mode textuel,
 * sur le terminal, mais elle ne dispose pas de toutes les
 * fonctionnalités possibles. Pour l'instant, elle n'est
 * pas utilisée.
 */
void afficher_carte(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
    int x, y;

    couleur_reset();

    for (x = 0; x < TAILLE_CARTE; x++) {

        for (y = 0; y < TAILLE_CARTE; y++) {
            case_t maCase = carte[x][y];

            if (!maCase.estVisible) {
                printf("   ");
            } else {

                switch (maCase.biome) {

                    case TERRE:
                        couleur_fond(YELLOW_BG);
                        break;

                    case EAU:
                        couleur_fond(CYAN_BG);
                        break;

                    case DESERT:
                        couleur_fond(ORANGE_BG);
                        break;

                    case NEIGE:
                        couleur_fond(WHITE_BG);
                        break;

                    case FORET:
                        couleur_fond(GREEN_BG);
                        break;

                    default:
                        couleur_fond(DEFAULT_BG);
                        break;

                }

                batiment_t monBatiment = maCase.batiment;

                couleur_texte(BLACK_TEXT);

                switch (monBatiment.type) {

                    case CAMPEMENT:
                        printf(" X ");
                        break;

                    case MAGASIN:
                        printf(" M ");
                        break;

                    default:
                        printf("   ");
                        break;

                }

            }

        }

        couleur_reset();
        printf("\n");
    }

    couleur_reset();

    printf("\n");

}

/* Leo */
/*
 * Cette fonction permet de dévoiler le brouillard sur la carte,
 * à partir d'un point donné d'abscisse x et d'ordonnée y,
 * et avec un rayon donné.
 */
void devoiler_brouillard_rayon(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y, int rayon) {
    int xDepart = x - rayon;

    if (xDepart < 0) {
        xDepart = 0;
    }

    int xArrivee = x + rayon;

    if (xArrivee > (TAILLE_CARTE - 1)) {
        xArrivee = (TAILLE_CARTE - 1);
    }

    int yDepart = y - rayon;

    if (yDepart < 0) {
        yDepart = 0;
    }

    int yArrivee = y + rayon;

    if (yArrivee > (TAILLE_CARTE - 1)) {
        yArrivee = (TAILLE_CARTE - 1);
    }

    int i, j;

    for (i = xDepart; i <= xArrivee; i++) {

        for (j = yDepart; j <= yArrivee; j++) {
            carte[j][i].estVisible = 1;
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
    int nb_zones = 250 + rand() % 101;

    for (int z = 0; z < nb_zones; z++) {
        coordonnee_t dep = { rand() % TAILLE_CARTE, rand() % TAILLE_CARTE };
        
        // Entre 7 et 16 cases de large/haut
        int largeur = 7 + rand() % 10;
        int hauteur = 7 + rand() % 10;

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
    // La TERRE est déjà initialisée partout par init_carte (J'ai fait des modifs)
    biome_t liste[] = {DESERT, NEIGE, FORET};
    
    for (int i = 0; i < 3; i++) {
        // 5 à 8 zones par biome suffisent pour une carte de 50(J'ai cherche des infos sur la carte de for the king)
        int nb_zones = 600 + rand() % 360; 
        
        for (int z = 0; z < nb_zones; z++) {
            coordonnee_t dep = { rand() % TAILLE_CARTE, rand() % TAILLE_CARTE };
            
            // 6 à 14 cases, car maintenant la carte est de taille 500
            int largeur = 6 + rand() % 9;
            int hauteur = 6 + rand() % 9;
            
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
    int nb = 240 + rand() % 121;

    for (i = 0; i < nb; i++) {
        int x = rand() % TAILLE_CARTE;
        int y = rand() % TAILLE_CARTE;

        /* On recommence jusqu'à trouver un endroit convenable,
         * C'est à dire sans monstre, et pas dans l'eau.
         */
        while (carte[x][y].monstre != NULL || carte[x][y].biome == EAU) {
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