#include "../lib/carte.h"

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

    while (!deplacement_possible(carte, *x, *y)) {
        *x = rand() % TAILLE_CARTE;
        *y = rand() % TAILLE_CARTE;
    }

}

/* Leo */
/*
 * Cette fonction retourne VRAI si le personnage
 * peut se déplacer sur cette case, FAUX sinon.
 */
int deplacement_possible(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int x, int y) {
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
 * soit par un monstre,
 * soit par un obstacle
 */
int case_occupee(case_t carte[TAILLE_CARTE][TAILLE_CARTE],
    int x, int y, int persX, int persY) {

    if (y == persY && x == persX) {
        return VRAI;
    } else {
        case_t maCase = carte[y][x];

        if (maCase.monstre != NULL || maCase.batiment.type != PAS_DE_BATIMENT
            || maCase.terrain != PAS_DE_TERRAIN) {
            return VRAI;
        } else {
            return FAUX;
        }

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

            /* Par défaut, toutes les cases sont cachées par le brouillard,
             * sauf celles à un rayon de 1 des tours du boss
             */

            if (x >= 1 && x <= 3 && y >= 1 && y <= 3) {
                maCase.estVisible = 1;
            } else if (x >= TAILLE_CARTE - 4 && x <= TAILLE_CARTE - 2 && y >= 1 && y <= 3) {
                maCase.estVisible = 1;
            } else if (x >= TAILLE_CARTE - 4 && x <= TAILLE_CARTE - 2 && y >= TAILLE_CARTE - 4 && y <= TAILLE_CARTE - 2) {
                maCase.estVisible = 1;
            } else if (x >= 1 && x <= 3 && y >= TAILLE_CARTE - 4 && y <= TAILLE_CARTE - 2) {
                maCase.estVisible = 1;
            } else {
                maCase.estVisible = 0;
            }

            maCase.monstre = NULL;
            maCase.terrain = PAS_DE_TERRAIN;

            carte[x][y] = maCase;
        }

    }

}


/* Massoud */
/*
 * Cette fonction permet de placer des cases d'eau
 * sur la carte, de manière aléatoire, et de façon
 * à ce que ça paraisse naturel.
 */
void generer_eau(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
    
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

// Massoud
// Vérifie à un rayon de 2 cases (grille hexagonale) pour éviter que les monstres s'agglutinent
int a_un_voisin_monstre(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int cx, int cy) {
    // On scanne un petit carré de 5x5 autour de la case cible (rayon d'environ 2 hexagones)
    for (int y = cy - 2; y <= cy + 2; y++) {
        for (int x = cx - 2; x <= cx + 2; x++) {
            
            // On vérifie qu'on ne sort pas de la carte
            if (x >= 0 && x < TAILLE_CARTE && y >= 0 && y < TAILLE_CARTE) {
                
                // Si ce n'est pas la case centrale ET qu'il y a un monstre
                if ((x != cx || y != cy) && carte[y][x].monstre != NULL) {
                    
                    // Calcul simplifié de la distance sur grille hexagonale
                    int diff_x = abs(x - cx);
                    int diff_y = abs(y - cy);
                    
                    // Approximation : si c'est dans un rayon de  environ 2 cases -> on refuse
                    if (diff_x + diff_y <= 3) { 
                        return VRAI; 
                    }
                }
            }
        }
    }
    return FAUX;
}

// Massoud
void faire_apparaitre_groupe(case_t carte[TAILLE_CARTE][TAILLE_CARTE], type_monstre_t type, coordonnee_t cases_dispos[], int nb_cases_dispos) {
    // Minimum 15 cases de biome (évite les monstres isolés sur les bords)
    if (nb_cases_dispos < 15) return; 

    // 1 monstre pour 12 cases libres + un léger aléatoire)
    int nb_a_placer = (nb_cases_dispos / 20) + (rand() % 2);
    if (nb_a_placer > nb_cases_dispos) nb_a_placer = nb_cases_dispos; 
    
    int monstres_places = 0;

    while (monstres_places < nb_a_placer && nb_cases_dispos > 0) {
        int index = rand() % nb_cases_dispos;
        int cx = cases_dispos[index].x;
        int cy = cases_dispos[index].y;
        
        if (!a_un_voisin_monstre(carte, cx, cy)) {
            carte[cy][cx].monstre = creer_monstre_aleatoire(type, cx, cy);
            monstres_places++;
        }
        
        // On retire la case testée de la liste pour ne pas boucler dessus à l'infini
        cases_dispos[index] = cases_dispos[nb_cases_dispos - 1];
        nb_cases_dispos--;
    }
}

/* Leo, améliorée par Massoud */
/*
 * Cette fonction permet de placer un certain nombre
 * de monstres sur la carte de manière aléatoire
 */
void placer_monstres(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
    // Taille du secteur scanné (12x12 cases)
    int taille_secteur = 12; 

    for (int i = 0; i < TAILLE_CARTE; i += taille_secteur) {
        for (int j = 0; j < TAILLE_CARTE; j += taille_secteur) {
            coordonnee_t cases_foret[144]; 
            coordonnee_t cases_desert[144];
            int nb_foret = 0, nb_desert = 0;
            
            // Scan du secteur pour lister les cases libres par biome
            for (int y = i; y < i + taille_secteur && y < TAILLE_CARTE; y++) {
                for (int x = j; x < j + taille_secteur && x < TAILLE_CARTE; x++) {
                    
                    if (carte[y][x].monstre == NULL && carte[y][x].terrain == PAS_DE_TERRAIN && 
                        carte[y][x].biome != EAU && carte[y][x].batiment.type == PAS_DE_BATIMENT) {
                        
                        if (carte[y][x].biome == FORET) {
                            cases_foret[nb_foret].x = x;
                            cases_foret[nb_foret].y = y;
                            nb_foret++;
                        } else if (carte[y][x].biome == DESERT) {
                            cases_desert[nb_desert].x = x;
                            cases_desert[nb_desert].y = y;
                            nb_desert++;
                        }
                    }
                }
            }
            
            faire_apparaitre_groupe(carte, TROLL, cases_foret, nb_foret);
            faire_apparaitre_groupe(carte, SQUELETTE, cases_desert, nb_desert);
        }
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
 * Cette fonction place également les 4 tours du boss
 * aux coins de la carte
 */
void placer_batiments(case_t carte[TAILLE_CARTE][TAILLE_CARTE]) {
    int i;
    int nb = 60 + rand() % 38;

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

    carte[2][2].batiment.type = TOUR_DU_BOSS;
    carte[2][TAILLE_CARTE - 3].batiment.type = TOUR_DU_BOSS;

    carte[TAILLE_CARTE - 3][2].batiment.type = TOUR_DU_BOSS;
    carte[TAILLE_CARTE - 3][TAILLE_CARTE - 3].batiment.type = TOUR_DU_BOSS;
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

    int dx[2][6] = {
        { 1,  0, -1, -1, -1,  0 },  // pair
        { 1,  1,  0, -1,  0,  1 }   // impair
    };

    int dy[2][6] = {
        { 0,  1,  1,  0, -1, -1 },  // pair
        { 0,  1,  1,  0, -1, -1 }   // impair
    };

    int cout_deplacement[2][6] = {
        { 1,  1,  2,  1,  2,  1 },  // pair
        { 1,  2,  1,  1,  1,  2}    // impair
    };

    int direction;

    for (direction = 0; direction < 6; direction++) {
        int nx = xDepart + dx[xDepart % 2][direction];
        int ny = yDepart + dy[xDepart % 2][direction];
        int cout = cout_deplacement[xDepart % 2][direction];

        if (nx >= 0 && nx < TAILLE_CARTE && ny >= 0 && ny < TAILLE_CARTE) {

            if (deplacement_possible(carte, nx, ny)) {
                /* Appel récursif */
                int distance = peut_atteindre_rec(carte, nx, ny, xCible, yCible, pts_deplacement_restants - cout, dejaVisite, perso);

                if (distance != INFINI) {

                    if (distance + cout < meilleure_dist) {
                        meilleure_dist = distance + cout;
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

/* Leo
 * Cette fonction déplace tous les monstres d'une case
 * sur la carte dans une direction aléatoire
 */
void deplacer_monstres(case_t carte[TAILLE_CARTE][TAILLE_CARTE], int persX, int persY) {
    /* on suppose qu'on a déjà fait srand(time(NULL)) dans le main */
    int x, y;

    int dejaDeplace[TAILLE_CARTE][TAILLE_CARTE] = {0};

    for (y = 0; y < TAILLE_CARTE; y++) {

        for (x = 0; x < TAILLE_CARTE; x++) {
            monstre_t * monstre = carte[y][x].monstre;

            if (monstre != NULL && !dejaDeplace[y][x]) {
                int direction = rand() % NB_DIRECTIONS;
                int nx = x, ny = y;

                switch (direction) {

                    case HAUT:
                        ny = y - 1;
                        break;

                    case BAS:
                        ny = y + 1;
                        break;

                    case GAUCHE:
                        nx = x - 1;
                        break;

                    case DROITE:
                        nx = x + 1;
                        break;

                }

                dejaDeplace[y][x] = 1;

                if (deplacement_possible(carte, nx, ny)
                    && !case_occupee(carte, nx, ny, persX, persY)) {
                    carte[ny][nx].monstre = monstre;
                    carte[y][x].monstre = NULL;

                    monstre->x = nx;
                    monstre->y = ny;

                    dejaDeplace[ny][nx] = 1;
                }

            }

        }

    }

}